/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Lobby
*/

#include "Lobby.hpp"
#include <ClientData.hpp>
#include <Server.hpp>
#include "spdlog/spdlog.h"

namespace server
{
    void lobbyAction(Server &server, std::shared_ptr<network::PeerWrapper> &peer,
        const struct network::LobbyActionPacket &lobby_action_packet)
    {


        switch (lobby_action_packet.actionType)
        {
        case network::LobbyActionType::CreateRoom:
            server.createClientRoom(peer);
            break;
        case network::LobbyActionType::JoinRoom:
            server.assignClientToRoom(peer, lobby_action_packet.roomId);
            break;
        case network::LobbyActionType::LeaveRoom:
            server.leaveClientRoom(peer, lobby_action_packet.roomId);
        case network::LobbyActionType::ChangeName: {
            const auto client_data = peer->getData<ClientData>();
            if (client_data.getRoom()) {
                const auto ecs_client = client_data.getRoom() ->getClient(client_data.getId());
                if (ecs_client) {
                    ecs_client->getData<ClientData>().setName(lobby_action_packet.name);
                    spdlog::info("Client {} changed name to {}", ecs_client->getData<ClientData>().getId(), lobby_action_packet.name);
                }
            } else {
                peer->getData<ClientData>().setName(lobby_action_packet.name);
                spdlog::info("Client {} changed name to {}", peer->getData<ClientData>().getId(), lobby_action_packet.name);
            }
            break;
        }

        case network::LobbyActionType::ChangeShip: {
            const auto client_data = peer->getData<ClientData>();
            if (client_data.getRoom()) {
                const auto ecs_client = client_data.getRoom() ->getClient(client_data.getId());
                if (ecs_client) {
                    ecs_client->getData<ClientData>().setShip(lobby_action_packet.shipId);
                    spdlog::info("Client {} changed ship to {}", ecs_client->getData<ClientData>().getId(),
                    lobby_action_packet.shipId);
                }
            } else {
                peer->getData<ClientData>().setShip(lobby_action_packet.shipId);
                spdlog::info("Client {} changed ship to {}", peer->getData<ClientData>().getId(),
                    lobby_action_packet.shipId);
            }
            break;
        }
        case network::LobbyActionType::ChangeReady: {
            const auto client_data = peer->getData<ClientData>();
            if (client_data.getRoom()) {
                const auto ecs_client = client_data.getRoom() ->getClient(client_data.getId());
                if (ecs_client) {
                    ecs_client->getData<ClientData>().setReadyState();
                    spdlog::info("Client {} is ready to play", ecs_client->getData<ClientData>().getId());
                }
            } else {
                peer->getData<ClientData>().setReadyState();
                    spdlog::info("Client {} is ready to play", peer->getData<ClientData>().getId());
            }
            break;
        }
        default:
            break;
        }
    }
} // namespace server
