/*
** EPITECH PROJECT, 2024
** client
** File description:
** Lights
*/

#pragma once

#include "raylib.h"
#include <string>

namespace client
{
    #define MAX_LIGHTS  100  // Max dynamic lights supported by shaders implemented

    typedef enum {
        LIGHT_DIRECTIONAL = 0,
        LIGHT_POINT
    } LightType;

    class Light
    {
    private:
        // Light data
        LightType _type;
        Vector3 _target{};
        float _attenuation{};
        std::string _name;
        int _nb;
        bool _enabled;

        // Shader location
        int _enabledLoc{};
        int _typeLoc{};
        int _positionLoc{};
        int _targetLoc{};
        int _colorLoc{};
        int _attenuationLoc{};

    public:
        Vector3 _position{};
        Color _color{};

        Light(LightType type, Vector3 position, Vector3 target, Color color, int nb);
        void UpdateLightValues(Shader shader, bool enabled = true);
    };
}

