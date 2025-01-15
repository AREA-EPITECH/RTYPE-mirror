/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** Pos
*/

#pragma once

namespace server {
    struct Pos {
        int x;
        int y;
    };

    struct Acceleration {
        int x;
        int y;
    };

    struct Projectile {
        Pos pos;
        Acceleration acceleration;
        network::FireType type;
        bool _from_player;
    };
} // namespace server