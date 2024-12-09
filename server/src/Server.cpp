/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Server
*/

#include "Server.hpp"
#include <algorithm>
#include "Room.hpp"
#include "spdlog/spdlog.h"
#include <chrono>
#include <cstdint>
#include <enet/enet.h>
#include <memory>
#include <sys/types.h>
#include <thread>
#include <network/Server.hpp>
#include <string>
#include <utility>
#include <vector>
#include <ThreadPool.hpp>
#include "ClientData.hpp"

namespace server {
    /**
     * Server constructor
     * @param argv: commannd line argument corresponding to port number
     */
    Server::Server(char *argv[]) : _thread_pool(std::thread::hardware_concurrency()) {
        if (argv[1] == nullptr) {
            throw ServerException("Port argument is missing");
        }
        try {
            std::string port_string(argv[1]);
            this->_port = std::stoi(port_string);
        } catch ([[maybe_unused]] std::invalid_argument const& ex) {
            throw ServerException("Wrong port argument");
        }
    }

    /**
     * Init server with network ENet library
     * Port listening
     */
    void Server::initServer() {
        if (!this->_server.start(this->_port)) {
            throw ServerException("Failed to start server");
        }
        spdlog::info("Server running on port {}...", this->_port);
    }

    /**
     * Polling events that catch different type of Paquets received by ENet library
     * ClientConnect: a client connects to server
     * ClientDisconnect: a client disconnects from server
     * DataReceive: a client sends data to server
     */
    void Server::pollEvent() {
        network::ServerEvent event;
        while (this->_server.pollEvent(event)) {
            switch (event.type) {
                case network::ServerEventType::ClientConnect:
                    spdlog::info("Client connected: {}", static_cast<void *>(event.peer->getPeer().get()));
                break;

                case network::ServerEventType::ClientDisconnect:
                    spdlog::info("Client disconnected: {}", static_cast<void *>(event.peer->getPeer().get()));
                break;

                case network::ServerEventType::DataReceive:
                    // Delegate data processing to the thread pool
                    this->_thread_pool.enqueueTask([peer = event.peer, data = std::move(event.data), dataType = event.packetType] {
                        // Data managment + Send snapshotPaquet to client
                        handleClientData(peer, data, dataType);
                    });
                break;
            }
        }
        // Small delay to avoid busy-waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    /**
     * Client creates a room
     * @param client
     */
    void Server::createClientRoom(const std::shared_ptr<ENetPeer> &client) {
        // generate unique id
        Room room(this->_waiting_rooms.size());
        room._clients.push_back(client);
        this->_waiting_rooms.push_back(room);
        spdlog::info("Client {} created room {}", static_cast<ClientData*>(client->data)->getId(), room.getId());
    }

    /**
     * Assign a Client to a room when a client wants to join an existent room
     * @param client
     * @param room_id
     */
    void Server::assignClientToRoom(const std::shared_ptr<ENetPeer> &client, const uint8_t room_id) {
        Room room_iterator(room_id);
        if (std::find(this->_waiting_rooms.begin(), this->_waiting_rooms.end(), room_iterator) != this->_waiting_rooms.end()) {
            room_iterator._clients.push_back(client);
            spdlog::info("Client {} joined room {}", static_cast<ClientData*>(client->data)->getId(), room_id);
        } else {
            throw ServerException("Room doesn't exist");
        }
    }

    /**
     * A Client leaves a room
     * Deletes NetworkPeer from a room's clients vector
     * @param client
     * @param room_id
     */
    void Server::leaveClientRoom(std::shared_ptr<ENetPeer> &client, uint16_t room_id) {
        Room room_iterator(room_id);
        if (std::find(this->_waiting_rooms.begin(), this->_waiting_rooms.end(), room_iterator) != this->_waiting_rooms.end()) {
            if (std::find(room_iterator._clients.begin(), room_iterator._clients.end(), client) != room_iterator._clients.end()) {
                room_iterator._clients.erase(std::remove(room_iterator._clients.begin(), room_iterator._clients.end(), client), room_iterator._clients.end());
                spdlog::info("Client {} left room {}", static_cast<ClientData*>(client->data)->getId(), room_iterator.getId());
            } else {
                const std::string error = fmt::format("Client {} doesn't belong to room {}", static_cast<ClientData*>(client->data)->getId(), room_iterator.getId());
                throw ServerException(error.c_str());
            }
        } else {
            throw ServerException("Room doesn't exist");
        }
    }

    /**
     * Stop server
     */
    void Server::stopServer() {
        this->_server.stop();
        spdlog::info("Shutting down server...");
    }

    /**
     * ServerException custom exception constructor
     * @param msg : exception message to log
     */
    Server::ServerException::ServerException(const char *msg): _message(msg) {}

    /**
     * Overrides what() method
     * @return char * pointer
     */
    auto Server::ServerException::what() const noexcept -> const char* {
        return _message.c_str();
    }

    /**
     * Deserialize if RawPaquet is sent from a client
     * Called when DataReceive Paquet type is received
     * @param peer
     * @param data
     */
    void handleClientData(std::shared_ptr<network::PeerWrapper> peer, std::any data, network::PacketType type) {
        if (type == network::PacketType::InputPacket) {
            auto packet = std::any_cast<network::InputPacket>(data);
        }
    }
} // namespace server
