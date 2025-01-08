/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** TextInputComponent
*/

#include <utility>

#include "ecs/Components.hpp"

namespace ecs
{
    TextInputComponent::TextInputComponent(const Rectangle _inputBox, const WindowFocus _focus,
                                           std::string _defaultText, const size_t _maxLength,
                                           std::function<int(int screenWidth, int screenHeight)> _dynamicX,
                                           std::function<int(int screenWidth, int screenHeight)> _dynamicY,
                                           const int _text_size, const int _border_width, const int _font_size,
                                           const Color _base_color_normal, const Color _text_color_normal,
                                           const Color _border_color_normal, const Color _base_color_focused,
                                           const Color _text_color_focused, const Color _border_color_focused,
                                           const Color _base_color_pressed, const Color _text_color_pressed,
                                           const Color _border_color_pressed) :
        inputBox(_inputBox), placeholder(std::move(_defaultText)), maxLength(_maxLength), focus(_focus),
        dynamicX(std::move(_dynamicX)), dynamicY(std::move(_dynamicY)), text_size(_text_size),
        border_width(_border_width), font_size(_font_size), base_color_normal(_base_color_normal),
        text_color_normal(_text_color_normal), border_color_normal(_border_color_normal),
        base_color_focused(_base_color_focused), text_color_focused(_text_color_focused),
        border_color_focused(_border_color_focused), base_color_pressed(_base_color_pressed),
        text_color_pressed(_text_color_pressed), border_color_pressed(_border_color_pressed)
    {
    }

    void TextInputComponent::drawTextInput(const int screenWidth, const int screenHeight, const WindowFocus _focus)
    {
        if (dynamicX)
            inputBox.x = static_cast<float>(dynamicX(screenWidth, screenHeight));
        if (dynamicY)
            inputBox.y = static_cast<float>(dynamicY(screenWidth, screenHeight));

        int previousState = GuiGetState();

        if (focus != _focus)
        {
            GuiDisable();
        }

        GuiLoadStyleDefault();
        GuiSetStyle(DEFAULT, TEXT_SIZE, text_size);
        GuiSetStyle(DEFAULT, BORDER_WIDTH, border_width);

        // Normal
        GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(base_color_normal));
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(text_color_normal));
        GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(border_color_normal));

        // Focused
        GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, ColorToInt(base_color_focused));
        GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(text_color_focused));
        GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, ColorToInt(border_color_focused));

        // Pressed
        GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, ColorToInt(base_color_pressed));
        GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(text_color_pressed));
        GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED, ColorToInt(border_color_pressed));

        char buffer[512] = {0};
        strncpy(buffer, text.c_str(), maxLength);

        bool active = GuiTextBox(inputBox, buffer, maxLength, isFocused);

        if (focus == _focus)
        {
            text = std::string(buffer);
            if (active)
            {
                isFocused = true;
            }
        }

        if (!isFocused && text.empty())
        {
            Vector2 textSize = MeasureTextEx(GetFontDefault(), placeholder.c_str(), (float)font_size, 1);
            float textY = inputBox.y + (inputBox.height - textSize.y) / 2;

            DrawText(placeholder.c_str(), (int)inputBox.x + 20, (int)textY, font_size, GRAY);
        }

        GuiSetState(previousState);
    }

    void TextInputComponent::handleInput()
    {

        Vector2 mousePosition = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            isFocused = CheckCollisionPointRec(mousePosition, inputBox);
        }
    }


} // namespace ecs
