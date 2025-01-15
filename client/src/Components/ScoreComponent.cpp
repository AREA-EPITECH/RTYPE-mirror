/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** ScoreComponent
*/

#include "ecs/Components.hpp"

namespace ecs {

    void ScoreComponent::draw_ingame() {
        std::string scoreText = "Score : " + std::to_string(score) + " / " + std::to_string(win_score);

        int fontSize = 30;
        int textWidth = MeasureText(scoreText.c_str(), fontSize);

        int screenWidth = GetScreenWidth();
        int xPosition = (screenWidth - textWidth) / 2;
        int yPosition = 10;

        DrawText(scoreText.c_str(), xPosition, yPosition, fontSize, WHITE);
    }

}
