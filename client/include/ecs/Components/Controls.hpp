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
        std::unordered_map<std::string, Sound> sounds;

        SoundComponent(float initialVolume = 50.0f)
                : volume(initialVolume) {}

        ~SoundComponent() {
            for (auto &[key, sound] : sounds) {
                UnloadSound(sound);
            }
        }

        void addSound(const std::string &key, const std::string &filePath) {
            if (sounds.find(key) != sounds.end()) {
                throw std::runtime_error("Sound key already exists: " + key);
            }
            Sound sound = LoadSound(filePath.c_str());
            SetSoundVolume(sound, volume / 100.0f);
            sounds[key] = sound;
        }

        void play(const std::string &key) {
            if (sounds.find(key) == sounds.end()) {
                throw std::runtime_error("Sound key not found: " + key);
            }
            PlaySound(sounds[key]);
        }

        void stop(const std::string &key) {
            auto it = sounds.find(key);
            if (it == sounds.end()) {
                throw std::runtime_error("Sound key not found: " + key);
            }

            StopSound(it->second);
            UnloadSound(it->second);

            sounds.erase(it);
        }


        void pause(const std::string &key) {
            if (sounds.find(key) == sounds.end()) {
                throw std::runtime_error("Sound key not found: " + key);
            }
            PauseSound(sounds[key]);
        }

        void resume(const std::string &key) {
            if (sounds.find(key) == sounds.end()) {
                throw std::runtime_error("Sound key not found: " + key);
            }
            ResumeSound(sounds[key]);
        }

        bool isPlaying(const std::string &key) const {
            if (sounds.find(key) == sounds.end()) {
                throw std::runtime_error("Sound key not found: " + key);
            }
            return IsSoundPlaying(sounds.at(key));
        }

        void setVolume(float newVolume) {
            volume = newVolume;
            for (auto &[key, sound] : sounds) {
                SetSoundVolume(sound, volume / 100.0f);
            }
        }
    };

    class MusicComponent {
    public:
        float volume;
        std::unordered_map<std::string, Music> musics;

        MusicComponent(float initialVolume = 50.0f)
                : volume(initialVolume) {}

        ~MusicComponent() {
            for (auto &[key, music] : musics) {
                StopMusicStream(music);
                UnloadMusicStream(music);
            }
        }

        void addMusic(const std::string &key, const std::string &filePath) {
            if (musics.find(key) != musics.end()) {
                throw std::runtime_error("Music key already exists: " + key);
            }
            Music music = LoadMusicStream(filePath.c_str());
            SetMusicVolume(music, volume / 100.0f);
            musics[key] = music;
        }

        void play(const std::string &key) {
            if (musics.find(key) == musics.end()) {
                throw std::runtime_error("Music key not found: " + key);
            }
            PlayMusicStream(musics[key]);
        }

        void stop(const std::string &key) {
            auto it = musics.find(key);
            if (it == musics.end()) {
                throw std::runtime_error("Music key not found: " + key);
            }

            StopMusicStream(it->second);
            UnloadMusicStream(it->second);

            musics.erase(it);
        }

        void pause(const std::string &key) {
            if (musics.find(key) == musics.end()) {
                throw std::runtime_error("Music key not found: " + key);
            }
            PauseMusicStream(musics[key]);
        }

        void resume(const std::string &key) {
            if (musics.find(key) == musics.end()) {
                throw std::runtime_error("Music key not found: " + key);
            }
            ResumeMusicStream(musics[key]);
        }

        void update(const std::string &key) {
            if (musics.find(key) == musics.end()) {
                throw std::runtime_error("Music key not found: " + key);
            }
            UpdateMusicStream(musics[key]);
        }

        bool isPlaying(const std::string &key) const {
            if (musics.find(key) == musics.end()) {
                throw std::runtime_error("Music key not found: " + key);
            }
            return IsMusicStreamPlaying(musics.at(key));
        }

        void setVolume(float newVolume) {
            volume = newVolume;
            for (auto &[key, music] : musics) {
                SetMusicVolume(music, volume / 100.0f);
            }
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
