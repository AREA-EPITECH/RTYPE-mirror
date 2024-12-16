/*
** EPITECH PROJECT, 2024
** client
** File description:
** Light
*/

#include "core/Lights.hpp"

namespace client
{

    /**
     * @brief Construct a new Light:: Light object
     * @param type
     * @param position
     * @param target
     * @param color
     * @param nb
     */
    Light::Light(const LightType type, const Vector3 position, const Vector3 target, const Color color, const int nb)
     {
        _nb = nb;
        if (nb < MAX_LIGHTS)
        {
            TraceLog(LOG_WARNING, TextFormat("Creating light number %d.", nb));
            _enabled = true;
            _type = type;
            _position = position;
            _target = target;
            _color = color;
        } else
        {
            TraceLog(LOG_WARNING, TextFormat("Max number of lights reached, can't create new lights."));
        }
     }

    /**
     * @brief Update the light values
     * @param shader
     */
    void Light::UpdateLightValues(const Shader shader)
    {
        _enabledLoc = GetShaderLocation(shader, TextFormat("lights[%i].enabled", _nb));
        _typeLoc = GetShaderLocation(shader, TextFormat("lights[%i].type", _nb));
        _positionLoc = GetShaderLocation(shader, TextFormat("lights[%i].position", _nb));
        _targetLoc = GetShaderLocation(shader, TextFormat("lights[%i].target", _nb));
        _colorLoc = GetShaderLocation(shader, TextFormat("lights[%i].color", _nb));

        SetShaderValue(shader, _enabledLoc, &_enabled, SHADER_UNIFORM_INT);
        SetShaderValue(shader, _typeLoc, &_type, SHADER_UNIFORM_INT);

        const float position[3] = { _position.x, _position.y, _position.z };
        SetShaderValue(shader, _positionLoc, position, SHADER_UNIFORM_VEC3);

        const float target[3] = { _target.x, _target.y, _target.z };
        SetShaderValue(shader, _targetLoc, target, SHADER_UNIFORM_VEC3);

        const float color[4] = { static_cast<float>(_color.r) / static_cast<float>(255),
            static_cast<float>(_color.g) / static_cast<float>(255),
            static_cast<float>(_color.b) / static_cast<float>(255),
            static_cast<float>(_color.a) / static_cast<float>(255)
        };
         SetShaderValue(shader, _colorLoc, color, SHADER_UNIFORM_VEC4);
    }
}
