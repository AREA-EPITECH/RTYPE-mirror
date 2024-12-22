#pragma once

#include "../DataType.hpp"

namespace ecs
{

    struct FocusComponent
    {
        WindowFocus focus;
    };

    class ButtonComponent
    {
    public:
        ButtonComponent(int _buttonWidth, int _buttonHeight, std::string _text, WindowFocus _focus,
                        std::function<void()> _onClick, std::function<int(int, int)> _dynamicX = nullptr,
                        std::function<int(int, int)> _dynamicY = nullptr, int _text_size = 12, int _border_width = 2,
                        Color _base_color_normal = {0, 0, 0, 100}, Color _text_color_normal = {0, 0, 0, 100},
                        Color _border_color_normal = {0, 0, 0, 100}, Color _base_color_focused = {0, 0, 0, 100},
                        Color _text_color_focused = {0, 0, 0, 100}, Color _border_color_focused = {0, 0, 0, 100});

        void updateButton(int screenWidth, int screenHeight);
        void drawButton(WindowFocus _focus) const;

    private:
        int buttonX = 0;
        int buttonY = 0;
        int buttonWidth;
        int buttonHeight;
        std::string text;
        std::function<void()> onClick;
        std::function<int(int, int)> dynamicX;
        std::function<int(int, int)> dynamicY;
        WindowFocus focus;

        int text_size;
        int border_width;

        // Normal
        Color base_color_normal;
        Color text_color_normal;
        Color border_color_normal;

        // Focused
        Color base_color_focused;
        Color text_color_focused;
        Color border_color_focused;
    };

    class TextInputComponent
    {
    public:
        Rectangle inputBox{};
        std::string text;
        std::string placeholder;
        bool isFocused{};
        size_t maxLength{};
        WindowFocus focus;

        std::function<int(int screenWidth, int screenHeight)> dynamicX;
        std::function<int(int screenWidth, int screenHeight)> dynamicY;

        int text_size;
        int border_width;
        int font_size;

        // Normal
        Color base_color_normal;
        Color text_color_normal;
        Color border_color_normal;

        // Focused
        Color base_color_focused;
        Color text_color_focused;
        Color border_color_focused;

        // Pressed
        Color base_color_pressed;
        Color text_color_pressed;
        Color border_color_pressed;

        explicit TextInputComponent(
            Rectangle _inputBox, WindowFocus _focus, std::string _defaultText = "", size_t _maxLength = 256,
            std::function<int(int screenWidth, int screenHeight)> _dynamicX = nullptr,
            std::function<int(int screenWidth, int screenHeight)> _dynamicY = nullptr, int _text_size = 12,
            int _border_width = 2, int _font_size = 20, Color _base_color_normal = {0, 0, 0, 100},
            Color _text_color_normal = {0, 0, 0, 100}, Color _border_color_normal = {0, 0, 0, 100},
            Color _base_color_focused = {0, 0, 0, 100}, Color _text_color_focused = {0, 0, 0, 100},
            Color _border_color_focused = {0, 0, 0, 100}, Color _base_color_pressed = {0, 0, 0, 100},
            Color _text_color_pressed = {0, 0, 0, 100}, Color _border_color_pressed = {0, 0, 0, 100});

        TextInputComponent() = default;

        void drawTextInput(int screenWidth, int screenHeight, WindowFocus _focus);

        void handleInput();
    };


    class ShowBoxComponent
    {
    public:
        Rectangle boxRect;
        std::string message;
        Color boxColor;
        Color textColor;
        bool isVisible;
        std::function<int(int screenWidth, int screenHeight)> dynamicX;
        std::function<int(int screenWidth, int screenHeight)> dynamicY;
        WindowFocus focus;

        TextInputComponent textInput;
        std::string closeButtonText;
        std::string continueButtonText;

        ShowBoxComponent(Rectangle _boxRect, std::string _message, Color _boxColor, Color _textColor,
                         WindowFocus _focus, std::string _textInput = "", std::string _closeButtonText = "Close",
                         std::string _continueButtonText = "Continue",
                         std::function<int(int screenWidth, int screenHeight)> _dynamicX = nullptr,
                         std::function<int(int screenWidth, int screenHeight)> _dynamicY = nullptr);

        bool draw(WindowFocus _focus);
        void handleClick(Vector2 mousePosition);
        void updateBox(int screenWidth, int screenHeight);
    };

    class ImageComponent
    {
    public:
        ImageComponent(const std::string &_imagePath, WindowFocus _focus,
                       std::function<int(int screenWidth, int screenHeight)> _dynamicX,
                       std::function<int(int screenWidth, int screenHeight)> _dynamicY, std::function<void()> _onClick,
                       float _width = 0.0f, float _height = 0.0f);

        void draw(int screenWidth, int screenHeight);
        void handleClick(Vector2 mousePosition, WindowFocus _focus);

        Texture2D texture;
        std::function<int(int screenWidth, int screenHeight)> dynamicX;
        std::function<int(int screenWidth, int screenHeight)> dynamicY;
        std::function<void()> onClick;
        float width;
        float height;
        WindowFocus focus;
    };
} // namespace ecs
