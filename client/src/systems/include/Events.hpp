#pragma once

#include "iostream"
#include <vector>
#include <filesystem>
#include "raylib.h"
#include <string>
#include "raymath.h"

namespace ecs {
// Window
    struct WindowOpenEvent {
    };

    struct WindowCloseEvent {
        bool shouldClose = false;
    };

    struct WindowDrawEvent {
    };

// Camera
    struct InitCameraEvent {
        Vector3 position;
        Vector3 target;
        Vector3 up;
        float fovy;
        int projection;
    };


// Models
    struct InitModelEvent {};

// Controls
    struct ControlsEvent {};
}