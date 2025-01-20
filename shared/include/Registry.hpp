#pragma once

#include <any>
#include <functional>
#include <optional>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include "Sparse_array.hpp"
#include "iostream"

using entity_t = std::size_t;

class Registry {
public:

    template <typename Component>
    Sparse_array<Component>& get_components() {
        auto type_idx = std::type_index(typeid(Component));
        auto it = _components_arrays.find(type_idx);
        if (it == _components_arrays.end())
            throw std::runtime_error("Component type not registered.");

        return std::any_cast<Sparse_array<Component>&>(it->second);
    }

    template <typename Component>
    const Sparse_array<Component>& get_components() const {
        auto type_idx = std::type_index(typeid(Component));
        auto it = _components_arrays.find(type_idx);
        if (it == _components_arrays.end())
            throw std::runtime_error("Component type not registered.");

        return std::any_cast<const Sparse_array<Component>&>(it->second);
    }

    template <typename Component>
    Sparse_array<Component>& register_component() {
        auto type_idx = std::type_index(typeid(Component));
        if (_components_arrays.find(type_idx) != _components_arrays.end())
            throw std::runtime_error("Component type already registered.");

        _components_arrays[type_idx] = Sparse_array<Component>();

        _erase_functions[type_idx] = [](Registry& reg, entity_t const& e) {
            reg.get_components<Component>().erase(e);
        };

        return std::any_cast<Sparse_array<Component>&>(_components_arrays[type_idx]);
    }

    template <typename Component>
    void unregister_component() {
        auto type_idx = std::type_index(typeid(Component));
        auto it = _components_arrays.find(type_idx);
        if (it == _components_arrays.end())
            throw std::runtime_error("Component type not registered.");

        _components_arrays.erase(type_idx);
        _erase_functions.erase(type_idx);
    }

    template <typename Component>
    typename Sparse_array<Component>::reference_type add_component(entity_t const& to, Component&& c) {
        return get_components<Component>().insert_at(to, std::forward<Component>(c));
    }

    template <typename Component, typename... Params>
    typename Sparse_array<Component>::reference_type emplace_component(entity_t const& to, Params&&... p) {
        return get_components<Component>().emplace_at(to, std::forward<Params>(p)...);
    }

    template <typename Component>
    void remove_component(entity_t const& from) {
        get_components<Component>().erase(from);
    }

    template <typename Component>
    void delete_component() {
        auto type_idx = std::type_index(typeid(Component));
        auto it = _components_arrays.find(type_idx);

        if (it != _components_arrays.end()) {
            _components_arrays.erase(type_idx);
            _erase_functions.erase(type_idx);
        }
    }

    entity_t spawn_entity() {
        entity_t entity;
        if (!_dead_entities.empty()) {
            entity = _dead_entities.back();
            _dead_entities.pop_back();
        } else {
            entity = _next_entity_id++;
        }

        _entity_to_index[entity] = entity;

        return entity;
    }

    void kill_entity(entity_t const& e) {
        if (_entity_to_index.find(e) == _entity_to_index.end())
            throw std::runtime_error("Entity not found");

        for (auto& [type_idx, func] : _erase_functions)
            func(*this, e);

        _entity_to_index.erase(e);
        _dead_entities.push_back(e);
    }

    template <typename Event>
    using system_type = std::function<void(Registry&, const Event&)>;

    template <typename Event>
    using systems_type = std::vector<system_type<Event>>;

    template <typename Event>
    void register_event() {
        auto event_idx = std::type_index(typeid(Event));
        if (_event_systems.find(event_idx) == _event_systems.end()) {
            _event_systems[event_idx] = std::vector<system_type<Event>>{};
        }
    }

    template <typename Event>
    auto& get_event_systems() {
        auto event_idx = std::type_index(typeid(Event));
        return std::any_cast<std::vector<system_type<Event>>&>(_event_systems[event_idx]);
    }

    template <typename Event, typename System>
    void subscribe(System&& system) {
        get_event_systems<Event>().emplace_back(std::forward<System>(system));
    }

    template <typename Event>
    void run_event(const Event& event) {
        auto& systems = get_event_systems<Event>();
        for (auto& system : systems)
            system(*this, event);
    }

    template <typename Event>
    void unsubscribe_all() {
        auto event_idx = std::type_index(typeid(Event));
        auto it = _event_systems.find(event_idx);
        if (it != _event_systems.end()) {
            it->second = std::vector<system_type<Event>>{};
        }
    }

private:
    std::unordered_map<std::type_index, std::any> _components_arrays;
    std::unordered_map<std::type_index, std::function<void(Registry&, entity_t const&)>> _erase_functions;
    std::vector<entity_t> _dead_entities;
    entity_t _next_entity_id = 0;
    std::unordered_map<entity_t, std::size_t> _entity_to_index;
    std::vector<std::function<void()>> _systems;
    std::unordered_map<std::type_index, std::any> _event_systems;
};

template <typename ComponentType>
void kill_entities_with_component(Registry& ecs) {
    auto& components = ecs.get_components<ComponentType>();
    for (std::size_t i = 0; i < components.size(); ++i) {
        if (components[i].has_value()) {
            ecs.kill_entity(i);
        }
    }
}
