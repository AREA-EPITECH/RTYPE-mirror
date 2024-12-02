#pragma once

#include "Registry.hpp"
#include "Events.hpp"
#include "Components.hpp"

// Window
void init_window_system(Registry& ecs, const WindowOpenEvent&);
void close_window_system(Registry& ecs, const WindowCloseEvent&);

// Menu
void draw_menu_system(Registry& ecs, const WindowDrawEvent&);

// Camera
void create_camera_system(Registry& ecs, const InitCameraEvent&);

// Models
void load_models_system(Registry& ecs, const InitModelEvent&);
void apply_shader_system(Registry& ecs, const InitModelEvent&);
void unload_models_system(Registry& ecs, const WindowCloseEvent&);

// Controls
void selector_controls_system(Registry &e, const ControlsEvent&);
void menu_controls_system(Registry &e, const ControlsEvent &);
void game_controls_system(Registry &e, const ControlsEvent&);