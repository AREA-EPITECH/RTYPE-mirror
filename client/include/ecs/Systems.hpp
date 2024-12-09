/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Systems
*/

#pragma once

#include "Registry.hpp"
#include "Events.hpp"
#include "Components.hpp"
#include "DataType.hpp"
#include "Display.hpp"
#include "Main.hpp"

namespace ecs {
// Window
    void init_window_system(Registry &ecs, const WindowOpenEvent &);
    void close_window_system(Registry &ecs, const WindowCloseEvent &);
    void change_window(Registry &ecs, WindowType type);

// Lobby
    void draw_lobby_system(Registry &ecs, const WindowDrawEvent &);
    void close_lobby_system(Registry &ecs, const WindowCloseEvent &);
    void open_lobby_system(Registry &ecs, const WindowOpenEvent &);

// Menu
    void draw_menu_system(Registry &ecs, const WindowDrawEvent &);

// Game
    void draw_game_system(Registry &ecs, const WindowDrawEvent &);
    void close_game_system(Registry &ecs, const WindowCloseEvent &);
    void open_game_system(Registry &ecs, const WindowOpenEvent &);

// Camera
    void create_camera_system(Registry &ecs, const InitCameraEvent &);

// Models
    void load_models_system(Registry &ecs, const InitModelEvent &);
    void load_model_from_file_system(Registry &ecs, const InitModelEvent &event);

// Shaders
    void load_shaders(Registry &ecs, const InitShaderEvent &event);
    void load_shader_from_file_system(Registry &ecs, const InitShaderEvent &);
    void set_shader_values(Registry &ecs, const InitShaderEvent &);
    void apply_shader_system(Registry &ecs, const InitShaderEvent &);

// Controls
    void lobby_controls_system(Registry &ecs, const ControlsEvent &);
    void menu_controls_system(Registry &ecs, const ControlsEvent &);
    void game_controls_system(Registry &ecs, const ControlsEvent &);

// Particles
    void particles_system(Registry &ecs, const ParticleSystemEvent &);

// Lights
    void create_light_system(Registry &ecs, const InitLightEvent &);
}
