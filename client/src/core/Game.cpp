/*
** EPITECH PROJECT, 2024
** client
** File description:
** Game
*/

#include "core/Game.hpp"
#include <algorithm>
#include "core/ParticleSystem.hpp"

namespace client
{
    void Game::createRaylibWindow(const int width, const int height, const std::string &title)
    {
        TraceLog(LOG_WARNING, TextFormat("Initiating window %dx%d named %s.", width, height, title));
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(width, height, title.c_str());
        SetTargetFPS(144);
    }

    void Game::enableRaylibCursor()
    {
        TraceLog(LOG_INFO, "Enabling cursor.");
        EnableCursor();
    }

    void Game::disableRaylibCursor()
    {
        TraceLog(LOG_WARNING, "Disabling cursor.");
        DisableCursor();
    }

    Camera Game::createAndSetCamera(const Vector3 position, const Vector3 target, const Vector3 up, const float fovy,
                                    const int projection)
    {
        TraceLog(LOG_INFO, TextFormat("Creating camera with following properties: position (%d, %d, %d), "
                                         "target (%d %d %d), up (%d, %d, %d), fovy %f.", position.x, position.y,
                                         position.z, target.x, target.y, target.z, up.x, up.y, up.z, fovy));
        Camera camera = {};
        camera.position = position;
        camera.target = target;
        camera.up = up;
        camera.fovy = fovy;
        camera.projection = projection;
        return camera;
    }

    std::vector<Model> Game::loadModelsFromPath(const std::string &path)
    {
        std::vector<std::string> vox_files;

        // Iterate over the directory
        for (const auto &entry : std::filesystem::directory_iterator(path))
        {
            // If the file is a .vox file, add it to the vector
            if (std::string file = entry.path().c_str(); file.find(".vox") != std::string::npos)
                vox_files.emplace_back(file);
        }

        const size_t nb_vox = vox_files.size();
        std::vector<Model> models;
        models.resize(nb_vox);

        for (int i = 0; i < nb_vox; i++)
        {
            const double t0 = GetTime() * 1000.0;

            // Load the model
            models[i] = LoadModel(vox_files[i].c_str());
            const double t1 = GetTime() * 1000.0;
            TraceLog(LOG_WARNING, TextFormat("Loaded file %s in %f ms.", vox_files[i].c_str(), t1 - t0));

            auto [min, max] = GetModelBoundingBox(models[i]);
            Vector3 center = {};
            center.x = min.x + (max.x - min.x) / 2;
            center.z = min.z + (max.z - min.z) / 2;

            // Translate the model to the center
            const Matrix matTranslate = MatrixTranslate(-center.x, 0, -center.z);
            models[i].transform = matTranslate;
        }

        return models;
    }

    void Game::applyShaderOnModels(const Shader shader, const std::vector<Model> &models)
    {
        for (auto model : models)
        {
            for (int j = 0; j < model.materialCount; j++)
            {
                model.materials[j].shader = shader;
            }
        }
    }

    void Game::unloadModels(const std::vector<Model> &models)
    {
        for (int i = 0; i < models.size(); i++)
        {
            UnloadModel(models[i]);
            TraceLog(LOG_WARNING, TextFormat("Unloaded model number %d.", i));
        }
    }

    std::vector<std::shared_ptr<Light>> Game::createLights(Shader &shader, std::vector<Vector3> &positions,
                                                           std::vector<Color> &colors)
    {
        std::vector<std::shared_ptr<client::Light>> lights = {};

        if (positions.size() != colors.size())
        {
            TraceLog(LOG_WARNING, "The number of positions and colors must be the same.");
            return lights;
        }
        for (int i = 0; i < positions.size(); i++)
        {
            lights.emplace_back(std::make_shared<Light>(LIGHT_POINT, positions[i], Vector3Zero(), colors[i], shader, i));
        }
        return lights;
    }


    void Game::mainLoopVoxelVisualizer(std::vector<Model> &models,
        const std::vector<std::shared_ptr<Light>> &lights, Camera &camera, Shader &shader)
    {
        Vector3 modelpos = {};
        Vector3 camerarot = {};
        size_t currentModel = 0;
        size_t nb_vox = models.size();
        bool rotating = false;
        float rotationAngle = 0.01f;

        std::vector<ParticleSystem> particle_systems;

        while (!WindowShouldClose())
            {
            UpdateCamera(&camera, CAMERA_FIRST_PERSON);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                particle_systems.emplace_back(Vector3{GetMousePosition().x, GetMousePosition().y, 0}, UP, true, 100, 500);
                TraceLog(LOG_WARNING, "Nb of particle systems: %d", particle_systems.size());
            }
            if (IsKeyDown(KEY_LEFT_SHIFT))
            {
                camera.position.y -= 0.1f;
            }
            if (IsKeyDown(KEY_SPACE))
            {
                camera.position.y += 0.1f;
            }

            if (IsKeyDown(KEY_UP))
            {
                rotationAngle += 0.01f;
            }
            if (IsKeyDown(KEY_DOWN))
            {
                rotationAngle -= 0.01f;
            }
            if (IsKeyPressed(KEY_P))
            {
                rotating = !rotating;
            }
            Matrix rotation = MatrixRotate((Vector3){ 0.0f, 1.0f, 0.0f }, rotationAngle);
            if (rotating)
                models[currentModel].transform = MatrixMultiply(models[currentModel].transform, rotation);

            if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
            {
                const Vector2 mouseDelta = GetMouseDelta();
                camerarot.x = mouseDelta.x * 0.05f;
                camerarot.y = mouseDelta.y * 0.05f;
            }

            UpdateCameraPro(&camera,
                (Vector3) {
                (IsKeyDown(KEY_W)) * 0.1f -
                    (IsKeyDown(KEY_S)) * 0.1f,
                    (IsKeyDown(KEY_D)) * 0.1f -
                    (IsKeyDown(KEY_A)) * 0.1f,
                    0.0f
            },
                camerarot,
                GetMouseWheelMove() * -2.0f);

            if (IsKeyPressed(KEY_RIGHT))
            {
                currentModel = (currentModel + 1) % nb_vox;
            }

            if (IsKeyPressed(KEY_LEFT))
            {
                if (currentModel != 0)
                {
                    currentModel = (currentModel - 1) % nb_vox;
                }
                else
                {
                    currentModel = nb_vox - 1;
                }
            }
            float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
            SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
            for (int i = 0; i < 4; i++)
            {
                lights[i].get()->UpdateLightValues(shader);
            }
            BeginDrawing();
            ClearBackground(RAYWHITE);
            for (auto it = particle_systems.begin(); it != particle_systems.end();)
            {
                if (!it->isAlive())
                {
                    // Delete the particle system if it is not alive
                    it = particle_systems.erase(it);
                }
                else
                {
                    // Update and draw the particle system
                    it->update();
                    it->draw();
                    ++it;
                 }
            }
            BeginMode3D(camera);
            DrawModel(models[currentModel], modelpos, 1.0f, WHITE);
            DrawGrid(10, 1.0);
            for (int i = 0; i < 4; i++)
            {
                if (lights[i].get()->_enabled)
                {
                    DrawSphereEx(lights[i].get()->_position, 0.2f, 8, 8, lights[i].get()->_color);
                }
                else
                {
                    DrawSphereWires(lights[i].get()->_position, 0.2f, 8, 8,
                        ColorAlpha(lights[i].get()->_color, 0.3f));
                }
            }
            EndMode3D();
            EndDrawing();
        }
    }
}
