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
    spdlog::info("ECS now empty.");
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
