# Network Library

## Overview

This library provides a high-level interface for handling networking in your application using ENet. It includes components for both client and server-side communication, along with support for serialization, events, and packet handling.

It include the protocol to use for the R-Type game.

## Server Usage

The `NetworkServer` class facilitates server-side networking.

### Setting up the server

```cpp
#include "network/Server.hpp"

network::NetworkServer server;

if (!server.start(12345)) {
    std::cerr << "Failed to start server on port 12345!" << std::endl;
    return -1;
}

// Server is running and ready to accept connections.
```

### Handling server events

Use `pollEvent` to process incoming events like client connections or data reception.

```cpp
#include "network/event/ServerEvent.hpp"

network::ServerEvent event;

while (server.pollEvent(event)) {
    switch (event.type) {
        case network::ServerEventType::ClientConnect:
            std::cout << "Client connected!" << std::endl;
            break;
        case network::ServerEventType::DataReceive:
            std::cout << "Data received from client!" << std::endl;
            break;
        case network::ServerEventType::ClientDisconnect:
            std::cout << "Client disconnected!" << std::endl;
            break;
    }
}
```

### Sending packets to clients

```cpp
#include "network/packet/ServerPacket.hpp"

// Send snapshot packet defined by the protocol
network::SnapshotPacket snapshotPacket;
// Populate packet with data...
server.sendSnapshotPacket(snapshotPacket, event.peer);

// Send lobby snapshot packet defined by the protocol
network::LobbySnapshotPacket lobbySnapshotPaccket;
// Populate packet with data...
server.sendLobbyPacket(lobbySnapshotPaccket, event.peer);

// You can also send raw packet directly in binary
std::vector<uint8_t> packet;
// Populate packet with data...
server.sendPacket(packet, event.peer);
```

## Client Usage

The `NetworkClient` class provides a client-side interface.

### Connecting to a server

```cpp
#include "network/Client.hpp"

network::NetworkClient client;

if (!client.connectToServer("127.0.0.1", 12345)) {
    std::cerr << "Failed to connect to server!" << std::endl;
    return -1;
}
```

### Handling client events

Use `pollEvent` to process incoming events like data reception or server disconnection.

```cpp
#include "network/event/ClientEvent.hpp"

network::ClientEvent event;

while (client.pollEvent(event)) {
    switch (event.type) {
        case network::ClientEventType::DataReceive:
            std::cout << "Data received from server!" << std::endl;
            break;
        case network::ClientEventType::ServerDisconnect:
            std::cout << "Disconnected from server!" << std::endl;
            break;
    }
}
```

### Sending packets to the server

```cpp
#include "network/packet/ClientPacket.hpp"

// Send input packet defined by the protocol
network::InputPacket inputPacket;
// Populate packet with data...
client.sendInputPacket(inputPacket);

// Send lobby action packet defined by the protocol
network::LobbyActionPacket lobbyActionPacket;
// Populate packet with data...
client.sendLobbyPacket(lobbyActionPacket);

// You can also send raw packet directly in binary
std::vector<uint8_t> packet;
// Populate packet with data...
client.sendPacket(packet);
```

## Packet Serialization and Deserialization

### Serialization

Use the `Packet` class to serialize packets before sending them.

```cpp
#include "network/Packet.hpp"

// Serialize Snapshot Packet
std::vector<uint8_t> serializedData = network::Packet::serializeSnapshotPacket(packet);

// Serialize Input Packet
std::vector<uint8_t> serializedData = network::Packet::serializeInputPacket(packet);

// Serialize Lobby Action Packet
std::vector<uint8_t> serializedData = network::Packet::serializeLobbyActionPacket(packet);

// Serialize Lobby Snapshot Packet
std::vector<uint8_t> serializedData = network::Packet::serializeLobbySnapshotPacket(packet);
```

### Deserialization

Deserialize packets when receiving raw data.

```cpp
#include "network/Packet.hpp"

auto [packetType, packetData] = network::Packet::deserializePacket(rawData);
```

## Events

The library uses `ServerEvent` and `ClientEvent` structs to handle events in a flexible way. Both support storing packet data as `std::any`.

### Accessing event data

```cpp
if (event.packetType == network::PacketType::SnapshotPacket) {
    auto snapshotPacket = std::any_cast<network::SnapshotPacket>(event.data);
    // Process snapshot packet...
}
```

## Advanced Usage

### Custom Data with PeerWrapper

You can associate custom data with peers using `PeerWrapper`.

```cpp
event.peer->setData<std::string>("Player123");
std::string playerName = event.peer->getData<std::string>();
```

## Error Handling

- Check return values of key methods like `start`, `connectToServer`, and `sendPacket`.
- Use `spdlog` for logging errors and debugging information.

```cpp
#include <spdlog/spdlog.h>

spdlog::error("Failed to send packet: {}", errorCode);
```
