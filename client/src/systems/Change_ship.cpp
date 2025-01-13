/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** Change_ship.cpp
*/

#include "Registry.hpp"
#include "ecs/Systems.hpp"
#include "ecs/Components.hpp"
#include "game/GameState.hpp"

static void send_update_to_server(Registry &ecs, int ship_id)
{
    auto gameState = getGameState(ecs);
    struct network::LobbyActionPacket packet;
    packet.actionType = network::LobbyActionType::ChangeShip;
    packet.shipId = ship_id;
    packet.roomId = gameState->get().getRoomId();
    packet.readyState = false;
    auto user = gameState->get().getUser();
    user.ship_id = ship_id;
    gameState->get().updateUser(user);
    ecs.run_event(packet);
}

void previous_ship(Registry &ecs)
{
    auto &models = ecs.get_components<ecs::VesselsComponent>();
    size_t current = -1;
    size_t to_change = 0;
    for (size_t i = 0; i < models.size(); i++)
    {
        if (models[i].has_value())
        {
            if (models[i].value().drawable)
                if (current == -1)
                {
                    to_change = i;
                    continue;
                }
                else
                {
                    models[current].value().drawable = true;
                    models[i].value().drawable = false;
                    send_update_to_server(ecs, models[current].value().ship_id);
                    return;
                }
            if (!models[i].value().drawable)
                current = i;
        }
    }
    if (current != -1 && models[current].has_value())
    {
        models[current].value().drawable = true;
        models[to_change].value().drawable = false;
        send_update_to_server(ecs, models[current].value().ship_id);
    }
}

void next_ship(Registry &ecs)
{
    auto &models = ecs.get_components<ecs::VesselsComponent>();
    size_t current = -1;
    size_t to_change = 0;

    for (size_t i = models.size(); i-- > 0;)
    {
        if (models[i].has_value())
        {
            if (models[i].value().drawable)
            {
                if (current == -1)
                {
                    to_change = i;
                    continue;
                }
                else
                {
                    models[current].value().drawable = true;
                    models[i].value().drawable = false;
                    send_update_to_server(ecs, models[current].value().ship_id);
                    return;
                }
            }
            if (!models[i].value().drawable)
                current = i;
        }
    }

    if (current != -1 && models[current].has_value())
    {
        models[current].value().drawable = true;
        models[to_change].value().drawable = false;
        send_update_to_server(ecs, models[current].value().ship_id);
    }
}
