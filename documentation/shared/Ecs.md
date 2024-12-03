# Ecs documentation

An ECS is an architectural design pattern, mostly used in video game development. This pattern follows
the principle of composition over inheritance. Instead of defining an inheritance tree as usual in Object Ori-
ented Programming, types are split into small yet highly reusable components

## Initialise registry 
the Registry will be the body of our Ecs. This is where we'll add all the components, systems, events and control data for our game.
### Add components to registry

#### - Create Structure for components:
Each component is a different type, which can be a structure, a class or a standard type (int, float, char...).
```c++
struct Position {
    float x, y;
};

struct Velocity {
    float dx, dy;
};

struct Drawable {
    sf::RectangleShape shape;

    Drawable(float width, float height, sf::Color color) {
        shape.setSize({width, height});
        shape.setFillColor(color);
    }
};

struct Controllable {};
```

#### - Register a component to the registry:
```c++
Registry reg;

reg.register_component<Position>();
reg.register_component<Velocity>();
reg.register_component<Drawable>();
reg.register_component<Controllable>();
```
###
## Entities in the registry
### Add / Remove / Modify an Entity
your data will be represented in the form of entities, which will simply be **ids** where you can add your components. Here, our entity will be a player, so we'll add a position, velocity, sprite (here, just a simple square) and a controllable component that lets us define that the entity is a player.

#### - Create an entity and add components to the entity
```c++
auto entity = reg.spawn_entity();

reg.add_component<Position>(entity, {400, 300}); // Position : x = 400, y = 300
reg.add_component<Velocity>(entity, {0, 0}); // Velocity : dx = 0, dy = 0
reg.add_component<Drawable>(entity, Drawable(50, 50, sf::Color::Red)); // Drawable : shape.width = 50, shape.height = 50, shape.color = sf::Color::Red
reg.add_component<Controllable>(entity, {}); // Controllable : empty (this can be used to check if an entity is a player or not)
```

#### - Get components array and component value of an entity
```c++
auto &velocity = reg.get_components<Velocity>();

if (velocity[entity].has_value()) { // or if (velocity[0].has_value())
    velocity[entity].value().dx // dx = 0
    velocity[entity].value().dy // dy = 0
} 

auto &position = reg.get_components<Position>();

if (position[entity].has_value()) { // or if (position[0].has_value())
    position[entity].value().x // x = 400
    position[entity].value().y // y = 300
} 

```

#### - Remove a component from an entity
```c++
auto entity1 = reg.spawn_entity();
auto entity2 = reg.spawn_entity();

reg.add_component<Position>(entity1, {400, 300});
reg.add_component<Velocity>(entity1, {0, 0});
reg.add_component<Drawable>(entity1, Drawable(50, 50, sf::Color::Red));
reg.add_component<Controllable>(entity1, {});

reg.add_component<Position>(entity2, {200, 100});
reg.add_component<Velocity>(entity2, {10, 5});
reg.add_component<Drawable>(entity2, Drawable(100, 50, sf::Color::Yellow));
reg.add_component<Controllable>(entity2, {});

reg.remove_component<Velocity>(entity1);

auto &velocity = reg.get_components<Velocity>();

if (velocity[entity1].has_value()) { // or if (velocity[0].has_value())
    // should not enter because we remove the velocity component from entity1
}

if (velocity[entity2].has_value()) { // or if (velocity[1].has_value())
    // should enter here because entity2 still has velocity component
}
```

#### - Kill an entity
the entities are stored in an array, so you can have three entities, kill the second and when you iterate on your entities, the loop will pass over the third entity.
```c++
auto entity1 = reg.spawn_entity();
auto entity2 = reg.spawn_entity();
auto entity3 = reg.spawn_entity();

reg.add_component<Position>(entity1, {400, 300});
reg.add_component<Velocity>(entity1, {0, 0});
reg.add_component<Drawable>(entity1, Drawable(50, 50, sf::Color::Red));
reg.add_component<Controllable>(entity1, {});

reg.add_component<Position>(entity2, {200, 100});
reg.add_component<Velocity>(entity2, {10, 5});
reg.add_component<Drawable>(entity2, Drawable(100, 50, sf::Color::Yellow));
reg.add_component<Controllable>(entity2, {});

...

reg.kill_entity(entity2)

auto &velocity = reg.get_components<Velocity>();

if (velocity[entity1].has_value()) { // or if (velocity[0].has_value())
    // should enter here
}

if (velocity[entity2].has_value()) { // or if (velocity[1].has_value())
    // should not enter because we kill entity2
}

if (velocity[entity3].has_value()) { // or if (velocity[2].has_value())
    // should enter here
}
```

###
## Event and Systems
Here, functions will be launched by systems associated with events. It's in your systems that you'll be able to access or modify the values of your entities (you can also do this externally with **get_component()**). You will then associate these systems with events, and each time you launch an event with **run_event()**, all associated systems will be executed in the order in which they were added.
Events are represented by structures or classes. You can therefore pass elements as parameters to your systems using events (e.g. the game window reference to modify it in a system).
#### - Create structure for each event
```c++
struct ControlEvent {};

struct RenderEvent {
    float delta_time;
    sf::RenderWindow& window;
};
```

#### - Register an event to the registry
```c++
reg.register_event<ControlEvent>();
reg.register_event<RenderEvent>();
```

