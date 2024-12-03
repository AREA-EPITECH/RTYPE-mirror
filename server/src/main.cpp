/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** main.cpp
*/

#include <atomic>
#include <csignal>
#include "NetworkWrapper.hpp"
#include "ThreadPool.hpp"
#include "spdlog/spdlog.h"
#include "Server.hpp"
#include <sstream>
#include <iostream>

std::atomic<bool> shutdownRequested(false);

void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        spdlog::info("Shutdown signal (Ctrl+C) received...");
        shutdownRequested.store(true);
    }
}

void runServer()
{
    network::NetworkServer server;

    if (!server.start(12345))
    {
        spdlog::error("Failed to start the server");
        return;
    }

    spdlog::info("Server running on port 12345... Press Ctrl+C to stop.");

    while (!shutdownRequested.load())
    {
        network::ServerEvent event;
        // Poll for server events
        while (server.pollEvent(event))
        {
            switch (event.type)
            {
            case network::ServerEventType::ClientConnect:
                spdlog::info("Client connected: {}", (void *)event.peer->getPeer().get());
                break;

            case network::ServerEventType::ClientDisconnect:
                spdlog::info("Client disconnected: {}", (void *)event.peer->getPeer().get());
                break;

            case network::ServerEventType::DataReceive:
                spdlog::info("Received data of size {} from {}", event.data.size(), (void *)event.peer->getPeer().get());
                break;
            }
        }

        // Add a small sleep to avoid busy-waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    spdlog::info("Shutting down server...");
    server.stop();
    spdlog::info("Server stopped.");
}

void handleClientData(ENetPeer* peer, const std::vector<uint8_t>& data) {
    //network::InputPacket input = network::Packet::deserializeInputPacket(data);
    spdlog::info("Processing data from client: {}", (void*)peer);
    // Process the data (game logic, etc.)
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
            switch (event.type) {
                case network::ServerEventType::ClientConnect:
                    spdlog::info("Client connected: {}", (void*)event.peer->getPeer().get());
                    break;

                case network::ServerEventType::ClientDisconnect:
                    spdlog::info("Client disconnected: {}", (void*)event.peer->getPeer().get());
                    break;

                case network::ServerEventType::DataReceive:
                    // Delegate data processing to the thread pool
                    threadPool.enqueueTask([peer = event.peer, data = std::move(event.data)] {
                        handleClientData(peer->getPeer().get(), data);
                    });
                    break;
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
