//
// Created by lferraro on 12/2/24.
//

#include "ecs/Systems.hpp"

namespace ecs {
    void load_models_system(Registry &ecs, const InitModelEvent &) {
        std::vector<std::string> vox_files;

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
            ecs.add_component<ModelComponent>(ModelEntity, {models, (i == 0)});
        }
    }

    void ApplyShaderSystem(Registry &ecs, const ShaderComponent &event) {
        auto &models = ecs.get_components<ModelComponent>();

        for (size_t i = 0; i < models.size(); ++i) {
            if (models[i]) {
                auto &model = models[i]->model;
                for (int j = 0; j < model.materialCount; ++j) {
                    model.materials[j].shader = event.shader;
                }
            }
        }
    }

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

    void apply_shader_system(Registry &ecs, const InitModelEvent &) {
        auto &models = ecs.get_components<ModelComponent>();

        Shader shader = LoadShader("client/assets/voxels/shaders/voxel_lighting.vs",
                                   "client/assets/voxels/shaders/voxel_lighting.fs");
        shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
        int ambientLoc = GetShaderLocation(shader, "ambient");
        SetShaderValue(shader, ambientLoc, (float[4]) {0.1f, 0.1f, 0.1f, 1.0f}, SHADER_UNIFORM_VEC4);

        for (std::size_t i = 0; i < models.size(); ++i) {
            if (models[i].has_value()) {
                Model &model = models[i]->model;

                for (int j = 0; j < model.materialCount; ++j) {
                    model.materials[j].shader = shader;
                }

                TraceLog(LOG_INFO, TextFormat("Applied shader to model of entity %zu.", i));
            }
        }
    }
}