#include "spdlog/spdlog.h"
#include "NetworkWrapper.hpp"

int main(int argc, char const *argv[])
{
    network::NetworkServer server;

    if (server.start(12345)) {
        spdlog::info("Start R-Type Server!");
        while (true) {
            server.processEvents();
        }
    } else {
        spdlog::error("Failed to start server.");
    }

    return 0;
}
