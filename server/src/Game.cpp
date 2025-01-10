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
        const struct network::InputPacket &input_packet)
    {
    }
} // namespace server
