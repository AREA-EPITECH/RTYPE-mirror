/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Models_systems
*/

#include "ecs/Systems.hpp"

namespace ecs {

    /**
    * Load all vessels in lobby
    * @param ecs
    */
    void load_models_system(Registry &ecs, const InitModelEvent &) {
        std::vector<std::string> vox_files;
        const int screenWidth = GetScreenWidth();

        for (const auto &entry: std::filesystem::directory_iterator("client/assets/voxels/player/spaceship")) {
            if (std::string file = entry.path().c_str(); file.find(".vox") != std::string::npos)
                vox_files.emplace_back(file);
        }

        for (int i = 0; i < vox_files.size(); i++) {
            Model models;
            const double t0 = GetTime() * 1000.0;

            TraceLog(LOG_WARNING, TextFormat("Trying to load file %s...", vox_files[i].c_str()));
            models = LoadModel(vox_files[i].c_str());
            const double t1 = GetTime() * 1000.0;
            TraceLog(LOG_WARNING, TextFormat("Loaded file %s in %f ms.", vox_files[i].c_str(), t1 - t0));

            auto [min, max] = GetModelBoundingBox(models);
            Vector3 center = {};
            center.x = min.x + (max.x - min.x) / 2;
            center.z = min.z + (max.z - min.z) / 2;

            const Matrix matTranslate = MatrixTranslate(-center.x, 0, -center.z);
            models.transform = matTranslate;

            auto ModelEntity = ecs.spawn_entity();
            std::cout << "MODEL ID : " << ModelEntity << std::endl;

            std::string name_str = "player_vessel";
            int fontSize = 54;
            int textWidth = MeasureText(name_str.c_str(), fontSize);
            int posX = static_cast<int>(screenWidth * 0.66) - textWidth / 2 + 20;

            TextComponent vessel_name(name_str, fontSize, posX, 100, 0, {120, 0, 0, 255});
            ecs.add_component<VesselsComponent>(ModelEntity, {models, (i == 0), vox_files[i], vessel_name});
        }
    }

    /**
    * Load vessels by filepath
    * @param ecs
    */
    void load_model_from_file_system(Registry &ecs, const InitModelEvent &) {
        auto &vessels = ecs.get_components<VesselsComponent>();
        for (std::size_t i = 0; i < vessels.size(); ++i) {
            if (vessels[i].has_value()) {
                auto &modelComponent = vessels[i].value();

                UnloadModel(modelComponent.model);

                if (!std::filesystem::exists(modelComponent.path)) {
                    TraceLog(LOG_ERROR, TextFormat("File %s does not exist!", modelComponent.path.c_str()));
                    continue;
                }

                const double t0 = GetTime() * 1000.0;
                TraceLog(LOG_WARNING, TextFormat("Reloading model from %s...", modelComponent.path.c_str()));
                modelComponent.model = LoadModel(modelComponent.path.c_str());
                const double t1 = GetTime() * 1000.0;
                TraceLog(LOG_WARNING, TextFormat("Reloaded model from %s in %f ms.", modelComponent.path.c_str(), t1 - t0));

                auto [min, max] = GetModelBoundingBox(modelComponent.model);
                Vector3 center = {};
                center.x = min.x + (max.x - min.x) / 2;
                center.z = min.z + (max.z - min.z) / 2;

                const Matrix matTranslate = MatrixTranslate(-center.x, 0, -center.z);
                const Matrix matRotate = MatrixRotateY(DEG2RAD * 90.0f);
                modelComponent.model.transform = MatrixMultiply(matTranslate, matRotate);

                ecs.add_component<ControllableComponent>(i, {});
                TraceLog(LOG_INFO, TextFormat("Model reloaded and centered from %s", modelComponent.path.c_str()));
            }
        }
    }

