/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** Controls.hpp
*/

#pragma once

#include <utility>

namespace ecs {

    class KeyBindingComponent {
    private:
        std::unordered_map<std::string, std::tuple<int, int>> key_bindings;

    public:
        KeyBindingComponent() {
            key_bindings["Move Up"] = std::make_tuple(KEY_Z, KEY_Z);
            key_bindings["Move Down"] = std::make_tuple(KEY_S, KEY_S);
            key_bindings["Move Left"] = std::make_tuple(KEY_Q, KEY_Q);
            key_bindings["Move Right"] = std::make_tuple(KEY_D, KEY_D);
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
        Sound sound;

        SoundComponent(const std::string &filePath, float initialVolume = 50.0f)
                : volume(initialVolume) {
            sound = LoadSound(filePath.c_str());
            SetSoundVolume(sound, volume / 100.0f);
        }

        void play() {
            PlaySound(sound);
        }

        void stop() {
            StopSound(sound);
        }

        void pause() {
            PauseSound(sound);
        }

        void resume() {
            ResumeSound(sound);
        }

        bool isPlaying() const {
            return IsSoundPlaying(sound);
        }

        void setVolume(float newVolume) {
            volume = newVolume;
            SetSoundVolume(sound, volume / 100.0f);
        }

    };

    class MusicComponent {
    public:
        float volume;
        Music sound;

        MusicComponent(const std::string &filePath, float initialVolume = 50.0f)
                :  volume(initialVolume) {
            sound = LoadMusicStream(filePath.c_str());
            SetMusicVolume(sound, volume / 100.0f);
        }

        void play() {
            PlayMusicStream(sound);
        }

        void stop() {
            StopMusicStream(sound);
        }

        void pause() {
            PauseMusicStream(sound);
        }

        void update() {
            UpdateMusicStream(sound);
        }

        void resume() {
            ResumeMusicStream(sound);
        }

        bool isPlaying() const {
            return IsMusicStreamPlaying(sound);
        }

        void setVolume(float newVolume) {
            volume = newVolume;
            SetMusicVolume(sound, volume / 100.0f);
        }
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
