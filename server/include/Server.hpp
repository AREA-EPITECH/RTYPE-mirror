/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Server
*/

#pragma once
#include <cstdint>
#include <enet/enet.h>
#include <exception>
#include <memory>
#include <string>
#include <vector>
#include <sys/types.h>
#include <Room.hpp>
#include <ThreadPool.hpp>
#include <network/PeerWrapper.hpp>
#include <network/Server.hpp>
#include <network/packet/PacketHeader.hpp>

namespace server {
    class Server {
        uint16_t _port{};
        network::NetworkServer _server;
        std::vector<Room> _waiting_rooms;
        std::vector<Room> _playing_rooms;
        ThreadPool _thread_pool;
    public:
        explicit Server(char *argv[]);
        Server(const Server &other) = delete;
        auto operator=(const Server &other) -> auto & = delete;
        ~Server() = default;

        void initServer();
        void stopServer();

        void pollEvent();
        void createClientRoom(const std::shared_ptr<ENetPeer> &client);
        void assignClientToRoom(const std::shared_ptr<ENetPeer> &client, uint8_t room_id);
        void leaveClientRoom(std::shared_ptr<ENetPeer> &client, uint16_t room_id);
        void changeClientName(std::shared_ptr<ENetPeer> &client, uint16_t room_id);

        // custom Exception
        class ServerException final : public std::exception {
            std::string _message;
        public:
            explicit ServerException(const char* msg);
            auto what() const noexcept -> const char* override;
        };
    };
    void handleClientData(std::shared_ptr<network::PeerWrapper> peer, std::any data, network::PacketType type);
} // namespace server