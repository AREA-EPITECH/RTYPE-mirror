/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Display.hpp
*/

#pragma once

// Display Lobby
void update_board_component(Registry &ecs, int screenWidth, int screenHeight);
void display_board (Registry &ecs, int screenWidth, int screenHeight);

void init_lobby_entity (Registry &ecs);

// Display Menu
void display_menu_selectors(Registry &ecs);
void update_menu_selectors(Registry &ecs, int screenWidth, int screenHeight);

void init_menu_entity (Registry &ecs);
