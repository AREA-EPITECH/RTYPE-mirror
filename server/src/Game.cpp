/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Game
*/

#include "Game.hpp"
#include <ClientData.hpp>
#include <Server.hpp>
#include "spdlog/spdlog.h"

namespace server
{
    void gameAction(Server &server, std::shared_ptr<network::PeerWrapper> &peer,
        const struct network::InputPacket &input_packet) {
        const auto client_id = peer->getData<ClientData>().getId();
        const auto ecs_client = peer->getData<ClientData>().getRoom()->getClient(client_id);
        if (!ecs_client) {
            spdlog::error("Error client {} not in ecs", client_id);
            return;
        }
        server.moveActionRoom(client_id, ecs_client->getData<ClientData>().getRoom()->getId(), input_packet.move);
        server.fireActionRoom(client_id, ecs_client->getData<ClientData>().getRoom()->getId(), input_packet.fire);
    }
} // namespace server
