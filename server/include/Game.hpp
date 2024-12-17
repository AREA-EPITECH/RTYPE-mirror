/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Game
*/

#pragma once
#include <Server.hpp>
#include <network/PeerWrapper.hpp>
#include <network/packet/ClientPacket.hpp>

namespace server
{
    void gameAction(Server &server, std::shared_ptr<network::PeerWrapper> &peer,
                    const network::InputPacket &input_packet);
} // namespace server
