/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** Init_ecs
*/

#include "Main.hpp"

/**
 * Init the ecs
 * @return
 */
Registry init_ecs()
{
    Registry ecs;
    ecs.register_component<ecs::Window>();
    ecs.register_component<ecs::GameComponent>();
    ecs.register_component<ecs::MapComponent>();
    ecs.register_component<ecs::Tile>();
    ecs.register_component<ecs::Money>();
    ecs.register_component<ecs::SelectorComponent>();
    ecs.register_component<ecs::Tower>();
    ecs.register_component<ecs::Shop>();
    ecs.register_component<ecs::TextureManager>();
    ecs.register_component<ecs::TextComponent>();

    ecs.register_event<ecs::CreateWindowEvent>();
    ecs.register_event<ecs::WindowOpenEvent>();
    ecs.register_event<ecs::WindowCloseEvent>();
    ecs.register_event<ecs::WindowDrawEvent>();
    ecs.register_event<ecs::ControlsEvent>();
    ecs.register_event<ecs::CreateTextEvent>();

    return ecs;
}
