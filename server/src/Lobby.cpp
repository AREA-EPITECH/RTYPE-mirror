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

namespace server {
    void lobbyAction(Server &server, const std::shared_ptr<network::PeerWrapper> &peer, const network::LobbyActionPacket &lobby_action_packet) {
        switch (lobby_action_packet.actionType) {
            case network::LobbyActionType::CreateRoom:
                server.createClientRoom(peer);
                break;
            case network::LobbyActionType::JoinRoom:
                server.assignClientToRoom(peer, lobby_action_packet.roomId);
                break;
            case network::LobbyActionType::LeaveRoom:
                server.leaveClientRoom(peer, lobby_action_packet.roomId);
            case network::LobbyActionType::ChangeName:
                peer->getData<ClientData>().setName(lobby_action_packet.name);
                spdlog::info("Client {} changed name to {}", peer->getData<ClientData>().getId(), lobby_action_packet.name);
                break;
            case network::LobbyActionType::ChangeShip:
                peer->getData<ClientData>().setShip(lobby_action_packet.shipId);
                spdlog::info("Client {} changed ship to {}", peer->getData<ClientData>().getId(), lobby_action_packet.shipId);
                break;
            case network::LobbyActionType::ChangeReady:
                peer->getData<ClientData>().setReadyState();
                spdlog::info("Client {} is ready to play", peer->getData<ClientData>().getId());
                break;
            default:
                break;
        }
    }
}
