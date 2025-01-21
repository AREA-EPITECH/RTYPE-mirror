/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** Game_components
*/

#include "ecs/Systems.hpp"

namespace ecs
{
    /**
     * @brief Create tower component
     * @param ecs
     * @param event
     */
    void create_tower_component(Registry &ecs, const CreateTowerEvent &event)
    {
        const auto entity = ecs.spawn_entity();
        ecs.add_component<Tower>(entity,
                                 {event._range, event._damage, event._fire_rate, std::chrono::steady_clock::now(),
                                  event._cost, event._name, event._texture, 0, 0.0f, event._tower_type,
                                  event._position});
    }

    /**
     * @brief Create text component
     * @param ecs
     * @param event
     */
    void create_text_component(Registry &ecs, const CreateTextEvent &event)
    {
        const auto entity = ecs.spawn_entity();
        ecs.add_component<TextComponent>(entity,
                                         {event._text,
                                          std::chrono::steady_clock::now(),
                                          event._duration,
                                          {event._position_x, event._position_y},
                                          {event._velocity_x, event._velocity_y},
                                          event._color,
                                          event._font_size});
    }

    /**
     * @brief Create enemy component
     * @param ecs
     * @param event
     */
    void create_enemy_component(Registry &ecs, const CreateEnemyEvent &event)
    {
        const auto entity = ecs.spawn_entity();
        ecs.add_component<EnemyComponent>(entity,
                                          {event._health, event._speed, event._damage, event._reward, event._texture,
                                           event._position, 0, 0.0f, std::chrono::steady_clock::now()});
    }
} // namespace ecs
