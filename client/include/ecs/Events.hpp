/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Events.hpp
*/

#pragma once

#include "raylib.h"
#include "raylib/raygui.h"
#include "raymath.h"
#include "iostream"
#include "core/ParticleSystem.hpp"
#include "core/Lights.hpp"
#include "DataType.hpp"
#include <utility>
#include <vector>
#include <filesystem>
#include <string>
#include <functional>

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
        size_t depth;
        int speed;
    };

    // Change focus
    struct ChangeFocusEvent {
        WindowFocus newFocus;
    };

    //Init menu
    struct InitMenuElement {};

    // Settings
    struct DisplaySettingEvent {};

// Health bar
    struct HealthBarEvent
    {
        std::string path;
    };
}
