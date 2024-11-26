//
// Created by lferraro on 11/24/24.
//

#include <criterion/criterion.h>
#include "shared/include/Registry.hpp"

struct Position {
    int x;
    int y;
};

struct Velocity {
    int x;
    int y;
};

struct UpdateEvent {
    int mul;
};

struct ParamEvent {
    int res;

    ParamEvent(int a, int b, int c) {
        res = a + b + c;
    }
};

void update_pos_system(Registry &r, const UpdateEvent &) {
    auto &positions = r.get_components<Position>();
    auto &velocity = r.get_components<Velocity>();

    for (int i = 0; i < positions.size() && i < velocity.size(); ++i) {
        if (velocity[i] && positions[i]) {
            auto &pos = positions[i].value();
            auto &vel = velocity[i].value();

            pos.x += vel.x;
            pos.y += vel.y;
        }
    }
}

void modify_pos_system(Registry &r, const UpdateEvent &event) {
    auto &positions = r.get_components<Position>();
    auto &velocity = r.get_components<Velocity>();

    for (int i = 0; i < positions.size() && i < velocity.size(); ++i) {
        if (velocity[i] && positions[i]) {
            auto &pos = positions[i].value();
            auto &vel = velocity[i].value();

            pos.x += 10 * event.mul;
            pos.y += 10 * event.mul;
        }
    }
}

Test(Systems, simple_system_event) {
    Registry reg;

    reg.register_component<Position>();
    reg.register_component<Velocity>();

    reg.register_event<UpdateEvent>();

    reg.subscribe<UpdateEvent>(update_pos_system);

    auto entity = reg.spawn_entity();

    reg.add_component<Position>(entity, {200, 100});
    reg.add_component<Velocity>(entity, {50, 30});

    auto component = reg.get_components<Position>();

    cr_assert_eq(component[entity].value().x, 200, "before system_event value x");
    cr_assert_eq(component[entity].value().y, 100, "before system_event value y");

    reg.run_event(UpdateEvent{0});

    component = reg.get_components<Position>();

    cr_assert_eq(component[entity].value().x, 250, "system_event value x");
    cr_assert_eq(component[entity].value().y, 130, "system_event value y");

}

Test(Systems, system_with_many_entity) {
    Registry reg;

    reg.register_component<Position>();
    reg.register_component<Velocity>();

    reg.register_event<UpdateEvent>();

    reg.subscribe<UpdateEvent>(update_pos_system);

    auto entity1 = reg.spawn_entity();
    auto entity2 = reg.spawn_entity();

    reg.add_component<Position>(entity1, {200, 100});
    reg.add_component<Velocity>(entity1, {50, 30});
    reg.add_component<Position>(entity2, {500, 50});
    reg.add_component<Velocity>(entity2, {10, 0});

    auto component = reg.get_components<Position>();

    cr_assert_eq(component[entity1].value().x, 200, "before system_event value entity1 x");
    cr_assert_eq(component[entity1].value().y, 100, "before system_event value entity1 y");
    cr_assert_eq(component[entity2].value().x, 500, "before system_event value entity2 x");
    cr_assert_eq(component[entity2].value().y, 50, "before system_event value entity2 y");

    reg.run_event(UpdateEvent{0});

    component = reg.get_components<Position>();

    cr_assert_eq(component[entity1].value().x, 250, "system_event value entity1 x");
    cr_assert_eq(component[entity1].value().y, 130, "system_event value entity1 y");
    cr_assert_eq(component[entity2].value().x, 510, "system_event value entity2 x");
    cr_assert_eq(component[entity2].value().y, 50, "system_event value entity2 y");
}

Test(Systems, system_with_many_entity_and_kill_entity) {
    Registry reg;

    reg.register_component<Position>();
    reg.register_component<Velocity>();

    reg.register_event<UpdateEvent>();

    reg.subscribe<UpdateEvent>(update_pos_system);

    auto entity1 = reg.spawn_entity();
    auto entity2 = reg.spawn_entity();
    auto entity3 = reg.spawn_entity();

    reg.add_component<Position>(entity1, {200, 100});
    reg.add_component<Velocity>(entity1, {50, 30});
    reg.add_component<Position>(entity2, {500, 50});
    reg.add_component<Velocity>(entity2, {10, 0});
    reg.add_component<Position>(entity3, {80, 60});
    reg.add_component<Velocity>(entity3, {0, 30});

    auto component = reg.get_components<Position>();

    cr_assert_eq(component[entity1].value().x, 200, "before system_event value entity1 x");
    cr_assert_eq(component[entity1].value().y, 100, "before system_event value entity1 y");
    cr_assert_eq(component[entity2].value().x, 500, "before system_event value entity2 x");
    cr_assert_eq(component[entity2].value().y, 50, "before system_event value entity2 y");
    cr_assert_eq(component[entity3].value().x, 80, "before system_event value entity3 x");
    cr_assert_eq(component[entity3].value().y, 60, "before system_event value entity3 y");

    reg.kill_entity(entity2);

    reg.run_event(UpdateEvent{});

    component = reg.get_components<Position>();

    cr_assert_eq(component[entity1].value().x, 250, "system_event value entity 1 x");
    cr_assert_eq(component[entity1].value().y, 130, "system_event value entity 1 y");
    cr_assert_not(component[entity2].has_value(), "system_event value entity2 has not value");
    cr_assert_eq(component[entity3].value().x, 80, "system_event value entity3 x");
    cr_assert_eq(component[entity3].value().y, 90, "system_event value entity3 y");
}

