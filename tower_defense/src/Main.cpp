/*
** EPITECH PROJECT, 2024
** Main
** File description:
** Main
*/

#include "Main.hpp"

/**
 * @brief Empty ecs from all entities and components
 * @param ecs
 */
void empty_ecs(Registry &ecs)
{
    spdlog::info("Emptying ECS...");
    auto &maps = ecs.get_components<ecs::MapComponent>();
    auto &selectors = ecs.get_components<ecs::SelectorComponent>();
    auto &textures_managers = ecs.get_components<ecs::TextureManager>();

    for (auto &map: maps)
    {
        if (map.has_value())
        {
            UnloadTexture(map.value()._game._money._texture);
            UnloadTexture(map.value()._game._life._texture);
        }
    }

    for (auto &selector: selectors)
    {
        if (selector.has_value())
        {
            UnloadTexture(selector.value()._texture);
        }
    }

    for (auto &texture_manager : textures_managers)
    {
        if (texture_manager.has_value())
        {
            for (auto &texture : texture_manager.value()._textures)
            {
                UnloadTexture(*texture.second);
            }
            for (auto &decor_texture : texture_manager.value()._decors_textures)
            {
                UnloadTexture(*decor_texture);
            }
        }
    }

    kill_entities_with_component<ecs::MapComponent>(ecs);
    kill_entities_with_component<ecs::Shop>(ecs);
    kill_entities_with_component<ecs::SelectorComponent>(ecs);
    kill_entities_with_component<ecs::TextureManager>(ecs);
    spdlog::info("ECS emptied.");
}

/**
 * @brief Main function, creates network thread and start game loop
 * @param argc
 * @param argv
 * @return
 */
int main(const int argc, const char *argv[])
{
    if (argc != 2)
    {
        std::cout << "USAGE: ./tower_defense <MAP_FILENAME>" << std::endl;
        return 84;
    }
    const std::string map_filename = argv[1];

    Registry ecs = init_ecs();

    const auto windowEntity = ecs.spawn_entity();
    ecs.add_component<ecs::Window>(windowEntity, {1024, 1024, "R-Type - Tower Defense"});

    ecs.subscribe<ecs::CreateWindowEvent>(ecs::init_window_system);
    ecs.subscribe<ecs::ControlsEvent>(ecs::game_controls_system);

    ecs.run_event(ecs::CreateWindowEvent{});
    ecs.run_event(ecs::WindowOpenEvent{});

    tower_defense::Parser parser;
    parser.parse_filemap(map_filename);
    ecs = parser.fill_ecs(ecs);

    ecs.subscribe<ecs::WindowDrawEvent>(ecs::draw_game_system);

    while (!WindowShouldClose())
    {
        ecs.run_event(ecs::WindowDrawEvent{});
    }
    empty_ecs(ecs);
    CloseAudioDevice();

    return 0;
}
