#pragma once

#include "Events.hpp"
#include "core/ParticleSystem.hpp"
#include "core/Lights.hpp"

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
        std::string path;
    };

    struct ShaderComponent {
        Shader shader;
    };

    struct LightComponent {
        std::shared_ptr<client::Light> light;
    };

    struct ParticleSystemComponent {
        client::ParticleSystem particleSystem;
    };
}
