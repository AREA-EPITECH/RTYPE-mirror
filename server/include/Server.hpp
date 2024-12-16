/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Server
*/

#pragma once
#include <sys/types.h>
#if defined(_WIN32) || defined(WIN32)
#include <WinSock2.h>
#endif

namespace server {
    class Server {
        u_int _port{};
    public:
        explicit Server(u_int port);
        Server(const Server &other) = delete;
        auto operator=(const Server &other) -> auto & = delete;
        ~Server() = default;

        void initServer();
        [[noreturn]] void mainloop();
    };
} // namespace server