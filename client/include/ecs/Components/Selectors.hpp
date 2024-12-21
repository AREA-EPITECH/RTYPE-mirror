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
                        std::function<int(int, int)> _dynamicY = nullptr, Color _buttonColor = GRAY);

        void updateButton(int screenWidth, int screenHeight);
        void drawButton(WindowFocus _focus);

    private:
        int buttonX = 0;
        int buttonY = 0;
        int buttonWidth;
        int buttonHeight;
        std::string text;
        std::function<void()> onClick;
        std::function<int(int, int)> dynamicX;
        std::function<int(int, int)> dynamicY;
        Color buttonColor;
        WindowFocus focus;
    };

    class TextInputComponent
    {
    public:
        Rectangle inputBox{};
        std::string text;
        std::string placeholder;
        Color boxColor{};
        Color textColor{};
        Color borderColor{};
        bool isFocused{};
        size_t maxLength{};
        WindowFocus focus;

        std::function<int(int screenWidth, int screenHeight)> dynamicX;
        std::function<int(int screenWidth, int screenHeight)> dynamicY;

        explicit TextInputComponent(Rectangle _inputBox, WindowFocus _focus, std::string _defaultText = "",
                                    size_t _maxLength = 256, Color _boxColor = BLUE, Color _textColor = BLACK,
                                    Color _borderColor = RED,
                                    std::function<int(int screenWidth, int screenHeight)> _dynamicX = nullptr,
                                    std::function<int(int screenWidth, int screenHeight)> _dynamicY = nullptr);

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
                                      int _minValue = 0, int _maxValue = 100, Color _boxColor = BLUE,
                                      Color _textColor = BLACK, Color _borderColor = RED,
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
        std::function<void()> onClose;
        std::function<void()> onContinue;
        WindowFocus focus;

        NumberInputComponent numberInput;
        std::string closeButtonText;
        std::string continueButtonText;

        ShowBoxComponent(Rectangle _boxRect, std::string _message, Color _boxColor, Color _textColor,
                         WindowFocus _focus, std::string _textInput = "", std::string _closeButtonText = "Close",
                         std::string _continueButtonText = "Continue", std::function<void()> _onClose = nullptr,
                         std::function<void()> _onContinue = nullptr,
                         std::function<int(int screenWidth, int screenHeight)> _dynamicX = nullptr,
                         std::function<int(int screenWidth, int screenHeight)> _dynamicY = nullptr);

        bool draw(WindowFocus _focus);
        void handleClick(Vector2 mousePosition);
        void updateBox(int screenWidth, int screenHeight);
    };
} // namespace ecs
