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
        switch (input_packet.move)
        {
            case network::MoveDirection::DownDirection:
                break;
            case network::MoveDirection::UpDirection:
                break;
            case network::MoveDirection::LeftDirection:
                break;
            case network::MoveDirection::RightDirection:
                break;
            case network::MoveDirection::NoneDirection:
            default:
                break;
        }

        switch (input_packet.fire)
        {
            case network::FireType::ChargedFire:
                break;
            case network::FireType::NormalFire:
                break;
            case network::FireType::NoneFire:
            default:
                break;
        }
    }
} // namespace server
