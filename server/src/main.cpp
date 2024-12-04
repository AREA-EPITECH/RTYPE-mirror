/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** main.cpp
*/

#include <atomic>
#include <csignal>
#include <spdlog/spdlog.h>
#include <sstream>
#include <iostream>

#include "ThreadPool.hpp"
#include "network/NetworkWrapper.hpp"
#include "network/Server.hpp"

std::atomic<bool> shutdownRequested(false);

void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        spdlog::info("Shutdown signal (Ctrl+C) received...");
        shutdownRequested.store(true);
    }
}

void handleClientData(ENetPeer* peer, const std::vector<uint8_t>& data) {
    //network::InputPacket input = network::Packet::deserializeInputPacket(data);
    spdlog::info("Processing data from client: {}", (void*)peer);
    // Process the data (game logic, etc.)
}

std::string deserializeString(const std::vector<uint8_t> &data) {
    size_t offset = 0;
    // Deserialize the string length
    uint32_t length = 0;
    std::memcpy(&length, data.data() + offset, sizeof(length));
    offset += sizeof(length);

    // Deserialize the string content
    std::string str(reinterpret_cast<const char *>(data.data() + offset), length);
    offset += length;

    return str;
}


void runMultithreadedServer() {
    network::NetworkServer server;
    if (!server.start(12345)) {
        spdlog::error("Failed to start server");
        return;
    }

    spdlog::info("Server running on port 12345...");

    ThreadPool threadPool(std::thread::hardware_concurrency()); // Create thread pool

    while (!shutdownRequested.load()) {
        network::ServerEvent event;

        // Poll for events
        while (server.pollEvent(event)) {
            std::vector<uint8_t> receivedPacket;
            switch (event.type) {
                case network::ServerEventType::ClientConnect:
                    spdlog::info("Client connected: {}", (void*)event.peer->getPeer().get());
                    break;

                case network::ServerEventType::ClientDisconnect:
                    spdlog::info("Client disconnected: {}", (void*)event.peer->getPeer().get());
                    break;

                case network::ServerEventType::DataReceive: {
                    if (event.packetType == network::PacketType::RawPacket) {
                        receivedPacket = std::any_cast<std::vector<uint8_t>>(event.data);
                        spdlog::info("Received data: {} of size: {}", deserializeString(receivedPacket), receivedPacket.size());
                    } else {
                        spdlog::info("Received not raw data");
                    }
                    break;
                }
            }
        }

        // Small delay to avoid busy-waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    spdlog::info("Shutting down server...");
    server.stop();
}

int main() {
    // Register signal handler
    std::signal(SIGINT, signalHandler);

    // Run the server
    runMultithreadedServer();

    spdlog::info("Application exited cleanly.");
    return 0;
}
