/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** main.cpp
*/

#include <atomic>
#include <csignal>
#include "spdlog/spdlog.h"
#include "Server.hpp"

std::atomic<bool> shutdown_requested(false);

static void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        spdlog::info("Shutdown signal (Ctrl+C) received...");
        shutdown_requested.store(true);
    }
}

static void runGameLoop(server::Server &server) {
    while (!shutdown_requested.load()) {
        server.pollEvent();
    }
}

int main(int argc, char *argv[]) {
    // Register signal handler
    std::signal(SIGINT, signalHandler);

    try {
        server::Server server(argv);
        server.initServer();
        runGameLoop(server);
        server.stopServer();
    } catch (server::Server::ServerException& e) {
        spdlog::error(e.what());
    }
    spdlog::info("Application exited cleanly.");
    return 0;
}
