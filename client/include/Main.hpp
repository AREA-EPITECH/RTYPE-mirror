/*
** EPITECH PROJECT, 2024
** client
** File description:
** Main
*/

#pragma once

#include "network/Client.hpp"
#include "network/packet/Packet.hpp"
#include <thread>
#include <atomic>
#include <chrono>
#include <spdlog/spdlog.h>
#include "ecs/Systems.hpp"

void init_menu_window (Registry& ecs);
void init_lobby_window (Registry& ecs);
void init_game_window (Registry& ecs);

Registry init_ecs();
void init_settings (Registry &ecs);