    /**
    * Load shaders by filepath
    * @param ecs
    */
    void load_shader_from_file_system(Registry &ecs, const InitShaderEvent &) {
        auto &shaders = ecs.get_components<ShaderComponent>();
        for (std::size_t i = 0; i < shaders.size(); ++i) {
            if (shaders[i].has_value()) {
                auto &shader_component = shaders[i].value();
                std::string vs_file = shader_component.vs_file;
                std::string fs_file = shader_component.fs_file;

                UnloadShader(shader_component.shader);

                TraceLog(LOG_WARNING, TextFormat("Trying to load shader from files %s and %s.",
                    shader_component.vs_file.c_str(), shader_component.fs_file.c_str()));
                if (!std::filesystem::exists(vs_file) ||
                    !std::filesystem::exists(fs_file)) {
                    TraceLog(LOG_ERROR, TextFormat("\n\nFile %s nor %s do not exist!", shader_component.vs_file.c_str(),
                        shader_component.fs_file.c_str()));
                    continue;
                }

                const double t0 = GetTime() * 1000.0;
                shader_component.shader = LoadShader(vs_file.c_str(),
                    fs_file.c_str());
                const double t1 = GetTime() * 1000.0;
                TraceLog(LOG_WARNING, TextFormat("Reloaded shader in %f ms.", t1 - t0));
                shader_component.shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader_component.shader,
                "viewPos");
                int ambientLoc = GetShaderLocation(shader_component.shader, "ambient");
                SetShaderValue(shader_component.shader, ambientLoc, (float[4]) {0.1f, 0.1f, 0.1f, 1.0f},
                SHADER_UNIFORM_VEC4);
            }
        }
    }

    void load_title_menu(Registry &ecs, const InitModelEvent &) {

        std::string title_file = "client/assets/voxels/title/r-type.vox";
        Model models;
        const double t0 = GetTime() * 1000.0;

        TraceLog(LOG_WARNING, TextFormat("Trying to load file %s...", title_file.c_str()));
        models = LoadModel(title_file.c_str());
        const double t1 = GetTime() * 1000.0;
        TraceLog(LOG_WARNING, TextFormat("Loaded file %s in %f ms.", title_file.c_str(), t1 - t0));

        auto [min, max] = GetModelBoundingBox(models);
        Vector3 center = {};
        center.x = min.x + (max.x - min.x) / 2;
        center.z = min.z + (max.z - min.z) / 2;

        const Matrix matTranslate = MatrixTranslate(-center.x, -1.5, -center.z);
        models.transform = matTranslate;

        auto ModelEntity = ecs.spawn_entity();
        std::cout << "MODEL ID : " << ModelEntity << std::endl;

        ecs.add_component<MenuText>(ModelEntity, {models});
    }

    /**
    * Create the camera
    * @param ecs
    * @param event
    */
    void create_camera_system(Registry &ecs, const InitCameraEvent &event) {
        auto entity = ecs.spawn_entity();

        Camera camera = {};
        camera.position = event.position;
        camera.target = event.target;
        camera.up = event.up;
        camera.fovy = event.fovy;
        camera.projection = event.projection;

        ecs.add_component<CameraComponent>(entity, {camera});

        TraceLog(LOG_INFO, TextFormat(
                "Created camera with properties: position (%f, %f, %f), target (%f, %f, %f), up (%f, %f, %f), fovy %f.",
                event.position.x, event.position.y, event.position.z,
                event.target.x, event.target.y, event.target.z,
                event.up.x, event.up.y, event.up.z,
                event.fovy
        ));
    }

    /**
     * @brief load shaders
     * @param ecs
     * @param event
     */
    void load_shaders(Registry &ecs, const InitShaderEvent &event)
    {
        Shader shader = LoadShader(event.vs_file.c_str(), event.fs_file.c_str());
        shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
        int ambientLoc = GetShaderLocation(shader, "ambient");
        SetShaderValue(shader, ambientLoc, (float[4]) {0.1f, 0.1f, 0.1f, 1.0f}, SHADER_UNIFORM_VEC4);

        auto entity = ecs.spawn_entity();
        ecs.add_component<ShaderComponent>(entity, {shader, event.vs_file, event.fs_file});
    }

    /**
     * @brief Set shader values
     * @param ecs
     */
    void set_shader_values(Registry &ecs, const InitShaderEvent &)
    {
        auto &shaders = ecs.get_components<ShaderComponent>();
        Shader shader = {};
        for (std::size_t i = 0; i < shaders.size(); ++i) {
            if (shaders[i].has_value()) {
                shader = shaders[i]->shader;
                break;
            }
        }

        auto &cameras = ecs.get_components<CameraComponent>();
        Camera camera = {};
        for (std::size_t i = 0; i < cameras.size(); ++i) {
            if (cameras[i].has_value()) {
                camera = cameras[i]->camera;
                break;
            }
        }

        float camera_pos[3] = {camera.position.x, camera.position.y, camera.position.z};
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], camera_pos, SHADER_UNIFORM_VEC3);
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], camera_pos, SHADER_UNIFORM_VEC3);
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], camera_pos, SHADER_UNIFORM_VEC3);
    }

    /**
     * @brief Apply shaders on vessels
     * @param ecs
     */
    void apply_shader_system(Registry &ecs, const InitShaderEvent &) {
        auto &vessels_models = ecs.get_components<VesselsComponent>();
        auto &projectiles_models = ecs.get_components<ProjectilesComponent>();
        auto &shaders = ecs.get_components<ShaderComponent>();
        auto &menus = ecs.get_components<MenuText>();
        Shader shader = {};
        for (std::size_t i = 0; i < shaders.size(); ++i) {
            if (shaders[i].has_value()) {
                shader = shaders[i]->shader;
                break;
            }
        }

        for (std::size_t i = 0; i < vessels_models.size(); ++i) {
            if (vessels_models[i].has_value()) {
                Model &model = vessels_models[i]->model;
                for (int j = 0; j < model.materialCount; ++j) {
                    model.materials[j].shader = shader;
                }
                TraceLog(LOG_INFO, TextFormat("Applied shader to model of entity %zu.", i));
            }
        }
        for (std::size_t i = 0; i < projectiles_models.size(); ++i) {
            if (projectiles_models[i].has_value()) {
                Model &model = projectiles_models[i]->model;
                for (int j = 0; j < model.materialCount; ++j) {
                    model.materials[j].shader = shader;
                }
                TraceLog(LOG_INFO, TextFormat("Applied shader to model of entity %zu.", i));
            }
        }
        for (std::size_t i = 0; i < menus.size(); ++i) {
            if (menus[i].has_value()) {
                Model &model = menus[i]->model;
                for (int j = 0; j < model.materialCount; ++j) {
                    model.materials[j].shader = shader;
                }
                TraceLog(LOG_INFO, TextFormat("Applied shader to model of entity %zu.", i));
            }
        }
    }

    /**
     * @brief Create light system
     * @param ecs
     * @param event
     */
    void create_light_system(Registry &ecs, const InitLightEvent &event)
    {
        const client::Light light{event.type, event.position, event.target, event.color, event.nb};
        auto entity = ecs.spawn_entity();
        ecs.add_component<LightComponent>(entity, {std::make_shared<client::Light>(light)});
    }

    /**
     * @brief Load background
     * @param ecs
     * @param event
     */
    void load_background(Registry &ecs, const InitBackgroundEvent &event)
    {
        BackgroundComponent background{event.path, event.depth, event.speed, event.offset};
        auto entity = ecs.spawn_entity();
        ecs.add_component<BackgroundComponent>(entity, {(std::move(background))});
    }


    /**
     * @brief Load decor element
     * @param ecs
     * @param event
     */
    void load_decor_element(Registry &ecs, const InitDecorElementEvent &event)
    {
        DecorElementComponent decor_element{event.path, event.speed, event.depth};
        auto entity = ecs.spawn_entity();
        ecs.add_component<DecorElementComponent>(entity, {(std::move(decor_element))});
    }

    /**
     * @brief Load projectiles models
     * @param ecs
     * @param
     */
    void load_projectiles_models(Registry &ecs, const InitModelEvent &)
    {
        std::vector<std::string> vox_files_enemy;
        std::vector<std::string> vox_files_player;

        for (const auto &entry: std::filesystem::directory_iterator("client/assets/voxels/player/shot")) {
            if (std::string file = entry.path().c_str(); file.find(".vox") != std::string::npos)
                vox_files_player.emplace_back(file);
        }

        for (const auto &entry: std::filesystem::directory_iterator("client/assets/voxels/enemy/shot")) {
            if (std::string file = entry.path().c_str(); file.find(".vox") != std::string::npos)
                vox_files_enemy.emplace_back(file);
        }

        for (int i = 0; i < vox_files_enemy.size(); i++) {
            Model models;
            const double t0 = GetTime() * 1000.0;

            TraceLog(LOG_WARNING, TextFormat("Trying to load file %s...", vox_files_enemy[i].c_str()));
            models = LoadModel(vox_files_enemy[i].c_str());
            const double t1 = GetTime() * 1000.0;
            TraceLog(LOG_WARNING, TextFormat("Loaded file %s in %f ms.", vox_files_enemy[i].c_str(), t1 - t0));

            auto [min, max] = GetModelBoundingBox(models);
            Vector3 center = {};
            center.x = min.x + (max.x - min.x) / 2;
            center.z = min.z + (max.z - min.z) / 2;

            const Matrix matTranslate = MatrixTranslate(-center.x, 0, -center.z);
            models.transform = matTranslate;
            auto ModelEntity = ecs.spawn_entity();
            std::cout << "MODEL ID : " << ModelEntity << std::endl;

            ecs.add_component<ProjectilesComponent>(ModelEntity, {models, false, vox_files_enemy[i],
                {}, false, {}, {}, {}, 0, {}});
        }

        for (int i = 0; i < vox_files_player.size(); i++) {
            Model models;
            const double t0 = GetTime() * 1000.0;

            TraceLog(LOG_WARNING, TextFormat("Trying to load file %s...", vox_files_player[i].c_str()));
            models = LoadModel(vox_files_player[i].c_str());
            const double t1 = GetTime() * 1000.0;
            TraceLog(LOG_WARNING, TextFormat("Loaded file %s in %f ms.", vox_files_player[i].c_str(), t1 - t0));

            auto [min, max] = GetModelBoundingBox(models);
            Vector3 center = {};
            center.x = min.x + (max.x - min.x) / 2;
            center.z = min.z + (max.z - min.z) / 2;

            const Matrix matTranslate = MatrixTranslate(-center.x, 0, -center.z);
            models.transform = matTranslate;

            auto ModelEntity = ecs.spawn_entity();
            std::cout << "MODEL ID : " << ModelEntity << std::endl;

            ecs.add_component<ProjectilesComponent>(ModelEntity, {models, false, vox_files_player[i],
                {}, true, {}, {}, {}, 0, {}});
        }
    }
}
