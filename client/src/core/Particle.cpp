/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Particle
*/

#include <algorithm>
#include "core/Particle.hpp"

namespace client
{
    /**
     * @brief Construct a new Particle:: Particle object
     * @param position
     * @param acceleration
     * @param size
     * @param type
     * @param color
     * @param direction
     */
    Particle::Particle(const Vector3 &position, const Vector3 &acceleration, const float size,
        const ParticleType type, const Color color, const Direction direction) :
        _position(position), _acceleration(acceleration), _type(type), _color(color),
    _direction(direction), _size(size)
    {
        switch (direction) {
        case DOWN:
            _velocity = Vector3{
                static_cast<float>(GetRandomValue(-20, 20)) * 0.1f,
                static_cast<float>(GetRandomValue(20, 50)) * 0.1f,
                static_cast<float>(GetRandomValue(-20, 20)) * 0.1f
            };
            break;
        case UP:
            _velocity = Vector3{
                static_cast<float>(GetRandomValue(-20, 20)) * 0.1f,
                static_cast<float>(GetRandomValue(-50, -20)) * 0.1f,
                static_cast<float>(GetRandomValue(-20, 20)) * 0.1f
            };
            break;
        case LEFT:
            _velocity = Vector3{
                static_cast<float>(GetRandomValue(-50, -20)) * 0.1f,
                static_cast<float>(GetRandomValue(-20, 20)) * 0.1f,
                static_cast<float>(GetRandomValue(-20, 20)) * 0.1f
            };
            break;
        case RIGHT:
            _velocity = Vector3{
                static_cast<float>(GetRandomValue(20, 50)) * 0.1f,
                static_cast<float>(GetRandomValue(-20, 20)) * 0.1f,
                static_cast<float>(GetRandomValue(-20, 20)) * 0.1f
            };
            break;
        default:
                _velocity = Vector3{
                    static_cast<float>(GetRandomValue(-20, 20)) * 0.1f,
                    static_cast<float>(GetRandomValue(-50, -20)) * 0.1f,
                    static_cast<float>(GetRandomValue(-20, 20)) * 0.1f
                };
            break;
        }
    }

    /**
     * @brief Draw cube for particle
     */
    void Particle::draw() const
    {
        if (_size > 0)
            DrawCube(_position, _size, _size, _size, _color);
    }

    /**
     * @brief Update particle
     */
    void Particle::update()
    {
        _velocity = {_velocity.x + _acceleration.x, _velocity.y + _acceleration.y, _velocity.z + _acceleration.z};
        _position = {_position.x + _velocity.x, _position.y + _velocity.y, _position.z + _velocity.z};

        if (_size > 0.0f) {
            _size = std::max(0.0f, _size - 0.1f);
        }
    }

    /**
     * @brief Get the size object
     * @return float
     */
    float Particle::getSize() const
    {
        return _size;
    }
}
