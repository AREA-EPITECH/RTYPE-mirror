/*
** EPITECH PROJECT, 2024
** client
** File description:
** Main
*/

#pragma once

#if defined(_WIN32)
	#define NOGDI // All GDI defines and routines
	#define NOUSER // All USER defines and routines
#endif

#include "raylib/raygui.h"
#include "ecs/Systems.hpp"
#include "network/Client.hpp"
#include "network/packet/Packet.hpp"
#include <thread>
#include <atomic>
#include <chrono>
#include <spdlog/spdlog.h>

void init_menu_window (Registry& ecs);
void init_lobby_window (Registry& ecs);
void init_game_window (Registry& ecs);
void init_end_game_window(Registry& ecs);

Registry init_ecs();

void previous_ship(Registry &ecs);
void next_ship(Registry &ecs);

void updateSettings(Registry &ecs, const std::string &jsonFilePath, int s_width, int _s_height);