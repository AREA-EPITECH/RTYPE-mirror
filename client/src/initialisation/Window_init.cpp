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
    ecs.unsubscribe_all<ecs::InitCameraEvent>();
    ecs.unsubscribe_all<ecs::InitModelEvent>();
    ecs.unsubscribe_all<ecs::InitShaderEvent>();
    ecs.unsubscribe_all<ecs::InitLightEvent>();
    ecs.unsubscribe_all<ecs::InitBackgroundEvent>();
    ecs.unsubscribe_all<ecs::InitDecorElementEvent>();

    ecs.subscribe<ecs::InitBackgroundEvent>(ecs::load_background);
    ecs.subscribe<ecs::InitDecorElementEvent>(ecs::load_decor_element);
    ecs.subscribe<ecs::InitShaderEvent>([](Registry &ecs, const ecs::InitShaderEvent &event) {
        load_shaders(ecs, event);
        set_shader_values(ecs, event);
        apply_shader_system(ecs, event);
    });
    ecs.subscribe<ecs::InitLightEvent>(ecs::create_light_system);
    ecs.subscribe<ecs::ControlsEvent>(ecs::menu_controls_system);
    ecs.subscribe<ecs::WindowOpenEvent>([](Registry &ecs, const ecs::WindowOpenEvent &event) {
        open_menu_system(ecs, event);
    });
    ecs.subscribe<ecs::WindowCloseEvent>([](Registry &e, const ecs::WindowCloseEvent &event) {
        close_menu_system(e, event);
    });
    ecs.subscribe<ecs::WindowDrawEvent>(ecs::draw_menu_system);
    ecs.subscribe<ecs::InitModelEvent>(ecs::load_title_menu);
    ecs.subscribe<ecs::InitCameraEvent>(ecs::create_camera_system);

    ecs.run_event(ecs::ChangeFocusEvent{ecs::MENU_FOCUS});
    init_menu_entity(ecs);
}

/**
 * Init the lobby window
 * @param ecs
 */
void init_lobby_window (Registry& ecs) {
    ecs.unsubscribe_all<ecs::InitCameraEvent>();
    ecs.unsubscribe_all<ecs::InitModelEvent>();
    ecs.unsubscribe_all<ecs::InitShaderEvent>();
    ecs.unsubscribe_all<ecs::InitLightEvent>();
    ecs.unsubscribe_all<ecs::InitBackgroundEvent>();
    ecs.unsubscribe_all<ecs::InitDecorElementEvent>();

    ecs.subscribe<ecs::InitBackgroundEvent>(ecs::load_background);
    ecs.subscribe<ecs::InitDecorElementEvent>(ecs::load_decor_element);
    ecs.subscribe<ecs::InitLightEvent>(ecs::create_light_system);
    ecs.subscribe<ecs::InitCameraEvent>(ecs::create_camera_system);
    ecs.subscribe<ecs::InitShaderEvent>([](Registry &ecs, const ecs::InitShaderEvent &event) {
        load_shader_from_file_system(ecs, event);
        set_shader_values(ecs, event);
        apply_shader_system(ecs, event);
    });
    ecs.subscribe<ecs::InitModelEvent>([](Registry& e, const ecs::InitModelEvent& event) {
        load_models_system(e, event);
    });

    ecs.subscribe<ecs::ControlsEvent>(ecs::lobby_controls_system);
    ecs.subscribe<ecs::WindowOpenEvent>([](Registry &ecs, const ecs::WindowOpenEvent &event) {
        open_lobby_system(ecs, event);
    });
    ecs.subscribe<ecs::WindowCloseEvent>([](Registry &e, const ecs::WindowCloseEvent &event) {
        close_lobby_system(e, event);
    });
    ecs.subscribe<ecs::WindowDrawEvent>(ecs::draw_lobby_system);

    ecs.run_event(ecs::ChangeFocusEvent{ecs::LOBBY_FOCUS});
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
        load_projectiles_models(e, event);
    });

    ecs.subscribe<ecs::WindowOpenEvent>([](Registry &ecs, const ecs::WindowOpenEvent &event) {
        open_game_system(ecs, event);
    });
    ecs.subscribe<ecs::WindowCloseEvent>([](Registry &e, const ecs::WindowCloseEvent &event) {
        close_game_system(e, event);
    });
    ecs.subscribe<ecs::WindowDrawEvent>(ecs::draw_game_system);

    ecs.run_event(ecs::ChangeFocusEvent{ecs::GAME_FOCUS});
}
