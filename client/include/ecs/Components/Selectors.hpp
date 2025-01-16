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
        void setText(const std::string &new_text);
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

    class CloseLeaderBoard : public ButtonComponent
    {
    public:
        CloseLeaderBoard(int _buttonWidth, int _buttonHeight, std::string _text, WindowFocus _focus,
                         std::function<void()> _onClick, std::function<int(int, int)> _dynamicX = nullptr,
                         std::function<int(int, int)> _dynamicY = nullptr, int _text_size = 12, int _border_width = 2,
                         Color _base_color_normal = {0, 0, 0, 100}, Color _text_color_normal = {0, 0, 0, 100},
                         Color _border_color_normal = {0, 0, 0, 100}, Color _base_color_focused = {0, 0, 0, 100},
                         Color _text_color_focused = {0, 0, 0, 100}, Color _border_color_focused = {0, 0, 0, 100}) :
            ButtonComponent(_buttonWidth, _buttonHeight, _text, _focus, _onClick, _dynamicX, _dynamicY, _text_size,
                            _border_width, _base_color_normal, _text_color_normal, _border_color_normal,
                            _base_color_focused, _text_color_focused, _border_color_focused)
        {
        }
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

    class NumberInputComponent
    {
    public:
        Rectangle inputBox{};
        int value;
        int minValue;
        int maxValue;
        std::string placeholder;
        Color boxColor{};
        Color textColor{};
        Color borderColor{};
        bool isFocused{};
        WindowFocus focus;

        std::function<int(int screenWidth, int screenHeight)> dynamicX;
        std::function<int(int screenWidth, int screenHeight)> dynamicY;

        explicit NumberInputComponent(Rectangle _inputBox, WindowFocus _focus, std::string _defaultText = "",
                                      int _minValue = 0, int _maxValue = 100, Color _boxColor = {0, 0, 0, 100},
                                      Color _textColor = {0, 0, 0, 100}, Color _borderColor = {0, 0, 0, 100},
                                      std::function<int(int screenWidth, int screenHeight)> _dynamicX = nullptr,
                                      std::function<int(int screenWidth, int screenHeight)> _dynamicY = nullptr);

        NumberInputComponent() = default;

        void drawNumberInput(int screenWidth, int screenHeight, WindowFocus _focus);

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
        std::function<void()> continue_func;
        std::function<void()> close_func;
        WindowFocus focus;

        NumberInputComponent numberInput;
        std::string closeButtonText;
        std::string continueButtonText;

        explicit ShowBoxComponent(Rectangle _boxRect, std::string _message, Color _boxColor, Color _textColor,
                                  WindowFocus _focus, std::string _textInput = "",
                                  std::string _closeButtonText = "Close", std::string _continueButtonText = "Continue",
                                  std::function<int(int screenWidth, int screenHeight)> _dynamicX = nullptr,
                                  std::function<int(int screenWidth, int screenHeight)> _dynamicY = nullptr,
                                  std::function<void()> _continue_func = nullptr,
                                  std::function<void()> _close_func = nullptr);

        ShowBoxComponent() = default;

        void draw(WindowFocus _focus);
        void handleClick(Vector2 mousePosition);
        void updateBox(int screenWidth, int screenHeight);
    };

    class ImageComponent
    {
    public:
        ImageComponent(const std::string &_imagePath, WindowFocus _focus,
                       std::function<int(int screenWidth, int screenHeight)> _dynamicX,
                       std::function<int(int screenWidth, int screenHeight)> _dynamicY, float _width = 0.0f,
                       float _height = 0.0f, std::function<void()> _onClick = nullptr);

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
