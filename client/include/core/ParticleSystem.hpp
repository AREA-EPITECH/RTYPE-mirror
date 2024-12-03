/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** ParticleSystem
*/

#pragma once
#include <vector>
#include "core/Particle.hpp"

namespace client
{
    class ParticleSystem
    {
    private:
        Vector3 _position;
        Direction _direction;
        bool _infinite;
        float _lifetime;
        size_t _max_particles;

    public:
        std::vector<Particle> _particleSystem;

        explicit ParticleSystem(const Vector3 &pos, Direction direction, bool infinite, float lifetime, size_t max_particles)
            noexcept;
        void draw();
        void update();
        bool isAlive() const;
    };
}