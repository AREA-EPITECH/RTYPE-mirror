#include <utility>

#pragma once

namespace ecs {

    class KeyBindingComponent {
    private:
        std::unordered_map<std::string, std::tuple<int, int>> key_bindings;

    public:
        KeyBindingComponent() {
            key_bindings["Move Up"] = std::make_tuple(KEY_UP, KEY_UP);
            key_bindings["Move Down"] = std::make_tuple(KEY_DOWN, KEY_DOWN);
            key_bindings["Move Left"] = std::make_tuple(KEY_LEFT, KEY_LEFT);
            key_bindings["Move Right"] = std::make_tuple(KEY_RIGHT, KEY_RIGHT);
            key_bindings["Basic Shoot"] = std::make_tuple(KEY_SPACE, KEY_SPACE);
            key_bindings["Special Shoot"] = std::make_tuple(KEY_LEFT_SHIFT, KEY_LEFT_SHIFT);
        }

        int getKey(const std::string &action) const {
            return std::get<0>(key_bindings.at(action));
        }

        void setKey(const std::string &action, int newKey) {
            std::get<0>(key_bindings[action]) = newKey;
        }

        void resetKey(const std::string &action) {
            std::get<0>(key_bindings[action]) = std::get<1>(key_bindings[action]);
        }

        void resetAllKeys() {
            for (auto& [action, key_tuple] : key_bindings) {
                std::get<0>(key_tuple) = std::get<1>(key_tuple);
            }
        }

        std::vector<std::string> getActions() const {
            std::vector<std::string> actions;
            for (const auto& [action, key_tuple] : key_bindings) {
                actions.push_back(action);
            }
            return actions;
        }
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
