/*
** EPITECH PROJECT, 2024
** client
** File description:
** Main
*/

#include "Main.hpp"

/**
 * Init the ecs
 * @return
 */
Registry init_ecs () {
    Registry ecs;

    ecs.register_component<ecs::Window>();
    ecs.register_component<ecs::VesselsComponent>();
    ecs.register_component<ecs::ShaderComponent>();
    ecs.register_component<ecs::CameraComponent>();
    ecs.register_component<ecs::ParticleSystemComponent>();
    ecs.register_component<ecs::LightComponent>();
    ecs.register_component<ecs::TextComponent>();
    ecs.register_component<ecs::ButtonComponent>();

    ecs.register_event<ecs::WindowOpenEvent>();
    ecs.register_event<ecs::WindowCloseEvent>();
    ecs.register_event<ecs::WindowDrawEvent>();
    ecs.register_event<ecs::InitCameraEvent>();
    ecs.register_event<ecs::InitModelEvent>();
    ecs.register_event<ecs::ControlsEvent>();
    ecs.register_event<ecs::ParticleSystemEvent>();
    ecs.register_event<ecs::InitLightEvent>();
    ecs.register_event<ecs::InitShaderEvent>();

    init_menu_window(ecs);

    return ecs;
}

int main() {
    Registry ecs = init_ecs();

    auto windowEntity = ecs.spawn_entity();
    ecs.add_component<ecs::Window>(windowEntity, {1920, 1080, "ECS Raylib - Multi Events",
        false});

    ecs.subscribe<ecs::InitLightEvent>(ecs::create_light_system);

    ecs.run_event(ecs::WindowOpenEvent{});

    while (!WindowShouldClose()) {
        ecs.run_event(ecs::WindowDrawEvent{});
    }

    return 0;
}
