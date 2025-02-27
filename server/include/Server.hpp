/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Server
*/

#pragma once

#include <Room.hpp>
#include <ThreadPool.hpp>
#include <cstdint>
#include <enet/enet.h>
#include <exception>
#include <memory>
#include <network/PeerWrapper.hpp>
#include <network/Server.hpp>
#include <network/packet/PacketHeader.hpp>
#include <network/packet/ClientPacket.hpp>
#include <string>
#include <sys/types.h>
#include <vector>
#if defined(_WIN32) || defined(WIN32)
#include <WinSock2.h>
#endif

namespace server
{
    class Room;
    class Server
    {
        uint16_t _port{};
        network::NetworkServer _server;
        std::vector<std::shared_ptr<Room>> _waiting_rooms;
        std::vector<std::shared_ptr<Room>> _playing_rooms;
    public:
        explicit Server(char *argv[]);
        Server(const Server &other) = delete;
        auto operator=(const Server &other) -> auto & = delete;
        ~Server() = default;

        void initServer();
        void stopServer();

        void pollEvent();
        void updateRooms();

        void changeRoomToPlaying(uint32_t room_id);
        void changeRoomToWaiting(uint32_t room_id);

        void clientDisconnect(std::shared_ptr<network::PeerWrapper> &peer);
        void createClientRoom(std::shared_ptr<network::PeerWrapper> &client);
        bool assignClientToRoom(std::shared_ptr<network::PeerWrapper> &client, uint32_t room_id);
        void leaveClientRoom(std::shared_ptr<network::PeerWrapper> &client, uint32_t room_id);

        void moveActionRoom(uint32_t client_id, uint32_t room_id, network::MoveDirection type);
        void fireActionRoom(uint32_t client_id, uint32_t room_id, network::FireType type);

        std::vector<std::shared_ptr<Room>>& getWaitingRooms();
        std::vector<std::shared_ptr<Room>>& getPlayingRooms();
        std::vector<std::shared_ptr<Room>> getAllRooms();
        network::NetworkServer &getServer();

        // custom Exception
        class ServerException final : public std::exception
        {
            std::string _message;

        public:
            explicit ServerException(const char *msg);
            auto what() const noexcept -> const char * override;
        };
    };
    void handleClientData(Server &server, std::shared_ptr<network::PeerWrapper> &peer, const std::any &data,
        network::PacketType type);
} // namespace server
