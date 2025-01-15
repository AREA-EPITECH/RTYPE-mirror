/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Systems
*/

#pragma once

#include <algorithm>
#include "Registry.hpp"
#include "Events.hpp"
#include "Components.hpp"
#include "DataType.hpp"
#include "Display.hpp"
#include "Main.hpp"

namespace ecs {
// Window
    void init_window_system(Registry &ecs, const CreateWindowEvent &);
    void close_window_system(Registry &ecs, const WindowCloseEvent &);
    void change_window(Registry &ecs, WindowType type);

// Focus
    void change_focus_system(Registry &ecs, const ChangeFocusEvent &event);
    WindowFocus get_focus(Registry &ecs);

// Lobby
    void draw_lobby_system(Registry &ecs, const WindowDrawEvent &);
    void close_lobby_system(Registry &ecs, const WindowCloseEvent &);
    void open_lobby_system(Registry &ecs, const WindowOpenEvent &);

// Menu
    void draw_menu_system(Registry &ecs, const WindowDrawEvent &);
    void open_menu_system(Registry &ecs, const WindowOpenEvent &);
    void close_menu_system(Registry &ecs, const WindowCloseEvent &);

// Game
    void draw_game_system(Registry &ecs, const WindowDrawEvent &);
    void close_game_system(Registry &ecs, const WindowCloseEvent &);
    void open_game_system(Registry &ecs, const WindowOpenEvent &);

// EndGame
    void draw_endgame_system(Registry &ecs, const WindowDrawEvent &);
    void close_endgame_system(Registry &ecs, const WindowCloseEvent &);
    void open_endgame_system(Registry &ecs, const WindowOpenEvent &);

// Camera
    void create_camera_system(Registry &ecs, const InitCameraEvent &);

// Models
    void load_models_system(Registry &ecs, const InitModelEvent &);
    void load_model_from_file_system(Registry &ecs, const InitModelEvent &event);
    void load_vessels_for_game(Registry &ecs);
    void load_title_menu(Registry &ecs, const InitModelEvent &);
    void load_projectiles_models(Registry &ecs, const InitModelEvent &);

// Shaders
    void load_shaders(Registry &ecs, const InitShaderEvent &event);
    void load_shader_from_file_system(Registry &ecs, const InitShaderEvent &);
    void set_shader_values(Registry &ecs, const InitShaderEvent &);
    void apply_shader_system(Registry &ecs, const InitShaderEvent &);

// Controls
    void lobby_controls_system(Registry &ecs, const ControlsEvent &);
    void menu_controls_system(Registry &ecs, const ControlsEvent &);
    void game_controls_system(Registry &ecs, const ControlsEvent &);
    void create_player_basic_projectile(Registry &ecs, uint32_t id, Model model, Vector3 position, Vector3 velocity, bool player,
                                        std::string &path, Vector3 target, int nb, Shader shader);

// Particles
    void particles_system(Registry &ecs, const ParticleSystemEvent &);

// Lights
    void create_light_system(Registry &ecs, const InitLightEvent &);

// Background
    void load_background(Registry &ecs, const InitBackgroundEvent &event);

// DecorElement
    void load_decor_element(Registry &ecs, const InitDecorElementEvent &event);

// Settings
    void display_settings_system (Registry &ecs, const DisplaySettingEvent &);
    std::string getPrintableKeyName(int key);

// HealthBar
    void create_health_bar_system(Registry &ecs, const HealthBarEvent &event);
}
