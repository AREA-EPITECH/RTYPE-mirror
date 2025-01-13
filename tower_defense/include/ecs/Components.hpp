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

    struct Money
    {
        unsigned int _value;
        Texture2D _texture;
    };

    struct GameComponent
    {
        unsigned int _health;
        bool _auto_increment_money;
        std::string _map_name;
        Money _money;
    };

    struct Tile {
        int _x;
        int _y;
        Texture2D _texture;
    };

    struct MapComponent {
        std::vector<Tile> _grass;
        std::vector<Tile> _path;
        std::vector<Tile> _decors;
        GameComponent _game;
    };

    struct EnemyComponent
    {
        Texture2D _texture{};
        bool _drawable = false;
        Tile _pos{};
    };

    struct SelectorComponent
    {
        Texture2D _texture;
        Tile _pos{};
        bool _drawable;
    };
}
