/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Events.hpp
*/

#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <filesystem>
#include <string>
#include <functional>
#include "raylib/raygui.h"

namespace ecs {
    struct CreateWindowEvent {
    };

    struct WindowOpenEvent {
    };

    struct WindowCloseEvent {
        bool shouldClose = false;
    };

    struct WindowDrawEvent {
    };

    struct ControlsEvent {};
}
