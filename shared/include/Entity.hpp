#pragma once

#include <cstddef>
#include <iostream>

class Entity {
private:
    size_t id;

public:
    explicit Entity(size_t value) : id(value) {}

    operator size_t() const {
        return id;
    }

    ~Entity() = default;

    Entity() = delete;

    Entity& operator=(size_t) = delete;
};
