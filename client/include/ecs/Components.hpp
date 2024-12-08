#pragma once

#include <utility>

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

        TextComponent(std::string _text, int _type = 0, Color _color = {253, 240, 213, 255}) {
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
        int buttonX;
        int buttonY;
        Color color{};
        TextComponent text;
        std::function<void()> onClick;

        void drawButton() const {
            DrawRectangle(buttonX, buttonY, buttonWidth, buttonHeight, color);
            DrawText(text.text.c_str(), text.posX, text.posY, text.fontSize, text.color);
        }

        void updateButton (int screenWidth, int screenHeight) {
            buttonX = static_cast<int>(static_cast<float>(screenWidth) * 0.66 - (buttonWidth / 2) + 20);
            buttonY = screenHeight - 150;

            int textWidth = MeasureText(text.text.c_str(), text.fontSize);
            text.posX = buttonX + (buttonWidth - textWidth) / 2;
            text.posY = buttonY + (buttonHeight - text.fontSize) / 2;
        }

        bool isButtonPressed (Vector2 mousePosition) {
            if (mousePosition.x >= buttonX && mousePosition.x <= (buttonX + buttonWidth) &&
                mousePosition.y >= buttonY && mousePosition.y <= (buttonY + buttonHeight)) {
                onClick();
                return true;
            }
            return false;
        }

        ButtonComponent(int _buttonWidth, int _buttonHeight, TextComponent _text, std::function<void()> _onClick = nullptr, int _buttonX = 0, int _buttonY = 0, Color _color = {120, 0, 0, 255}) {
            buttonWidth = _buttonWidth;
            buttonHeight = _buttonHeight;
            buttonX = _buttonX;
            buttonY = _buttonY;
            color = _color;
            onClick = std::move(_onClick);

            text = std::move(_text);

            int textWidth = MeasureText(text.text.c_str(), text.fontSize);
            text.posX = buttonX + (buttonWidth - textWidth) / 2;
            text.posY = buttonY + (buttonHeight - text.fontSize) / 2;
        }
    };

    class VesselsComponent {
    public:
        Model model{};
        bool drawable;
        std::string path;
        TextComponent name;

        VesselsComponent(Model _model, bool _drawable, std::string _path, TextComponent _name) {
            model = _model;
            drawable = _drawable;
            path = std::move(_path);
            name = std::move(_name);
        }
    };
}
