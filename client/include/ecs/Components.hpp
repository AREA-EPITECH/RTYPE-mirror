#pragma once

#include "Events.hpp"
#include "core/ParticleSystem.hpp"

namespace ecs {
    struct Window {
        int width;
        int height;
        std::string title;
        bool isOpen = false;
    };

    struct CameraComponent {
        Camera camera;
    };

    struct ModelComponent {
        Model model;
        bool drawable;
    };

    struct ShaderComponent {
        Shader shader;
    };

    struct LightComponent {
        Vector3 position;
        Color color;
    };

    struct ParticleSystemComponent {
        client::ParticleSystem particleSystem;
    };
}
