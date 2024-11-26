#include "spdlog/spdlog.h"
#include "NetworkWrapper.hpp"

int main(int argc, char const *argv[])
{
    network::NetworkClient client;

    if (client.connectToServer("localhost", 12345)) {
        spdlog::info("Start R-Type Client!");
        while (true) {
            client.processEvents();
        }
    } else {
        spdlog::error("Failed to connect to server.");
    }
    return 0;
}
