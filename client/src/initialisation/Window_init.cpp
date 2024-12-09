/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Window_init
*/


#include "Registry.hpp"
#include "ecs/Systems.hpp"

/**
 * Init the menu window
 * @param ecs
 */
void init_menu_window (Registry& ecs) {
    ecs.subscribe<ecs::ControlsEvent>(ecs::menu_controls_system);
    ecs.subscribe<ecs::WindowOpenEvent>(ecs::init_window_system);
    ecs.subscribe<ecs::WindowCloseEvent>(ecs::close_window_system);
    ecs.subscribe<ecs::WindowDrawEvent>(ecs::draw_menu_system);
}

/**
 * Init the lobby window
 * @param ecs
 */
void init_lobby_window (Registry& ecs) {

    ecs.unsubscribe_all<ecs::InitCameraEvent>();
    ecs.unsubscribe_all<ecs::InitModelEvent>();
    ecs.unsubscribe_all<ecs::InitShaderEvent>();

    ecs.subscribe<ecs::InitCameraEvent>(ecs::create_camera_system);
    ecs.subscribe<ecs::InitShaderEvent>([](Registry &ecs, const ecs::InitShaderEvent &event) {
        load_shaders(ecs, event);
        set_shader_values(ecs, event);
        apply_shader_system(ecs, event);
    });
    ecs.subscribe<ecs::InitModelEvent>([](Registry& e, const ecs::InitModelEvent& event) {
        load_models_system(e, event);
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

    init_lobby_entity(ecs);
}

/**
 * Init the game window
 * @param ecs
 */
void init_game_window (Registry& ecs) {
    ecs.unsubscribe_all<ecs::InitCameraEvent>();
    ecs.unsubscribe_all<ecs::InitModelEvent>();
    ecs.unsubscribe_all<ecs::InitLightEvent>();
    ecs.unsubscribe_all<ecs::InitShaderEvent>();
    ecs.unsubscribe_all<ecs::InitBackgroundEvent>();
    ecs.unsubscribe_all<ecs::InitDecorElementEvent>();

    ecs.subscribe<ecs::InitBackgroundEvent>(ecs::load_background);
    ecs.subscribe<ecs::InitDecorElementEvent>(ecs::load_decor_element);
    ecs.subscribe<ecs::InitLightEvent>(ecs::create_light_system);
    ecs.subscribe<ecs::ParticleSystemEvent>(ecs::particles_system);
    ecs.subscribe<ecs::ControlsEvent>(ecs::game_controls_system);
    ecs.subscribe<ecs::InitCameraEvent>(ecs::create_camera_system);
    ecs.subscribe<ecs::InitShaderEvent>([](Registry &ecs, const ecs::InitShaderEvent &event) {
        load_shader_from_file_system(ecs, event);
        set_shader_values(ecs, event);
        apply_shader_system(ecs, event);
    });
    ecs.subscribe<ecs::InitModelEvent>([](Registry& e, const ecs::InitModelEvent& event) {
        load_model_from_file_system(e, event);
    });

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
