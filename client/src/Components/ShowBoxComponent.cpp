/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** ShowBoxComponent
*/

#include <utility>

#include "ecs/Components.hpp"

namespace ecs
{
    ShowBoxComponent::ShowBoxComponent(Rectangle _boxRect, std::string _message, Color _boxColor, Color _textColor,
                                       WindowFocus _focus, std::string _textInput, std::string _closeButtonText,
                                       std::string _continueButtonText, std::function<void()> _onClose,
                                       std::function<void()> _onContinue,
                                       std::function<int(int screenWidth, int screenHeight)> _dynamicX,
                                       std::function<int(int screenWidth, int screenHeight)> _dynamicY) :
        boxRect(_boxRect), message(std::move(_message)), boxColor(_boxColor), textColor(_textColor), isVisible(false),
        onClose(std::move(_onClose)), onContinue(std::move(_onContinue)), dynamicX(std::move(_dynamicX)),
        dynamicY(std::move(_dynamicY)), focus(_focus), closeButtonText(std::move(_closeButtonText)),
        continueButtonText(std::move(_continueButtonText))
    {
        numberInput.placeholder = "";
        numberInput.focus = _focus;
        numberInput.minValue = 0;
        numberInput.maxValue = 100;
        numberInput.value = numberInput.minValue;
        if (dynamicX)
            boxRect.x = dynamicX(GetScreenWidth(), GetScreenHeight());
        if (dynamicY)
            boxRect.y = dynamicY(GetScreenWidth(), GetScreenHeight());

        numberInput.dynamicX = [](int screenWidth, int screenHeight) { return static_cast<int>(screenWidth + 10); };
        numberInput.dynamicY = [](int screenWidth, int screenHeight) { return static_cast<int>(screenHeight + 40); };

        numberInput.inputBox = {boxRect.x + 10, boxRect.y + 40, boxRect.width - 20, 30};
    }

    bool ShowBoxComponent::draw(WindowFocus _focus)
    {
        if (!isVisible)
            return false;

        int previousState = GuiGetState();

        if (focus != _focus)
        {
            GuiDisable();
        }

        if (dynamicX)
            boxRect.x = dynamicX(GetScreenWidth(), GetScreenHeight());
        if (dynamicY)
            boxRect.y = dynamicY(GetScreenWidth(), GetScreenHeight());

        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
        GuiPanel(boxRect, message.c_str());

        numberInput.drawNumberInput(boxRect.x, boxRect.y, focus);
        numberInput.handleInput();

        Rectangle closeButtonRect = {boxRect.x + 10, boxRect.y + boxRect.height - 50, (boxRect.width - 30) / 2, 40};
        Rectangle continueButtonRect = {closeButtonRect.x + closeButtonRect.width + 10, closeButtonRect.y,
                                        closeButtonRect.width, closeButtonRect.height};

        if (GuiButton(closeButtonRect, closeButtonText.c_str()))
        {
            if (onClose)
                onClose();
            isVisible = false;
            return false;
        }

        if (GuiButton(continueButtonRect, continueButtonText.c_str()))
        {
            if (onContinue)
                onContinue();
            isVisible = false;
            return false;
        }
        GuiSetState(previousState);
        return true;
    }

    void ShowBoxComponent::handleClick(Vector2 mousePosition) {}

    void ShowBoxComponent::updateBox(int screenWidth, int screenHeight)
    {
        if (dynamicX)
            boxRect.x = dynamicX(screenWidth, screenHeight);
        if (dynamicY)
            boxRect.y = dynamicY(screenWidth, screenHeight);

        numberInput.inputBox = {boxRect.x + 10, boxRect.y + 40, boxRect.width - 20, 30};
    }
} // namespace ecs
