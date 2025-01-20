/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Components.hpp
*/

#pragma once

#define MAX_HEALTH 10

#include <utility>

#include "raylib/raygui.h"
#include "Events.hpp"
#include "Registry.hpp"
#include "Components/Selectors.hpp"
#include "Components/Controls.hpp"
#include "core/ParticleSystem.hpp"
#include <cstring>
#include <string>
#include <spdlog/spdlog.h>

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

    struct ShaderComponent {
        std::shared_ptr<Shader> shader;
        std::string vs_file;
        std::string fs_file;
    };

    struct LightComponent {
        std::shared_ptr<client::Light> light;
    };

    struct ParticleSystemComponent {
        client::ParticleSystem particleSystem;
    };

    class TextComponent {
    public:
        std::string text;
        int posX{};
        int posY{};
        int fontSize{};
        Color color{};
        int type{};

        TextComponent(std::string _text, int _fontSize, int _posX, int _posY, int _type = 0,  Color _color = {253, 240, 213, 255}) {
            text = std::move(_text);
            posX = _posX;
            posY = _posY;
            fontSize = _fontSize;
            color = _color;
            type = _type;
        }

        explicit TextComponent(std::string _text, int _type = 0, Color _color = {253, 240, 213, 255}) {
            text = std::move(_text);
            color = _color;
            type = _type;
        }

        TextComponent() {
            text = "";
            color = {253, 240, 213, 255};
            type = 0;
        };

        void updateText(int screenWidth, int screenHeight) {
            int fontSizeFromHeight = static_cast<int>(static_cast<float>(screenHeight) * 0.05f);
            int fontSizeFromWidth = static_cast<int>(static_cast<float>(screenWidth) * 0.03f);

            fontSize = std::min(fontSizeFromHeight, fontSizeFromWidth);
        }
    };

    class VesselsComponent {
    public:
        uint32_t id;
        Model model{};
        bool drawable;
        std::string path;
        TextComponent name;
        Vector3 position = {0, 0, 0};
        int health = MAX_HEALTH;
        int ship_id = 0;
        bool is_enemy = false;

        VesselsComponent(uint32_t _id, Model _model, bool _drawable, std::string _path, TextComponent _name, int _ship_id, bool _is_enemy) {
            id = _id;
            model = _model;
            drawable = _drawable;
            path = std::move(_path);
            name = std::move(_name);
            ship_id = _ship_id;
            is_enemy = _is_enemy;
        }

        void Move(const client::Direction direction, const Camera &camera)
        {
            Vector2 screen_pos = GetWorldToScreen(position, camera);
            const float max_height = static_cast<float>(GetScreenHeight()) * 0.1f;
            const float min_height = static_cast<float>(GetScreenHeight()) * 0.9f;

            const float min_width = static_cast<float>(GetScreenWidth()) * 0.15f;
            const float max_width = static_cast<float>(GetScreenWidth()) * 0.6f;

            switch (direction)
            {
            case client::Direction::UP:
                if ((screen_pos.y += 0.1f) > max_height)
                {
                    position.y += 0.1f;
                }
                break;
            case client::Direction::DOWN:
                if ((screen_pos.y -= 0.1f) < min_height)
                {
                    position.y -= 0.1f;
                }
                break;
            case client::Direction::LEFT:
                if ((screen_pos.x -= 0.1f) > min_width)
                {
                    position.x -= 0.1f;
                }
                break;
            case client::Direction::RIGHT:
                if ((screen_pos.x += 0.1f) < max_width)
                {
                    position.x += 0.1f;
                }
                break;
            default:
                break;
           }
        }
    };

    struct ControllableComponent{};

    struct EnemyComponent{};

    class MenuText {
    public:
        Model model{};

        MenuText(Model _model) {
            model = _model;
        }
    };

    class BackgroundComponent
    {
    public:
        Texture2D texture{};
        size_t depth;
        float speed;
        float offset;

        explicit BackgroundComponent(const std::string &path, const size_t depth, const float speed,
            const float offset) {
            texture = LoadTexture(path.c_str());
            this->depth = depth;
            this->speed = speed;
            this->offset = offset;
        }

        void DrawLayer(const int screen_width, const int screen_height) const {
            const float texture_width = static_cast<float>(this->texture.width) *
                (static_cast<float>(screen_height) / static_cast<float>(this->texture.height));
            const int repeatCount = static_cast<int>(screen_width / texture_width) + 2;

            for (int i = 0; i < repeatCount; i++) {
                DrawTexturePro(
                    this->texture,
                    {0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)},
                    {i * texture_width + offset, 0, texture_width, static_cast<float>(screen_height)},
                    {0, 0}, 0.0f, WHITE);
            }
        }
    };

    class DecorElementComponent
    {
    public:
        Texture2D texture{};
        float x{};
        int y{};
        int speed;
        size_t depth;

        explicit DecorElementComponent(const std::string &path, const int speed, const size_t depth)
        {
            texture = LoadTexture(path.c_str());
            this->speed = speed;
            this->depth = depth;
        }

        void ResetPosition(const int screen_width, const int screen_height)
        {
            x = static_cast<float>(screen_width);
            y = GetRandomValue(0, screen_height - 50);
        }

        void Update(const float deltaTime, const int screen_width, const int screen_height)
        {
            x -= static_cast<float>(speed) * deltaTime;

            if (x + static_cast<float>(texture.width) * (static_cast<float>(screen_height) /
                static_cast<float>(texture.height)) < 0) {
                ResetPosition(screen_width, screen_height);
            }
        }

        void DrawDecorElement(const int screen_width, const int screen_height) const
        {
            const float scale_factor = static_cast<float>(screen_height) / static_cast<float>(texture.height);
            const float scaled_width = static_cast<float>(texture.width) * scale_factor;

            DrawTexturePro(texture, {0, 0, static_cast<float>(texture.width),
                static_cast<float>(texture.height)}, {x, 0, scaled_width,
                static_cast<float>(screen_height)}, {0, 0}, 0.0f, WHITE
            );
        }
    };

    class ProjectilesComponent {
    public:
        uint32_t id;
        Model model{};
        bool drawable;
        std::string path;
        Vector3 position{};
        bool player;
        Vector3 velocity{};
        std::shared_ptr<client::Light> light;

        ProjectilesComponent(uint32_t _id, Model _model, bool _drawable, std::string _path, Vector3 _position, bool _player,
            Vector3 _velocity, Vector3 _target, Color _color, int _nb, Shader _shader) {
            id = _id;
            model = _model;
            drawable = _drawable;
            path = std::move(_path);
            position = _position;
            player = _player;
            velocity = _velocity;
            light = std::make_shared<client::Light>(client::LIGHT_DIRECTIONAL, position, _target, _color, _nb);
            light->UpdateLightValues(_shader);
        }

        void ApplyVelocity(const float deltaTime) {
            position.x += velocity.x * deltaTime;
            position.y += velocity.y * deltaTime;
            light->_position.x += velocity.x * deltaTime;
            light->_position.y += velocity.y * deltaTime;
        }

        [[nodiscard]] bool IsAlive(const Camera &camera) const {
            const Vector2 screenPos = GetWorldToScreen(position, camera);

            return screenPos.x >= 0 && screenPos.x <= static_cast<float>(GetScreenWidth()) &&
                       screenPos.y >= 0 && screenPos.y <= static_cast<float>(GetScreenHeight());
        }
    };

    struct HealthBarComponent
    {
        std::vector<Texture> textures;
    };

    class ScoreComponent {
    public:
        int win_score;
        int score;
        int level = 1;

        ScoreComponent(int _win_score = 0, int _score = 0) : win_score(_win_score), score(_score) {};

        void draw_ingame();
    };

    class FilterComponent {
    public:
        ColorBlindMode currentMode;

        FilterComponent(ColorBlindMode mode = ColorBlindMode::NONE)
                : currentMode(mode) {

        }

        ~FilterComponent() = default;

        void setMode(ColorBlindMode mode) {
            currentMode = mode;
        }

        void applyFilter() {
            if (currentMode == ColorBlindMode::NONE) return;

            applyColorFilter();
        }

    private:
        void applyColorFilter() {
            Color filterColor;

            switch (currentMode) {
                case ColorBlindMode::PROTANOPIA:
                    filterColor = Color{255, 180, 180, 50};
                    break;
                case ColorBlindMode::DEUTERANOPIA:
                    filterColor = Color{180, 255, 180, 50};
                    break;
                case ColorBlindMode::TRITANOPIA:
                    filterColor = Color{180, 180, 255, 50};
                    break;
                case ColorBlindMode::NONE:
                default:
                    return;
            }

            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), filterColor);
        }

    };


    class ExplosionComponent {
    public:
        Texture2D spriteSheet;
        int frameWidth;
        int frameHeight;
        int totalFrames;
        int currentFrame;
        float frameTime;
        float elapsedTime;
        bool active;
        Vector3 position;
        float scale;

        ExplosionComponent(const std::string &spritePath, int frameWidth, int frameHeight, int totalFrames, float frameTime, Vector3 position)
                : frameWidth(frameWidth), frameHeight(frameHeight), totalFrames(totalFrames),
                  currentFrame(0), frameTime(frameTime), elapsedTime(0.0f), active(true), position(position), scale(2.0f) {
            spriteSheet = LoadTexture(spritePath.c_str());
        }

        void UnloadExplosion () {
            UnloadTexture(spriteSheet);
        }

        ~ExplosionComponent() = default;

        void update(float deltaTime) {
            if (!active) return;

            elapsedTime += deltaTime;

            if (elapsedTime >= frameTime) {
                elapsedTime = 0.0f;
                currentFrame++;

                if (currentFrame >= totalFrames) {
                    active = false;
                    currentFrame = 0;
                }
            }
        }

        void draw(Camera camera, Vector3 maxHitbox, Vector3 minHitbox) {
            if (!active) return;

            Rectangle sourceRect = {
                    static_cast<float>(currentFrame * frameWidth),
                    0.0f,
                    static_cast<float>(frameWidth),
                    static_cast<float>(frameHeight)
            };

            Vector2 pos2d = GetWorldToScreen(position, camera);
            Vector2 maxhitbox2d = GetWorldToScreen(maxHitbox, camera);
            Vector2 minhitbox2d = GetWorldToScreen(minHitbox, camera);
            Rectangle destRect = {
                    pos2d.x - 80,
                    pos2d.y - 50,
                    static_cast<float>(frameWidth) * scale,
                    static_cast<float>(frameHeight) * scale
            };

            DrawTexturePro(spriteSheet, sourceRect, destRect, {0.0f, 0.0f}, 0.0f, WHITE);
        }
    };

    class PopupComponent
    {
    public:
        Rectangle boxRect;
        std::string title;
        std::string message;
        Color boxColor;
        Color textColor;
        bool isVisible = true;
        std::function<int(int screenWidth, int screenHeight)> dynamicX;
        std::function<int(int screenWidth, int screenHeight)> dynamicY;
        WindowFocus focus;
        std::function<void()> close_func;

        explicit PopupComponent(Rectangle _boxRect, std::string _title, std::string _message, Color _boxColor, Color _textColor,
                                WindowFocus _focus,
                                std::function<int(int screenWidth, int screenHeight)> _dynamicX = nullptr,
                                std::function<int(int screenWidth, int screenHeight)> _dynamicY = nullptr,
                                std::function<void()> _close_func = nullptr);

        PopupComponent() = default;

        void draw(WindowFocus _focus);
        void updateBox(int screenWidth, int screenHeight);
        static void createPopup(Registry &ecs, const std::string &title, const std::string &message);
    };
}
