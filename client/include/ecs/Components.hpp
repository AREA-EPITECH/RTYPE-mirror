/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Components.hpp
*/

#pragma once

#include <utility>

#include "Events.hpp"
#include "core/ParticleSystem.hpp"
#include <cstring>

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
        Shader shader;
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

    class ButtonComponent {
    public:
        int buttonWidth;
        int buttonHeight;
        Color color{};
        TextComponent text;
        std::function<int(int screenWidth, int screenHeight)> dynamicX;
        std::function<int(int screenWidth, int screenHeight)> dynamicY;
        std::function<void()> onClick;

        int buttonX{};
        int buttonY{};

        void drawButton() const;

        void updateButton(int screenWidth, int screenHeight);

        bool isButtonPressed(Vector2 mousePosition);

        [[nodiscard]] bool isHover(Vector2 mousePosition) const;

        ButtonComponent(
                int _buttonWidth,
                int _buttonHeight,
                TextComponent _text,
                std::function<void()> _onClick = nullptr,
                std::function<int(int screenWidth, int screenHeight)> _dynamicX = nullptr,
                std::function<int(int screenWidth, int screenHeight)> _dynamicY = nullptr,
                Color _color = {120, 0, 0, 255});

    };

    class VesselsComponent {
    public:
        Model model{};
        bool drawable;
        std::string path;
        TextComponent name;
        Vector3 position = {0, 0, 0};

        VesselsComponent(Model _model, bool _drawable, std::string _path, TextComponent _name) {
            model = _model;
            drawable = _drawable;
            path = std::move(_path);
            name = std::move(_name);
        }

        void Move(const client::Direction direction, Camera &camera)
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
                if ((screen_pos.x -= 0.1f) > max_width)
                {
                    position.x -= 0.1f;
                }
                break;
            case client::Direction::RIGHT:
                if ((screen_pos.x += 0.1f) < min_width)
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
        int x{};
        int y{};
        int speed;

        explicit DecorElementComponent(const std::string &path, const int speed)
        {
            texture = LoadTexture(path.c_str());
            this->speed = speed;
        }

        void ResetPosition(const int screen_width, const int screen_height)
        {
            x = screen_width;
            y = GetRandomValue(0, screen_height - 50);
        }

        void Update(const float deltaTime, const int screen_width, const int screen_height)
        {
            x -= static_cast<int>(static_cast<float>(speed) * deltaTime);

            if (static_cast<float>(x) + static_cast<float>(texture.width) * (static_cast<float>(screen_height) /
                static_cast<float>(texture.height)) < 0) {
                ResetPosition(screen_width, screen_height);
            }
        }

        void DrawDecorElement(const int screen_width, const int screen_height) const
        {
            const float scale_factor = static_cast<float>(screen_height) / static_cast<float>(texture.height);
            const float scaled_width = static_cast<float>(texture.width) * scale_factor;

            DrawTexturePro(texture, {0, 0, static_cast<float>(texture.width),
                static_cast<float>(texture.height)}, {static_cast<float>(x), 0, scaled_width,
                static_cast<float>(screen_height)}, {0, 0}, 0.0f, WHITE
            );
        }
    };

    class ProjectilesComponent {
    public:
        Model model{};
        bool drawable;
        std::string path;
        Vector3 position{};
        bool player;
        Vector3 velocity{};

        ProjectilesComponent(Model _model, bool _drawable, std::string _path, Vector3 _position, bool _player,
            Vector3 _velocity) {
            model = _model;
            drawable = _drawable;
            path = std::move(_path);
            position = _position;
            player = _player;
            velocity = _velocity;
        }

        void ApplyVelocity() {
            position.x += velocity.x;
            position.y += velocity.y;
        }

        [[nodiscard]] bool IsAlive(const Camera &camera) const {
            const Vector2 screenPos = GetWorldToScreen(position, camera);

            return screenPos.x >= 0 && screenPos.x <= static_cast<float>(GetScreenWidth()) &&
                       screenPos.y >= 0 && screenPos.y <= static_cast<float>(GetScreenHeight());
        }
    };

    class TextInputComponent {
    public:
        Rectangle inputBox;
        std::string text;
        std::string placeholder;
        Color boxColor;
        Color textColor;
        Color borderColor;
        bool isFocused;
        size_t maxLength;

        std::function<int(int screenWidth, int screenHeight)> dynamicX;
        std::function<int(int screenWidth, int screenHeight)> dynamicY;

        TextInputComponent(Rectangle _inputBox, const std::string& _defaultText = "", size_t _maxLength = 256,
                           Color _boxColor = LIGHTGRAY, Color _textColor = BLACK, Color _borderColor = DARKGRAY,
                           std::function<int(int screenWidth, int screenHeight)> _dynamicX = nullptr,
                           std::function<int(int screenWidth, int screenHeight)> _dynamicY = nullptr);

        void drawTextInput();
        void handleInput();
        void updateInput(int screenWidth, int screenHeight);
    };

    class ShowBoxComponent {
    public:
        Rectangle boxRect;
        TextInputComponent textInput;
        ButtonComponent closeButton;
        ButtonComponent continueButton;
        std::string message;
        Color boxColor;
        Color textColor;
        bool isVisible;
        std::function<int(int screenWidth, int screenHeight)> dynamicX;
        std::function<int(int screenWidth, int screenHeight)> dynamicY;

        ShowBoxComponent(Rectangle _boxRect, std::string _message, Color _boxColor, Color _textColor,
                         TextInputComponent _textInput, ButtonComponent _closeButton,
                         ButtonComponent _continueButton, std::function<int(int screenWidth, int screenHeight)> _dynamicX = nullptr,
                         std::function<int(int screenWidth, int screenHeight)> _dynamicY = nullptr);

        void draw();
        void handleInput(char key);
        void handleClick(Vector2 mousePosition);
        void updateBox(int screenWidth, int screenHeight);
    };
}
