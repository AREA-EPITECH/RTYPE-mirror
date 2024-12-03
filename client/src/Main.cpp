/*
** EPITECH PROJECT, 2024
** client
** File description:
** Main
*/

#include "core/Lights.hpp"
#include "Registry.hpp"
#include "ecs/Components.hpp"
#include "ecs/Events.hpp"
#include "ecs/Systems.hpp"

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

    ecs.subscribe<ecs::WindowOpenEvent>(ecs::init_window_system);

    ecs.subscribe<ecs::WindowDrawEvent>(ecs::draw_menu_system);
    ecs.subscribe<ecs::WindowCloseEvent>(ecs::close_window_system);

    ecs.subscribe<ecs::InitCameraEvent>(ecs::create_camera_system);
    ecs.subscribe<ecs::InitModelEvent>([](Registry& e, const ecs::InitModelEvent& event) {
        load_models_system(e, event);
        apply_shader_system(e, event);
    });

    ecs.subscribe<ecs::ControlsEvent>(ecs::menu_controls_system);

    return ecs;
}

int main() {
    Registry ecs = init_ecs();

    auto windowEntity = ecs.spawn_entity();
    ecs.add_component<ecs::Window>(windowEntity, {1920, 1080, "ECS Raylib - Multi Events", false});

    ecs.run_event(ecs::WindowOpenEvent{});

    while (!WindowShouldClose()) {
        ecs.run_event(ecs::WindowDrawEvent{});
    }

    return 0;
}
