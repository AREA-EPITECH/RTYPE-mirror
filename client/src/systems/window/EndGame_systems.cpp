/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** End_game_systems
*/

#include "ecs/Systems.hpp"

namespace ecs {
    void draw_endgame_system(Registry &ecs, const WindowDrawEvent &) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLUE, 0.9f));
        EndDrawing();
    }

    void open_endgame_system(Registry &ecs, const WindowOpenEvent &) {
        kill_entities_with_component<ecs::ShowBoxComponent>(ecs);
        kill_entities_with_component<ecs::TextInputComponent>(ecs);
        kill_entities_with_component<ecs::ButtonComponent>(ecs);
        kill_entities_with_component<ecs::TextComponent>(ecs);
        kill_entities_with_component<ecs::CloseLeaderBoard>(ecs);
        kill_entities_with_component<ecs::VesselsComponent>(ecs);
        kill_entities_with_component<ecs::ProjectilesComponent>(ecs);
        kill_entities_with_component<ecs::ControllableComponent>(ecs);
        kill_entities_with_component<ecs::MenuText>(ecs);
        kill_entities_with_component<ecs::ShaderComponent>(ecs);
        kill_entities_with_component<ecs::CameraComponent>(ecs);
        kill_entities_with_component<ecs::LightComponent>(ecs);
        kill_entities_with_component<ecs::BackgroundComponent>(ecs);
        kill_entities_with_component<ecs::DecorElementComponent>(ecs);
        kill_entities_with_component<ecs::HealthBarComponent>(ecs);
        kill_entities_with_component<ecs::ParticleSystemComponent>(ecs);
        kill_entities_with_component<ecs::ImageComponent>(ecs);
        kill_entities_with_component<ecs::FilterComponent>(ecs);
    }

    void close_endgame_system(Registry &ecs, const WindowCloseEvent &) {
    }
}