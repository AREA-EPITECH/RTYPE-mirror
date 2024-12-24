/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** ImageComponent
*/

#include "ecs/Components.hpp"

namespace ecs {
    ImageComponent::ImageComponent(const std::string& _imagePath, WindowFocus _focus,
                                   std::function<int(int screenWidth, int screenHeight)> _dynamicX,
                                   std::function<int(int screenWidth, int screenHeight)> _dynamicY,
                                   float _width, float _height, std::function<void()> _onClick)
            : dynamicX(std::move(_dynamicX)), dynamicY(std::move(_dynamicY)), onClick(std::move(_onClick)),
              width(_width), height(_height), focus(_focus) {
        texture = LoadTexture(_imagePath.c_str());

        if (width == 0.0f) width = static_cast<float>(texture.width);
        if (height == 0.0f) height = static_cast<float>(texture.height);
    }

    void ImageComponent::draw(int screenWidth, int screenHeight) {
        float x = dynamicX ? static_cast<float>(dynamicX(screenWidth, screenHeight)) : 0.0f;
        float y = dynamicY ? static_cast<float>(dynamicY(screenWidth, screenHeight)) : 0.0f;

        Rectangle sourceRect = {0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)};
        Rectangle destRect = {x, y, width, height};
        Vector2 origin = {0, 0};

        DrawTexturePro(texture, sourceRect, destRect, origin, 0.0f, WHITE);
    }

    void ImageComponent::handleClick(Vector2 mousePosition, WindowFocus _focus) {
        float x = dynamicX ? static_cast<float>(dynamicX(GetScreenWidth(), GetScreenHeight())) : 0.0f;
        float y = dynamicY ? static_cast<float>(dynamicY(GetScreenWidth(), GetScreenHeight())) : 0.0f;

        Rectangle imageRect = {x, y, width, height};

        if (CheckCollisionPointRec(mousePosition, imageRect) && onClick && focus == _focus) {
            onClick();
        }
    }
}
