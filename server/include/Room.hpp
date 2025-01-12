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
        explicit Room(uint32_t id);
        ~Room();
        bool operator==(const Room& other) const;

        std::vector<network::LobbyPlayer> toLobbyPlayers() const;
        void sendUpdateRoom(Server &server);

        void addClient(std::shared_ptr<network::PeerWrapper> &peer);
        std::shared_ptr<network::PeerWrapper> getClient(uint32_t id);
        void removeClient(uint32_t id);

        bool getClientsReadiness() const;
        uint32_t getId() const;
        void setId(uint32_t id);
        network::LobbyGameState getState() const;
        void setState(network::LobbyGameState state);
    };
} // namespace server