//
// Created by lferraro on 11/21/24.
//

#include <criterion/criterion.h>
#include "shared/include/Registry.hpp"

Test(Registry, spawn_entity) {
    Registry reg;
    auto entity1 = reg.spawn_entity();
    auto entity2 = reg.spawn_entity();
    cr_assert_neq(entity1, entity2, "Each entity should have a unique ID.");
}

Test(Registry, add_and_get_component) {
    Registry reg;
    reg.register_component<int>();
    auto entity = reg.spawn_entity();
    reg.add_component<int>(entity, 42);

    auto& components = reg.get_components<int>();
    cr_assert_eq(components[entity].value(), 42, "Component value should match the inserted value.");
}

Test(Registry, remove_component) {
    Registry reg;
    reg.register_component<int>();
    auto entity = reg.spawn_entity();
    reg.add_component<int>(entity, 99);
    reg.remove_component<int>(entity);

    auto& components = reg.get_components<int>();
    cr_assert_not(components[entity].has_value(), "Component should be removed from the entity.");
}

Test(Registry, kill_entity) {
    Registry reg;
    reg.register_component<int>();
    auto entity = reg.spawn_entity();
    reg.add_component<int>(entity, 1);
    reg.kill_entity(entity);

    auto& components = reg.get_components<int>();
    cr_assert_not(components[entity].has_value(), "Components of the killed entity should be erased.");
    cr_assert_throw(reg.kill_entity(entity), std::runtime_error, "Killing a non-existent entity should throw an exception.");
}

Test(Registry, entity_with_struct) {
    struct Position {
        float x, y;
    };

    struct Name {
        std::string name;
    };

    Registry reg;
    reg.register_component<Position>();
    reg.register_component<Name>();
    auto entity1 = reg.spawn_entity();
    auto entity2 = reg.spawn_entity();

    reg.add_component<Position>(entity1, {2.0, 1.5});
    reg.add_component<Position>(entity2, {9.0, -3.6});
    reg.add_component<Name>(entity2,{"bootstrap"});

    auto& pos = reg.get_components<Position>();
    auto& name = reg.get_components<Name>();

    float test_value = -3.6;

    cr_assert_eq(pos[entity1].value().x, 2.0, "1: Component value should match the inserted value.");
    cr_assert_eq(pos[entity1].value().y, 1.5, "2: Component value should match the inserted value.");
    cr_assert_eq(pos[1].value().x, 9.0, "3: Component value should match the inserted value.");
    cr_assert_eq(pos[1].value().y, test_value, "4: Component value should match the inserted value.");
    cr_assert_eq(name[entity2].value().name, "bootstrap", "5: Component value should match the inserted value.");
}

Test(Registry, add_component_many_times) {
    struct Position {
        float x, y;
    };

    struct Name {
        std::string name;
    };

    Registry reg;
    reg.register_component<Position>();
    reg.register_component<Name>();
    auto entity1 = reg.spawn_entity();
    auto entity2 = reg.spawn_entity();

    reg.add_component<Position>(entity1, {2.0, 1.5});
    reg.add_component<Position>(entity2, {9.0, -3.6});
    reg.add_component<Name>(entity2,{"bootstrap"});

    reg.add_component<Position>(entity1, {3.0, 12.0});
    reg.add_component<Position>(entity2, {3.0, 45.8});
    reg.add_component<Name>(entity2,{"New Name"});

    auto& pos = reg.get_components<Position>();
    auto& name = reg.get_components<Name>();

    float test_value = 45.8;

    cr_assert_eq(pos[entity1].value().x, 3.0, "1: Component value should match the inserted value.");
    cr_assert_eq(pos[entity1].value().y, 12.0, "2: Component value should match the inserted value.");
    cr_assert_eq(pos[1].value().x, 3.0, "3: Component value should match the inserted value.");
    cr_assert_eq(pos[1].value().y, test_value, "4: Component value should match the inserted value.");
    cr_assert_eq(name[entity2].value().name, "New Name", "5: Component value should match the inserted value.");
}
