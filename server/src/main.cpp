#include <exception>
#include "spdlog/spdlog.h"
#include "Server.hpp"
#include <sstream>
#include <iostream>

int main(int argc, char const *argv[])
{
    try {
        const std::string s = argv[1];
        std::stringstream stream;
        stream << s;
        u_int port = 0;
        stream >> port;
        server::Server server(port);
        server.initServer();
    } catch (std::exception &e) {
        spdlog::error(e.what());
    }
    return 0;
}
