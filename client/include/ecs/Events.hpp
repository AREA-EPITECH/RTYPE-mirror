#pragma once

#include "iostream"
#include <utility>
#include <vector>
#include <filesystem>
#include "raylib.h"
#include <string>
#include "raymath.h"
#include "core/ParticleSystem.hpp"
#include "core/Lights.hpp"
#include <functional>

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

// Lights
    struct InitLightEvent {
        client::LightType type;
        Vector3 position;
        Vector3 target;
        Color color;
        Shader shader;
        int nb;
    };

// Models
    struct InitModelEvent {};

// Shaders
    struct InitShaderEvent
    {
        std::string vs_file;
        std::string fs_file;
    };

// Controls
    struct ControlsEvent {};

// ParticleSystem
    struct ParticleSystemEvent {
        Vector3 position;
        client::Direction direction;
        bool infinite;
        float lifetime;
        size_t max_particles;
    };
}