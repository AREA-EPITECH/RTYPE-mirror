/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Components.hpp
*/

#pragma once

#include <cstring>
#include <utility>
#include "Events.hpp"
#include "DataType.hpp"
#include <random>
#include "ecs/DataType.hpp"

namespace ecs {
    class TextureManager
    {
    public:
        std::map<tower_defense::TileType, std::shared_ptr<Texture2D>> _textures;
        std::vector<std::shared_ptr<Texture2D>> _decors_textures;

        TextureManager() = default;
        ~TextureManager() = default;

        void add_texture(const tower_defense::TileType type, const std::string &path)
        {
            if (_textures.find(type) != _textures.end())
                return;
            if (type == tower_defense::DECOR)
            {
                _decors_textures.emplace_back(std::make_shared<Texture2D>(LoadTexture(path.c_str())));
                return;
            }
            _textures[type] = std::make_shared<Texture2D>(LoadTexture(path.c_str()));
        }

        std::shared_ptr<Texture2D> get_texture(const tower_defense::TileType type)
        {
            if (type == tower_defense::DECOR)
            {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(0, static_cast<int>(_decors_textures.size() - 1));

                return _decors_textures[dis(gen)];
            }
            return _textures[type];
        }
    };

    struct Window {
        int width;
        int height;
        std::string title;
        bool isOpen = false;
    };

    struct Money
    {
        unsigned int _value;
        Texture2D _texture;
    };

    struct Life
    {
        int _health;
        Texture2D _texture;
    };

    struct GameComponent
    {
        std::string _map_name;
        Money _money;
        Life _life;
        int _frame_counter;
        float _frame_time;
        std::vector<std::vector<tower_defense::EnemyWave>> _enemy_waves = {};
        int _current_wave = 0;
        std::chrono::time_point<std::chrono::steady_clock> _wave_start_time = std::chrono::steady_clock::now();
        bool _wave_started = false;
    };

    struct Tile {
        tower_defense::Position _position;
        tower_defense::TileType _type;
        std::shared_ptr<Texture2D> _texture;
    };

    struct EnemyComponent
    {
        int _health;
        int _speed; // Moves every _speed second(s)
        int _damage;
        int _reward;
        std::shared_ptr<Texture2D> _texture;
        tower_defense::Position _position;
        tower_defense::EnemyType _enemy_type;
        int _frame = 0;
        float _frame_counter = 0.0f;
        std::chrono::time_point<std::chrono::steady_clock> _last_move = std::chrono::steady_clock::now();
    };

    struct Tower
    {
        int _range;
        int _damage;
        int _fire_rate; // Time between each shot
        std::chrono::time_point<std::chrono::steady_clock> _last_shot = std::chrono::steady_clock::now();
        int _cost;
        std::string _name;
        std::shared_ptr<Texture2D> _texture;
        int _frame = 0;
        float _frame_counter = 0.0f;
        tower_defense::TileType _tower_type;
        tower_defense::Position _position;
    };

    struct MapComponent {
        std::vector<Tile> _grass;
        std::vector<Tile> _path;
        std::vector<Tile> _decors;
        GameComponent _game;
    };

    struct Type
    {
        tower_defense::TileType _type;
    };

    struct SelectorComponent
    {
        Texture2D _texture{};
        tower_defense::Position _position{};
        bool _drawable{};
        std::vector<Rectangle> _no_clickable;
    };

    struct Shop
    {
        std::vector<Tower> _towers;
        bool _open;
    };

    struct TextComponent
    {
        std::string _text;
        std::chrono::time_point<std::chrono::steady_clock> _creation;
        int _duration;
        tower_defense::Position _position;
        tower_defense::Position _velocity;
        Color _color;
        int _font_size = 32;
    };

    class SoundComponent {
    public:
        float volume;
        std::unordered_map<tower_defense::SoundType, Sound> sounds;

        SoundComponent(float initialVolume = 50.0f)
                : volume(initialVolume) {}

        ~SoundComponent() {
            for (auto &[key, sound] : sounds) {
                UnloadSound(sound);
            }
        }

        void addSound(const tower_defense::SoundType key, const std::string &file_path) {
            if (sounds.find(key) != sounds.end()) {
                return;
            }
            Sound sound = LoadSound(file_path.c_str());
            SetSoundVolume(sound, volume / 100.0f);
            sounds[key] = sound;
        }

        void play(const tower_defense::SoundType key) {
            if (sounds.find(key) == sounds.end()) {
                return;
            }
            PlaySound(sounds[key]);
        }

        void stop(const tower_defense::SoundType key) {
            auto it = sounds.find(key);
            if (it == sounds.end()) {
                return;
            }

            StopSound(it->second);
            UnloadSound(it->second);

            sounds.erase(it);
        }


        void pause(const tower_defense::SoundType key) {
            if (sounds.find(key) == sounds.end()) {
                return;
            }
            PauseSound(sounds[key]);
        }

        void resume(const tower_defense::SoundType key) {
            if (sounds.find(key) == sounds.end()) {
                return;
            }
            ResumeSound(sounds[key]);
        }

        bool isPlaying(const tower_defense::SoundType key) const {
            if (sounds.find(key) == sounds.end()) {
                return false;
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
        std::unordered_map<tower_defense::MusicType, Music> musics;

        MusicComponent(float initialVolume = 50.0f)
                : volume(initialVolume) {}

        ~MusicComponent() {
            for (auto &[key, music] : musics) {
                StopMusicStream(music);
                UnloadMusicStream(music);
            }
        }

        void addMusic(const tower_defense::MusicType key, const std::string &filePath) {
            if (musics.find(key) != musics.end()) {
                return;
            }
            Music music = LoadMusicStream(filePath.c_str());
            SetMusicVolume(music, volume / 100.0f);
            musics[key] = music;
        }

        void play(const tower_defense::MusicType key) {
            if (musics.find(key) == musics.end()) {
                return;
            }
            PlayMusicStream(musics[key]);
        }

        void stop(const tower_defense::MusicType key) {
            auto it = musics.find(key);
            if (it == musics.end()) {
                return;
            }

            StopMusicStream(it->second);
            UnloadMusicStream(it->second);

            musics.erase(it);
        }

        void pause(const tower_defense::MusicType key) {
            if (musics.find(key) == musics.end()) {
                return;
            }
            PauseMusicStream(musics[key]);
        }

        void resume(const tower_defense::MusicType key) {
            if (musics.find(key) == musics.end()) {
                return;
            }
            ResumeMusicStream(musics[key]);
        }

        void update(const tower_defense::MusicType key) {
            if (musics.find(key) == musics.end()) {
                return;
            }
            UpdateMusicStream(musics[key]);
        }

        bool isPlaying(const tower_defense::MusicType key) const {
            if (musics.find(key) == musics.end()) {
                return false;
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
}
