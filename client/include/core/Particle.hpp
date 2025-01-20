/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Particle
*/

#pragma once

extern "C"
{
    #include "raylib.h"
}

namespace client
{
    // Particle types
    enum ParticleType
    {
        REACTOR,
        PLAYER_BASIC_SHOT,
        PLAYER_SUPER_SHOT,
        ENEMY_BASIC_SHOT,
        ENEMY_SUPER_SHOT,

    };

    // Particle directions where it disperses
    enum Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    class Particle
    {
    private:
        Vector3 _position;
        Vector3 _velocity{};
        Vector3 _acceleration;
        ParticleType _type;
        Color _color;
        Direction _direction;
        float _size;

    public:
        Particle(const Vector3 &position, const Vector3 &acceleration, float size, ParticleType type, Color color,
            Direction direction);

        void draw() const;
        void update();
        [[nodiscard]] float getSize() const;
    };
}
