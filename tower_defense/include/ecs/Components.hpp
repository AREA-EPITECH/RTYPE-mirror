/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Components.hpp
*/

#pragma once
#include <cstring>
#include <utility>
#include "Events.hpp"

#include "DataType.hpp"

namespace ecs {
    struct Window {
        int width;
        int height;
        std::string title;
        bool isOpen = false;
    };

    struct Tile {
        int _x;
        int _y;
        Texture2D _texture;
    };

    struct PathComponent {};

    struct MapComponent {
        std::vector<Tile> _grass;
        std::vector<Tile> _path;
        std::vector<Tile> _decors;
    };

    struct EnemyComponent
    {
        Texture2D _texture{};
        bool _drawable = false;
        Tile _pos{};
    };
}