#### - Create systems function
```c++
// this system apply each entity velocity to each entity position 
void position_system(Registry& r, const RenderEvent& event) {
    auto& positions = r.get_components<Position>();
    auto& velocities = r.get_components<Velocity>();

    for (size_t i = 0; i < positions.size() && i < velocities.size(); ++i) {
        if (positions[i] && velocities[i]) {
            auto& pos = positions[i].value();
            auto& vel = velocities[i].value();

            pos.x += vel.dx * event.delta_time;
            pos.y += vel.dy * event.delta_time;
        }
    }
}

// this system modify velocity of each controllable entity if a key is pressed 
void control_system(Registry& r, const ControlEvent&) {
    auto& velocities = r.get_components<Velocity>();
    auto& controllables = r.get_components<Controllable>();
    
    for (size_t i = 0; i < velocities.size() && i < controllables.size(); ++i) {
        if (velocities[i] && controllables[i]) {
            auto& vel = velocities[i].value();
            vel.dx = vel.dy = 0;
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                vel.dy = -1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                vel.dy = 1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                vel.dx = -1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                vel.dx = 1.0f;
        }
    }
}
```

#### - Add a system to an event
```c++
reg.subscribe<ControlEvent>(control_system);
reg.subscribe<RenderEvent>(position_system);

// You can add many system to each event, they will be executed in the order in which they were added
// ex :
reg.subscribe<ControlEvent>(control_system);
reg.subscribe<ControlEvent>(other_system1);
reg.subscribe<ControlEvent>(other_system2);

// OR create a lambda to directly add code or as many system you want : 
reg.subscribe<ControlEvent>([](Register &r, const ControlEvent &event) {
    control_system(r, event);
    other_system(r, event);
    std::cout << "You can add another code here" << std::endl;
});

// you can also add the same system to an event several times, so that it executes several times.
reg.subscribe<ControlEvent>([](Register &r, const ControlEvent &event) {
    control_system(r, event);
    control_system(r, event);
    control_system(r, event);
});
```

#### - Add a system to many event
```c++
reg.subscribe<ControlEvent>(control_system);
reg.subscribe<RenderEvent>([](Registry &r, const RenderEvent &event) {
    control_system(r, ControlEvent{});
});
```
#### - Remove all systems from an event
```c++
reg.unsubscribe_all<ControlEvent>();
reg.unsubscribe_all<RenderEvent>();
```

#### - Run an event
```c++
sf::RenderWindow window(sf::VideoMode(800, 600), "game window");

reg.run_event(ControlEvent{});
reg.run_event(RenderEvent{1.0f, window});
```

## Example

Here's an example using ecs and sfml to move a player, represented by a red square, in a window

```c++
struct Position {
    float x, y;
};

struct Velocity {
    float dx, dy;
};

struct Drawable {
    sf::RectangleShape shape;

    Drawable(float width, float height, sf::Color color) {
        shape.setSize({width, height});
        shape.setFillColor(color);
    }
};

struct ControlEvent {};

struct UpdateEvent {};

struct RenderEvent {
    float delta_time;
    sf::RenderWindow& window;
};

struct CloseEvent {
    sf::RenderWindow& window;
};

struct Controllable {};

void control_system(Registry& r, const ControlEvent&) {
    auto& velocities = r.get_components<Velocity>();
    auto& controllables = r.get_components<Controllable>();

    for (size_t i = 0; i < velocities.size() && i < controllables.size(); ++i) {
        if (velocities[i] && controllables[i]) {
            auto& vel = velocities[i].value();
            vel.dx = vel.dy = 0;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                vel.dy = -1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                vel.dy = 1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                vel.dx = -1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                vel.dx = 1.0f;
        }
    }
}

void position_system(Registry& r, const RenderEvent& event) {
    auto& positions = r.get_components<Position>();
    auto& velocities = r.get_components<Velocity>();

    for (size_t i = 0; i < positions.size() && i < velocities.size(); ++i) {
        if (positions[i] && velocities[i]) {
            auto& pos = positions[i].value();
            auto& vel = velocities[i].value();

            pos.x += vel.dx * event.delta_time;
            pos.y += vel.dy * event.delta_time;
        }
    }
}

void draw_system(Registry& r, const RenderEvent& event) {
    auto& positions = r.get_components<Position>();
    auto& drawables = r.get_components<Drawable>();

    event.window.clear();

    for (size_t i = 0; i < positions.size() && i < drawables.size(); ++i) {
        if (positions[i] && drawables[i]) {
            auto& pos = positions[i].value();
            auto& drawable = drawables[i].value();

            drawable.shape.setPosition(pos.x, pos.y);
            event.window.draw(drawable.shape);

        }
    }
    event.window.display();
}

void close_system(Registry& , const CloseEvent &event) {
    std::cout << "QUIT GAME" << std::endl;
    event.window.close();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "game window");
    window.setFramerateLimit(60);

    Registry reg;

    reg.register_component<Position>();
    reg.register_component<Velocity>();
    reg.register_component<Drawable>();
    reg.register_component<Controllable>();

    auto entity = reg.spawn_entity();
    reg.add_component<Position>(entity, {400, 300});
    reg.add_component<Velocity>(entity, {0, 0});
    reg.add_component<Drawable>(entity, Drawable(50, 50, sf::Color::Red));
    reg.add_component<Controllable>(entity, {});

    reg.register_event<ControlEvent>();
    reg.register_event<CloseEvent>();
    reg.register_event<RenderEvent>();

    reg.subscribe<ControlEvent>(control_system);
    reg.subscribe<RenderEvent>(position_system);
    reg.subscribe<RenderEvent>(draw_system);
    reg.subscribe<CloseEvent>([](Registry& r, const CloseEvent& event) {
        close_system(r, event);
    });

    while (window.isOpen()) {
        sf::Event sf_event;
        while (window.pollEvent(sf_event)) {
            if (sf_event.type == sf::Event::Closed) {
                reg.run_event(CloseEvent{window});
            }

        }

        reg.run_event(ControlEvent{});
        reg.run_event(RenderEvent{1.0f, window});

    }

    return 0;
}
```