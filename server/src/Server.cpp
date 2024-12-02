/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Server
*/

#include "Server.hpp"
#include "spdlog/spdlog.h"

#include <exception>
#include <iostream>
#include <sys/types.h>

namespace server {
    Server::Server(u_int port) : _port(port) {}

    void Server::initServer() {
        try {
            spdlog::info("Init server...");
        } catch (std::exception &e) {
            spdlog::error(e.what());
        }
    }


    [[noreturn]] void Server::mainloop() {
        while (true) {
        }
    }

} // namespace server
