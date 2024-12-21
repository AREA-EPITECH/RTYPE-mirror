/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** DataType.hpp
*/

#pragma once

namespace ecs {
    enum WindowType {
        MENU,
        LOBBY,
        GAME,
    };

    enum WindowFocus {
        DEFAULT_FOCUS,

        // MENU
        MENU_FOCUS,
        JOIN_ROOM_FOCUS,

        // LOBBY
        LOBBY_FOCUS,

        // GAME
        GAME_FOCUS
    };

}