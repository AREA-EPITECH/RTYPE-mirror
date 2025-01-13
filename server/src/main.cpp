/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** main.cpp
*/

#include <atomic>
#include <chrono>
#include <csignal>
#include <chrono>
#include "spdlog/spdlog.h"
#include "Server.hpp"

std::atomic<bool> shutdown_requested(false);

/**
 * Handle signal
 * @param signal: SIGINT for CTRL+C
 */
static void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        spdlog::info("Shutdown signal (Ctrl+C) received...");
        shutdown_requested.store(true);
    }
}

static void runMainLoop(server::Server &server)
{
    auto last_lobby_snapshot = std::chrono::steady_clock::now();
    auto last_snapshot = std::chrono::steady_clock::now();
    while (!shutdown_requested.load())
    {
        server.pollEvent();
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_lobby_snapshot).count();
        if (elapsed_time >= 100)
        {
            for (auto room : server.getWaitingRooms())
            {
                room->sendUpdateRoom(server);
            }
            last_lobby_snapshot = current_time;
        }
        elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_snapshot).count();
        if (elapsed_time >= 20)
        {
            for (auto room : server.getPlayingRooms())
            {
                room->sendUpdateRoom(server);
            }
            last_snapshot = current_time;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

int main(int argc, char *argv[]) {
    std::signal(SIGINT, signalHandler);

    try {
        server::Server server(argv);
        server.initServer();
        runMainLoop(server);
        server.stopServer();
    } catch (server::Server::ServerException& e) {
        spdlog::error(e.what());
    }
    spdlog::info("Application exited cleanly.");
    return 0;
}
