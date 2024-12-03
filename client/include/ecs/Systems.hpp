#pragma once

#include "Registry.hpp"
#include "Events.hpp"
#include "Components.hpp"
#include "DataType.hpp"
#include "Main.hpp"

namespace ecs {
// Window
    void init_window_system(Registry &ecs, const WindowOpenEvent &);
    void close_window_system(Registry &ecs, const WindowCloseEvent &);

// Lobby
    void draw_lobby_system(Registry &ecs, const WindowDrawEvent &);
    void close_lobby_system(Registry &ecs, const WindowCloseEvent &);
    void open_lobby_system(Registry &ecs, const WindowOpenEvent &);

// Menu
    void draw_menu_system(Registry &ecs, const WindowDrawEvent &);

// Game
    void draw_game_system(Registry &ecs, const WindowDrawEvent &);

// Camera
    void create_camera_system(Registry &ecs, const InitCameraEvent &);

// Models
    void load_models_system(Registry &ecs, const InitModelEvent &);
    void apply_shader_system(Registry &ecs, const InitModelEvent &);

// Controls
    void lobby_controls_system(Registry &ecs, const ControlsEvent &);
    void menu_controls_system(Registry &ecs, const ControlsEvent &);
    void game_controls_system(Registry &ecs, const ControlsEvent &);
}