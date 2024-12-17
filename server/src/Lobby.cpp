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
                     const network::LobbyActionPacket &lobby_action_packet)
    {
        network::LobbySnapshotPacket lobby_snapshot_packet;
        switch (lobby_action_packet.actionType)
        {
        case network::LobbyActionType::CreateRoom:
            server.createClientRoom(peer);
            lobby_snapshot_packet.roomId = peer->getData<ClientData>().getRoom()->getId();
            lobby_snapshot_packet.gameState = peer->getData<ClientData>().getRoom()->getState();
            server.getServer().sendLobbyPacket(lobby_snapshot_packet, peer);
            break;
        case network::LobbyActionType::JoinRoom:
            server.assignClientToRoom(peer, lobby_action_packet.roomId);
            lobby_snapshot_packet.roomId = lobby_action_packet.roomId;
            lobby_snapshot_packet.gameState = peer->getData<ClientData>().getRoom()->getState();
            server.getServer().sendLobbyPacket(lobby_snapshot_packet, peer);
            break;
        case network::LobbyActionType::LeaveRoom:
            server.leaveClientRoom(peer, lobby_action_packet.roomId);
        case network::LobbyActionType::ChangeName:
            peer->getData<ClientData>().setName(lobby_action_packet.name);
            spdlog::info("Client {} changed name to {}", peer->getData<ClientData>().getId(), lobby_action_packet.name);
            break;
        case network::LobbyActionType::ChangeShip:
            peer->getData<ClientData>().setShip(lobby_action_packet.shipId);
            spdlog::info("Client {} changed ship to {}", peer->getData<ClientData>().getId(),
                         lobby_action_packet.shipId);
            break;
        case network::LobbyActionType::ChangeReady:
            peer->getData<ClientData>().setReadyState();
            spdlog::info("Client {} is ready to play", peer->getData<ClientData>().getId());
            break;
        default:
            break;
        }
    }
} // namespace server
