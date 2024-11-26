/*
** EPITECH PROJECT, 2024
** client
** File description:
** Game
*/

#pragma once

#include "raylib.h"
#include <string>
#include "raymath.h"
#include <vector>
#include <filesystem>
#include "core/Lights.hpp"

namespace client
{
    class Game
    {
    public:
        // Window init
        static void createRaylibWindow(int width, int height, const std::string &title);
        static void enableRaylibCursor();
        static void disableRaylibCursor();

        // Camera
        static Camera createAndSetCamera(Vector3 position, Vector3 target, Vector3 up, float fovy, int projection);

        // Models & shaders
        static std::vector<Model> loadModelsFromPath(const std::string &path);
        static void applyShaderOnModels(Shader shader, const std::vector<Model> &models);
        static void unloadModels(const std::vector<Model> &models);

        // Lights
        static std::vector<std::shared_ptr<Light>> createLights(Shader &shader, std::vector<Vector3> &positions,
            std::vector<Color> &colors);

        // Main loop to visualize voxels
        static void mainLoopVoxelVisualizer(std::vector<Model> &models,
            const std::vector<std::shared_ptr<Light>> &lights, Camera &camera, Shader &shader);
    };
}
