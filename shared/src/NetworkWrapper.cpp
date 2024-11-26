/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** NetworkWrapper.cpp
*/

#include "NetworkWrapper.hpp"
#include "spdlog/spdlog.h"

bool network::NetworkWrapper::initialized = false;

// Initialize ENet
bool network::NetworkWrapper::initialize() {
    if (initialized) return true;

    if (enet_initialize() != 0) {
        spdlog::error("Network initialization failed!");
        return false;
    }

    initialized = true;
    return true;
}

// Shutdown ENet
void network::NetworkWrapper::shutdown() {
    if (initialized) {
        enet_deinitialize();
        initialized = false;
    }
}

// Server Constructor
network::NetworkServer::NetworkServer(): host(nullptr) {
    NetworkWrapper::initialize();
}

network::NetworkServer::~NetworkServer() {
    stop();
    NetworkWrapper::shutdown();
}

// Start the server
bool network::NetworkServer::start(uint16_t port) {
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = port;

    host = enet_host_create(&address, 32, 2, 0, 0);
    if (!host) {
        spdlog::error("Failed to create server!");
        return false;
    }
    return true;
}

// Stop the server
void network::NetworkServer::stop() {
    if (host) {
        enet_host_destroy(host);
        host = nullptr;
    }
}

// Send a packet to a specific peer
bool network::NetworkServer::sendPacket(const std::vector<uint8_t>& data, ENetPeer* peer) {
    ENetPacket* packet = enet_packet_create(data.data(), data.size(), ENET_PACKET_FLAG_RELIABLE);
    return enet_peer_send(peer, 0, packet) >= 0;
}

// Process incoming events (handle connections, disconnections, data)
void network::NetworkServer::processEvents() {
    ENetEvent event;
    while (enet_host_service(host, &event, 1000) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                spdlog::info("A new client connected!");
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                spdlog::info("Received a packet of length: {}", event.packet->dataLength);
                // Process the packet (parse and handle)
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                spdlog::info("A client disconnected!");
                break;
        }
    }
}

// Client Constructor
network::NetworkClient::NetworkClient() : client(nullptr), serverPeer(nullptr) {
    NetworkWrapper::initialize();
}

network::NetworkClient::~NetworkClient() {
    disconnect();
    NetworkWrapper::shutdown();
}

// Connect to the server
bool network::NetworkClient::connectToServer(const std::string& host, uint16_t port) {
    ENetAddress address;
    enet_address_set_host(&address, host.c_str());
    address.port = port;

    client = enet_host_create(nullptr, 1, 2, 0, 0);
    if (!client) {
        spdlog::error("Failed to create client!");
        return false;
    }

    serverPeer = enet_host_connect(client, &address, 2, 0);
    if (!serverPeer) {
        spdlog::error("Failed to connect to server!");
        return false;
    }

    ENetEvent event;
    if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        spdlog::info("Connected to server!");
        return true;
    }

    return false;
}

// Disconnect from the server
void network::NetworkClient::disconnect() {
    if (client) {
        enet_host_destroy(client);
        client = nullptr;
        serverPeer = nullptr;
    }
}

// Send a packet to the server
bool network::NetworkClient::sendPacket(const std::vector<uint8_t>& data) {
    if (serverPeer) {
        ENetPacket* packet = enet_packet_create(data.data(), data.size(), ENET_PACKET_FLAG_RELIABLE);
        return enet_peer_send(serverPeer, 0, packet) >= 0;
    }
    return false;
}

// Process incoming events (handle server messages)
void network::NetworkClient::processEvents() {
    ENetEvent event;
    while (enet_host_service(client, &event, 1000) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                spdlog::info("Received a packet of length: {}", event.packet->dataLength);
                // Process the packet (parse and handle)
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                spdlog::info("Disconnected from server");
                break;
        }
    }
}
