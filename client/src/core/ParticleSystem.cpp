/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** ParticleSystem
*/

#include "core/ParticleSystem.hpp"
#include <algorithm>

namespace client
{
     ParticleSystem::ParticleSystem(const Vector3 &pos, const Direction direction, const bool infinite,
        const float lifetime, const size_t max_particles) noexcept : _position(pos), _direction(direction),
        _infinite(infinite), _lifetime(lifetime), _max_particles(max_particles)
    {
         for (size_t i = _particleSystem.size(); i < _max_particles; i++)
         {
             // Add new particles to the particle system
             _particleSystem.emplace_back(_position, Vector3{0.0f, 0.05f, 0.0f}, 10.0f, REACTOR,
                 BLUE, _direction);
         }
    }

    void ParticleSystem::draw()
     {
         for (auto obj = _particleSystem.begin(); obj < _particleSystem.end(); ++obj)
         {
             // Draw all particles in the particle system
             obj->draw();
         }
     }

    void ParticleSystem::update()
     {
         if (!this->isAlive())
             // If the particle system is not alive, return, it should be deleted soon in game loop
             return;

         // Remove particles that are too small
         _particleSystem.erase(
             std::remove_if(_particleSystem.begin(), _particleSystem.end(), []( auto &p) {
                 p.update();
                 return p.getSize() <= 5;
            }),
            _particleSystem.end()
         );

         for (size_t i = _particleSystem.size(); i < _max_particles; i++)
         {
             // Add new particles to the particle system to replace the ones that were deleted
             _particleSystem.emplace_back(_position, Vector3{0.0f, 0.0f, 0.0f}, 10.0f, REACTOR,
                 BLUE, _direction);
         }

         if (!_infinite)
             _lifetime -= 1;
     }

    bool ParticleSystem::isAlive() const
     {
         if (!_infinite && _lifetime <= 0)
             return false;

         return true;
     }

}