/*
** EPITECH PROJECT, 2024
** client
** File description:
** Main
*/

#include "core/Lights.hpp"
#include "Registry.hpp"
#include "systems/include/Components.hpp"
#include "systems/include/Events.hpp"
#include "systems/include/Systems.hpp"

Registry init_ecs () {
    Registry ecs;

    ecs.register_component<ecs::Window>();
    ecs.register_component<ecs::ModelComponent>();
    ecs.register_component<ecs::ShaderComponent>();
    ecs.register_component<ecs::CameraComponent>();

    ecs.register_event<ecs::WindowOpenEvent>();
    ecs.register_event<ecs::WindowCloseEvent>();
    ecs.register_event<ecs::WindowDrawEvent>();
    ecs.register_event<ecs::InitCameraEvent>();
    ecs.register_event<ecs::InitModelEvent>();
    ecs.register_event<ecs::ControlsEvent>();
    
    ecs.subscribe<ecs::WindowOpenEvent>([](Registry &ecs, const ecs::WindowOpenEvent &event) {
        init_window_system(ecs, event);
        open_selector_system(ecs, event);
    });
    ecs.subscribe<ecs::WindowDrawEvent>(ecs::draw_selector_system);
    ecs.subscribe<ecs::WindowCloseEvent>([](Registry &e, const ecs::WindowCloseEvent &event) {
        close_selector_system(e, event);
        close_window_system(e, event);
    });

    ecs.subscribe<ecs::InitCameraEvent>(ecs::create_camera_system);
    ecs.subscribe<ecs::InitModelEvent>([](Registry& e, const ecs::InitModelEvent& event) {
        load_models_system(e, event);
        apply_shader_system(e, event);
    });
    ecs.subscribe<ecs::ControlsEvent>(ecs::selector_controls_system);

    return ecs;
}

int main() {
    Registry ecs = init_ecs();

    auto windowEntity = ecs.spawn_entity();
    ecs.add_component<ecs::Window>(windowEntity, {800, 600, "ECS Raylib - Multi Events", false});

    ecs.run_event(ecs::WindowOpenEvent{});

    while (!WindowShouldClose()) {
        ecs.run_event(ecs::WindowDrawEvent{});
    }

    return 0;
}
