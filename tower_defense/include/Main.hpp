/*
** EPITECH PROJECT, 2025
** tower_defense
** File description:
** Main
*/

#pragma once

#include <thread>
#include <atomic>
#include <chrono>
#include <spdlog/spdlog.h>
#include "ecs/Systems.hpp"
#include "ecs/Components.hpp"
#include "initialisation/Parser.hpp"
#include "raylib/raygui.h"
#include "ecs/Components.hpp"

Registry init_ecs();
