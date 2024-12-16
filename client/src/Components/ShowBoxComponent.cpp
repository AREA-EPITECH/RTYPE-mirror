//
// Created by lferraro on 12/11/24.
//

#include "ecs/Components.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raylib/raygui.h"

namespace ecs {

    ShowBoxComponent::ShowBoxComponent(Rectangle _boxRect, std::string _message, Color _boxColor, Color _textColor,
                                       TextInputComponent _textInput, ButtonComponent _closeButton,
                                       ButtonComponent _continueButton)
            : boxRect(_boxRect),
              textInput(std::move(_textInput)),
              closeButton(std::move(_closeButton)),
              continueButton(std::move(_continueButton)),
              message(std::move(_message)),
              boxColor(_boxColor),
              textColor(_textColor),
              isVisible(false) {

        textInput.inputBox.width = _boxRect.width - 40;
        textInput.inputBox.height = 100;

        textInput.dynamicX = [_boxRect](int screenWidth, int screenHeight) {
            return static_cast<int>(_boxRect.x + 20);
        };

        textInput.dynamicY = [_boxRect](int screenWidth, int screenHeight) {
            return static_cast<int>(_boxRect.y + 100);
        };

        closeButton.buttonWidth = _boxRect.width / 2 - 40;
        closeButton.buttonHeight = 50;

        closeButton.dynamicX = [_boxRect](int screenWidth, int screenHeight) {
            return static_cast<int>(_boxRect.x + 20);
        };

        closeButton.dynamicY = [_boxRect](int screenWidth, int screenHeight) {
            return static_cast<int>(_boxRect.y + screenHeight - 70);
        };

        continueButton.buttonWidth = _boxRect.width / 2 - 40;
        continueButton.buttonHeight = 50;

        int w = continueButton.buttonWidth;
        continueButton.dynamicX = [_boxRect, w](int screenWidth, int screenHeight) {
            return static_cast<int>(_boxRect.x + screenWidth - w - 20);
        };

        continueButton.dynamicY = [_boxRect](int screenWidth, int screenHeight) {
            return static_cast<int>(_boxRect.y + screenHeight - 70);
        };
    }

    void ShowBoxComponent::draw() {
        if (!isVisible) return;

        textInput.handleInput();

        DrawRectangleRec(boxRect, boxColor);
        DrawText(message.c_str(),
                 static_cast<int>(boxRect.x + 10),
                 static_cast<int>(boxRect.y + 10),
                 20,
                 textColor);

        textInput.drawTextInput();

        closeButton.drawButton();
        continueButton.drawButton();
    }

    void ShowBoxComponent::handleClick(Vector2 mousePosition) {
        if (!isVisible) return;

        if (closeButton.isButtonPressed(mousePosition)) {
            isVisible = false;
        }

        if (continueButton.isButtonPressed(mousePosition)) {
            isVisible = false;
        }

    }

    void ShowBoxComponent::handleInput(char key) {
        if (!isVisible) return;

        textInput.handleInput();
    }

    void ShowBoxComponent::updateBox() {
        textInput.updateInput(boxRect.width, boxRect.height);
        closeButton.updateButton(boxRect.width, boxRect.height);
        continueButton.updateButton(boxRect.width, boxRect.height);
    }
}
