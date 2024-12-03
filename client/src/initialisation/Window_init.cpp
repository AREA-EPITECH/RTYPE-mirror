//
// Created by lferraro on 12/3/24.
//

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
    ecs.subscribe<ecs::ControlsEvent>(ecs::game_controls_system);
    ecs.subscribe<ecs::WindowOpenEvent>(ecs::init_window_system);
    ecs.subscribe<ecs::WindowCloseEvent>(ecs::close_window_system);
    ecs.subscribe<ecs::WindowDrawEvent>(ecs::draw_game_system);
}