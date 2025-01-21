/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** DataType.hpp
*/

#pragma once

#include <map>

namespace ecs
{
    enum WindowType
    {
        MENU,
        LOBBY,
        GAME,
        END_GAME,
    };

    enum WindowFocus
    {
        DEFAULT_FOCUS,

        // MENU
        MENU_FOCUS,
        JOIN_ROOM_FOCUS,
        SETTINGS_FOCUS,
        POPUP_FOCUS,

        // LOBBY
        LOBBY_FOCUS,

        // GAME
        GAME_FOCUS,

        // END_GAME
        END_GAME_FOCUS
    };

#define ASSETS_PATH "client/assets/"
#define ASSET_FILE(file) ASSETS_PATH file


    enum ButtonStyle
    {
        // Normal
        BUTTON_BASE_COLOR_NORMAL = 0,
        BUTTON_TEXT_COLOR_NORMAL,
        BUTTON_BORDER_COLOR_NORMAL,

        // Focused
        BUTTON_BASE_COLOR_FOCUSED,
        BUTTON_TEXT_COLOR_FOCUSED,
        BUTTON_BORDER_COLOR_FOCUSED
    };

    const std::map<ButtonStyle, Color> ButtonStyleColors = {
        // Normal
        {BUTTON_BASE_COLOR_NORMAL, {148, 113, 150, 200}},
        {BUTTON_TEXT_COLOR_NORMAL, {0, 0, 0, 255}},
        {BUTTON_BORDER_COLOR_NORMAL, {16, 7, 16, 200}},

        // Focused & pressed
        {BUTTON_BASE_COLOR_FOCUSED, {195, 122, 164, 200}},
        {BUTTON_TEXT_COLOR_FOCUSED, {0, 0, 0, 255}},
        {BUTTON_BORDER_COLOR_FOCUSED, {16, 7, 16, 200}}};

    enum InputStyle
    {
        // Normal
        INPUT_BASE_COLOR_NORMAL = 0,
        INPUT_TEXT_COLOR_NORMAL,
        INPUT_BORDER_COLOR_NORMAL,

        // Focused
        INPUT_BASE_COLOR_FOCUSED,
        INPUT_TEXT_COLOR_FOCUSED,
        INPUT_BORDER_COLOR_FOCUSED,

        // Focused
        INPUT_BASE_COLOR_PRESSED,
        INPUT_TEXT_COLOR_PRESSED,
        INPUT_BORDER_COLOR_PRESSED

    };

    const std::map<InputStyle, Color> InputStyleColors = {
        // Normal
        {INPUT_BASE_COLOR_NORMAL, {148, 113, 150, 200}},
        {INPUT_TEXT_COLOR_NORMAL, {255, 255, 255, 255}},
        {INPUT_BORDER_COLOR_NORMAL, {16, 7, 16, 200}},

        // Focused
        {INPUT_BASE_COLOR_FOCUSED, {195, 122, 164, 200}},
        {INPUT_TEXT_COLOR_FOCUSED, {255, 255, 255, 255}},
        {INPUT_BORDER_COLOR_FOCUSED, {16, 7, 16, 200}},

        // Focused
        {INPUT_BASE_COLOR_PRESSED, {195, 122, 164, 200}},
        {INPUT_TEXT_COLOR_PRESSED, {255, 255, 255, 255}},
        {INPUT_BORDER_COLOR_PRESSED, {16, 7, 16, 200}}};

    const std::unordered_map<int, std::string> nonPrintableKeyNames = {
            {KEY_SPACE, "Space"},
            {KEY_UP, "Up"},
            {KEY_DOWN, "Down"},
            {KEY_LEFT, "Left"},
            {KEY_RIGHT, "Right"},
            {KEY_LEFT_SHIFT, "Left Shift"}
    };

    enum class ColorBlindMode {
        NONE,
        PROTANOPIA,
        DEUTERANOPIA,
        TRITANOPIA
    };
} // namespace ecs