Test(Systems, many_systems_in_event) {
    Registry reg;

    reg.register_component<Position>();
    reg.register_component<Velocity>();

    reg.register_event<UpdateEvent>();

    reg.subscribe<UpdateEvent>(update_pos_system);
    reg.subscribe<UpdateEvent>(modify_pos_system);

    auto entity = reg.spawn_entity();

    reg.add_component<Position>(entity, {200, 100});
    reg.add_component<Velocity>(entity, {50, 30});

    auto component = reg.get_components<Position>();

    cr_assert_eq(component[entity].value().x, 200, "before system_event value x");
    cr_assert_eq(component[entity].value().y, 100, "before system_event value y");

    reg.run_event(UpdateEvent{1});

    component = reg.get_components<Position>();

    cr_assert_eq(component[entity].value().x, 260, "system_event value x");
    cr_assert_eq(component[entity].value().y, 140, "system_event value y");
}

Test(Systems, same_systems_multiple_times_in_event) {
    Registry reg;

    reg.register_component<Position>();
    reg.register_component<Velocity>();

    reg.register_event<UpdateEvent>();

    reg.subscribe<UpdateEvent>(update_pos_system);
    reg.subscribe<UpdateEvent>(update_pos_system);

    auto entity = reg.spawn_entity();

    reg.add_component<Position>(entity, {200, 100});
    reg.add_component<Velocity>(entity, {50, 30});

    auto component = reg.get_components<Position>();

    cr_assert_eq(component[entity].value().x, 200, "before system_event value x");
    cr_assert_eq(component[entity].value().y, 100, "before system_event value y");

    reg.run_event(UpdateEvent{1});

    component = reg.get_components<Position>();

    cr_assert_eq(component[entity].value().x, 300, "system_event value x");
    cr_assert_eq(component[entity].value().y, 160, "system_event value y");
}

Test(Systems, events_with_many_system) {
    Registry reg;

    reg.register_component<Position>();
    reg.register_component<Velocity>();

    reg.register_event<UpdateEvent>();

    reg.subscribe<UpdateEvent>([](Registry &r, const UpdateEvent &event) {
        update_pos_system(r, event);
        auto &positions = r.get_components<Position>();

        for (int i = 0; i < positions.size(); ++i) {
            if (positions[i]) {
                auto &pos = positions[i].value();
                pos.y = 25;
            }
        }
    });

    auto entity = reg.spawn_entity();

    reg.add_component<Position>(entity, {200, 100});
    reg.add_component<Velocity>(entity, {50, 30});

    auto component = reg.get_components<Position>();

    cr_assert_eq(component[entity].value().x, 200, "before system_event value x");
    cr_assert_eq(component[entity].value().y, 100, "before system_event value y");

    reg.run_event(UpdateEvent{1});

    component = reg.get_components<Position>();

    cr_assert_eq(component[entity].value().x, 250, "system_event value x");
    cr_assert_eq(component[entity].value().y, 25, "system_event value y");
}

Test(Systems, events_with_lambda) {
    Registry reg;

    reg.register_component<Position>();

    reg.register_event<ParamEvent>();

    reg.subscribe<ParamEvent>([](Registry &r, const ParamEvent &event) {
        auto &pos = r.get_components<Position>();

        for (int i = 0; i < pos.size(); i++) {
            if (pos[i]) {
                pos[i].value().x += event.res;
                pos[i].value().y += event.res;
            }
        }
    });

    auto entity = reg.spawn_entity();

    reg.add_component<Position>(entity, {200, 100});

    auto component = reg.get_components<Position>();

    cr_assert_eq(component[entity].value().x, 200, "before system_event value x");
    cr_assert_eq(component[entity].value().y, 100, "before system_event value y");

    reg.run_event(ParamEvent{10, 5, 5});

    component = reg.get_components<Position>();

    cr_assert_eq(component[entity].value().x, 220, "system_event value x");
    cr_assert_eq(component[entity].value().y, 120, "system_event value y");
}

Test(Systems, system_in_many_event) {
    Registry reg;

    int a = 0;
    int b = 4;
    int c = 6;

    reg.register_component<Position>();
    reg.register_component<Velocity>();

    reg.register_event<ParamEvent>();
    reg.register_event<UpdateEvent>();

    reg.subscribe<UpdateEvent>(update_pos_system);
    reg.subscribe<ParamEvent>([](Registry &r, const ParamEvent &event) {
        update_pos_system(r, UpdateEvent{event.res});
    });

    auto entity = reg.spawn_entity();

    reg.add_component<Position>(entity, {200, 100});
    reg.add_component<Velocity>(entity, {50, 30});

    auto component = reg.get_components<Position>();

    cr_assert_eq(component[entity].value().x, 200, "before system_event value x");
    cr_assert_eq(component[entity].value().y, 100, "before system_event value y");

    reg.run_event(UpdateEvent{0});

    component = reg.get_components<Position>();

    cr_assert_eq(component[entity].value().x, 250, "system_event value x after UpdateEvent");
    cr_assert_eq(component[entity].value().y, 130, "system_event value y after UpdateEvent");

    reg.run_event(ParamEvent{a, b, c});

    component = reg.get_components<Position>();

    cr_assert_eq(component[entity].value().x, 300, "system_event value x after ParamEvent");
    cr_assert_eq(component[entity].value().y, 160, "system_event value y after ParamEvent");
}
