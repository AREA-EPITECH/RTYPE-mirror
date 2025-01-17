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
            for (auto &room : server.getWaitingRooms())
            {
                room->sendUpdateRoom(server);
            }
            last_lobby_snapshot = current_time;
        }
        elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_snapshot).count();
        if (elapsed_time >= 20)
        {
            for (auto it = server.getPlayingRooms().begin(); it != server.getPlayingRooms().end(); ) {
                if (!(*it)->isClientinsideRoom()) {
                    it->reset();
                    it = server.getPlayingRooms().erase(it);
                } else {
                    (*it)->run(elapsed_time);
                    if ((*it)->checkWin()) {
                        spdlog::info("Win ! Go to level {}", (*it)->getLevel() + 1);
                        (*it)->setLevel((*it)->getLevel() + 1, server);
                        std::string level_map = fmt::format("./server/levels/map{}.json", (*it)->getLevel());
                        if (!std::filesystem::exists(level_map)) {
                            spdlog::info("Game Over no more level");
                            struct network::ErrorPacket error_packet;
                            error_packet.message = std::string("Game over");
                            error_packet.type = network::ErrorType::NoMoreLevel;
                            auto &clients = (*it)->getClients();
                            for (int i = 0; i < clients.size(); i++)
                            {
                                if (clients[i].has_value())
                                {
                                    server.getServer().sendErrorPacket(error_packet, clients[i].value());
                                }
                            }
                            it->reset();
                            it = server.getWaitingRooms().erase(it);
                            it++;
                        }
                        continue;
                    }
                    if ((*it)->checkLose()) {
                        spdlog::info("Lose ! Go back to level {}", (*it)->getLevel());
                        (*it)->setLevel((*it)->getLevel(), server);
                        continue;
                    }
                    (*it)->sendUpdateRoom(server);
                    ++it;
                }
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
