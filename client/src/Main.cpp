/*
** EPITECH PROJECT, 2024
** client
** File description:
** Main
*/

#include "core/Game.hpp"
#include "core/Lights.hpp"

int main()
{
    // Init Raylib window
    client::Game::createRaylibWindow(1080, 1080, "Voxels Visualizer");

    // Init camera
    Camera camera = client::Game::createAndSetCamera((Vector3){ 0.0f, 10.0f, 10.0f },
        (Vector3){ 0.0f, 0.0f, 0.0f }, (Vector3){ 0.0f, 1.0f, 0.0f },
        45.0f, CAMERA_PERSPECTIVE);

    // Load models
    std::vector<Model> models = client::Game::loadModelsFromPath("client/assets/voxels/");

    // Load shader
    Shader shader = LoadShader("client/assets/voxels/shaders/voxel_lighting.vs",
        "client/assets/voxels/shaders/voxel_lighting.fs");
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]) { 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);

    // Apply shader
    client::Game::applyShaderOnModels(shader, models);

    // Creating lights
    std::vector<Vector3> positions = { { -20, 20, -20 }, { 20, -20, 20 },
        { -20, 20, 20 }, { 20, -20, -20 } };
    std::vector colors = {WHITE, WHITE, WHITE, WHITE};
    const std::vector<std::shared_ptr<client::Light>> lights = client::Game::createLights(shader, positions, colors);

    // Main loop
    client::Game::mainLoopVoxelVisualizer(models, lights, camera, shader);

    // Unload models and close window
    client::Game::unloadModels(models);
    CloseWindow();
    return 0;
}
