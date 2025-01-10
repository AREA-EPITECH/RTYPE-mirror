/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Room
*/

#pragma once
#include <cstdint>
#include <memory>
#include <Server.hpp>
#include <enet/enet.h>
#include <vector>
#include <network/PeerWrapper.hpp>
#include <network/packet/ServerPacket.hpp>
#include <Registry.hpp>

namespace server {
    class Server;
    class Room {
        uint32_t _id;
        network::LobbyGameState _state;
        Registry _registry;
    public:
        std::vector<std::shared_ptr<network::PeerWrapper>> _clients;

        explicit Room();
        bool operator==(const Room& other) const;

        std::vector<network::LobbyPlayer> toLobbyPlayers() const;
        void sendUpdateRoom(Server &server);

        bool getClientsReadiness() const;
        uint32_t getId() const;
        void setId(uint32_t id);
        u_int8_t getSize() const;
        network::LobbyGameState getState() const;
        void setState(network::LobbyGameState state);
    };
} // namespace server