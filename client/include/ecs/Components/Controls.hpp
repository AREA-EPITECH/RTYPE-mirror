#include <utility>

#pragma once

namespace ecs {

    class ControlsComponent {
    public:
        KeyType type;
        KeyboardKey key;
        KeyboardKey initial_key;

        ControlsComponent(KeyType _type, KeyboardKey _key) : type(_type), initial_key(_key), key(_key) {}
    };

    class SoundComponent {
    public:
        float volume;

        SoundComponent(float initialVolume = 50.0f);

        void drawVolumeSlider(int x, int y, int width);
    };

    class SettingsComponent {
    public:
        ImageComponent background;

        SettingsComponent(ImageComponent _background) : background(std::move(_background)) {};

        void drawSettings(int screenWidth, int screenHeight) {
            background.draw(screenWidth, screenHeight);
        };
    };

}
