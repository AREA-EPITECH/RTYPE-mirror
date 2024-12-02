#pragma once

#include "Registry.hpp"
#include "Events.hpp"
#include "Components.hpp"

// Window
void init_window_system(Registry& ecs, const WindowOpenEvent&);
void close_window_system(Registry& ecs, const WindowCloseEvent&);
void update_window_system(Registry& ecs, const WindowUpdateEvent&);

void create_camera_system(Registry& ecs, const InitCameraEvent&);
void load_models_system(Registry& ecs, const InitModelEvent&);
void apply_shader_system(Registry& ecs, const InitModelEvent&);
void unload_models_system(Registry& ecs, const WindowCloseEvent&);