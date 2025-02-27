/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Lobby
*/

#pragma once
#include <Server.hpp>
#include <network/PeerWrapper.hpp>
#include <network/packet/ClientPacket.hpp>

namespace server {
    void lobbyAction(Server &server, std::shared_ptr<network::PeerWrapper> &peer, const struct network::LobbyActionPacket &lobby_action_packet);
} // namespace server
