/*
** EPITECH PROJECT, 2024
** client
** File description:
** Main
*/

#include "core/Game.hpp"
#include "core/Lights.hpp"
#include "Registry.hpp"
#include "systems/Components.hpp"
#include "systems/Events.hpp"
#include "systems/Systems.hpp"

/*
int main()
{

    // Init Raylib window
    client::Game::createRaylibWindow(600, 600, "Voxels Visualizer");
    client::Game::disableRaylibCursor();

    // Init camera
    Camera camera = client::Game::createAndSetCamera((Vector3){ 0.0f, 10.0f, 10.0f },
        (Vector3){ 0.0f, 0.0f, 0.0f }, (Vector3){ 0.0f, 1.0f, 0.0f },
        45.0f, CAMERA_PERSPECTIVE);

    // Load models
    std::vector<Model> models = client::Game::loadModelsFromPath("client/assets/voxels/");

    size_t currentModel = 0;
    const size_t nb_vox = models.size();

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
    std::vector<Color> colors = { WHITE, WHITE, WHITE, WHITE };
    std::vector<std::shared_ptr<client::Light>> lights = client::Game::createLights(shader, positions, colors);

    // Main loop
    client::Game::mainLoopVoxelVisualizer(models, lights, camera, shader);

    // Unload models and close window
    client::Game::unloadModels(models);
    CloseWindow();
    return 0;

}
*/

Registry init_ecs () {
    Registry ecs;

    ecs.register_component<Window>();
    ecs.register_component<ModelComponent>();
    ecs.register_component<ShaderComponent>();
    ecs.register_component<CameraComponent>();

    ecs.register_event<WindowOpenEvent>();
    ecs.register_event<WindowCloseEvent>();
    ecs.register_event<WindowUpdateEvent>();
    ecs.register_event<InitCameraEvent>();
    ecs.register_event<InitModelEvent>();

    ecs.subscribe<WindowOpenEvent>(init_window_system);
    ecs.subscribe<WindowUpdateEvent>(update_window_system);
    ecs.subscribe<WindowCloseEvent>([](Registry &e, const WindowCloseEvent &event) {
        unload_models_system(e, event);
        close_window_system(e, event);
    });

    ecs.subscribe<InitCameraEvent>(create_camera_system);
    ecs.subscribe<InitModelEvent>([](Registry& e, const InitModelEvent& event) {
        load_models_system(e, event);
        apply_shader_system(e, event);
    });

    return ecs;
}

int main() {
    Registry ecs = init_ecs();

    auto windowEntity = ecs.spawn_entity();
    ecs.add_component<Window>(windowEntity, {800, 600, "ECS Raylib - Multi Events", false});

    ecs.run_event(WindowOpenEvent{});

    ecs.run_event(InitCameraEvent{ { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, CAMERA_PERSPECTIVE});
    ecs.run_event(InitModelEvent{});

    while (!WindowShouldClose()) {
        ecs.run_event(WindowUpdateEvent{});
    }
    return 0;
}
