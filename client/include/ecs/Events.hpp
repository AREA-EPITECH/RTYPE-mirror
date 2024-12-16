/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Events.hpp
*/

#pragma once

#include "iostream"
#include <utility>
#include <vector>
#include "core/Lights.hpp"
#include <filesystem>
#include <string>
#include "raymath.h"
#include "core/ParticleSystem.hpp"
#include <functional>
#include "raylib/raygui.h"

namespace ecs {
// Window
    struct CreateWindowEvent {

    };

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
        Color color;
    };

// Background
    struct InitBackgroundEvent {
        std::string path;
        size_t depth;
        float speed;
        float offset;
    };

// Decor element
    struct InitDecorElementEvent
    {
        std::string path;
        int speed;
    };
}