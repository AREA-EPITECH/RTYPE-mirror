/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Window_init
*/


#include "Registry.hpp"
#include "ecs/Systems.hpp"

void init_menu_window (Registry& ecs) {
    ecs.subscribe<ecs::ControlsEvent>(ecs::menu_controls_system);
    ecs.subscribe<ecs::WindowOpenEvent>(ecs::init_window_system);
    ecs.subscribe<ecs::WindowCloseEvent>(ecs::close_window_system);
    ecs.subscribe<ecs::WindowDrawEvent>(ecs::draw_menu_system);
}

void init_lobby_window (Registry& ecs) {

    ecs.unsubscribe_all<ecs::InitCameraEvent>();
    ecs.unsubscribe_all<ecs::InitModelEvent>();

    ecs.subscribe<ecs::InitCameraEvent>(ecs::create_camera_system);
    ecs.subscribe<ecs::InitModelEvent>([](Registry& e, const ecs::InitModelEvent& event) {
        load_models_system(e, event);
        apply_shader_system(e, event);
        load_lights(e, event);
    });

    ecs.subscribe<ecs::ControlsEvent>(ecs::lobby_controls_system);
    ecs.subscribe<ecs::WindowOpenEvent>([](Registry &ecs, const ecs::WindowOpenEvent &event) {
        init_window_system(ecs, event);
        open_lobby_system(ecs, event);
    });
    ecs.subscribe<ecs::WindowCloseEvent>([](Registry &e, const ecs::WindowCloseEvent &event) {
        close_lobby_system(e, event);
        close_window_system(e, event);
    });
    ecs.subscribe<ecs::WindowDrawEvent>(ecs::draw_lobby_system);
}

void init_game_window (Registry& ecs) {
    ecs.subscribe<ecs::InitLightEvent>(ecs::create_light_system);
    ecs.subscribe<ecs::ParticleSystemEvent>(ecs::particles_system);
    ecs.subscribe<ecs::ControlsEvent>(ecs::game_controls_system);
    ecs.subscribe<ecs::WindowOpenEvent>([](Registry &ecs, const ecs::WindowOpenEvent &event) {
        init_window_system(ecs, event);
        open_game_system(ecs, event);
    });
    ecs.subscribe<ecs::WindowCloseEvent>([](Registry &e, const ecs::WindowCloseEvent &event) {
        close_game_system(e, event);
        close_window_system(e, event);
    });
    ecs.subscribe<ecs::WindowDrawEvent>(ecs::draw_game_system);
}