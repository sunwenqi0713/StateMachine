# StateMachine

A lightweight, header-friendly C++17 finite state machine library featuring structured events, lifecycle hooks, per-constructor handler registration, and basic thread safety.

## Features

- **Structured events** — `Event` carries a name and an optional `std::any` payload
- **Lifecycle hooks** — `onEnter()` / `onExit()` called automatically on every transition
- **Handler registration in constructor** — use `handle("EVENT", fn)` instead of branching inside `handleEvent`
- **Fresh instances per transition** — factories create a new state object on each `transitionTo`, eliminating stale state
- **Thread-safe dispatch** — `std::recursive_mutex` allows re-entrant transitions (state → FSM → transition) from a single thread
- **Error callback** — customizable error handler instead of hard-coded `std::cerr`
- **State query** — `currentStateName()` and `isInState()` for runtime introspection
- No external dependencies, requires C++17

## Quick Start

### 1. Define states

```cpp
// MyStates.h
#include <state_machine/State.h>
#include <state_machine/StateMachine.h>

class Idle : public State {
  public:
    Idle() {
        handle("START", [](const Event& event, StateMachine& fsm) {
            fsm.transitionTo("Running");
        });
    }
    void onEnter() override { std::cout << "[Idle] entered\n"; }
    void onExit()  override { std::cout << "[Idle] exited\n"; }

  protected:
    void onUnhandledEvent(const Event& e, StateMachine&) override {
        std::cout << "[Idle] unhandled: " << e.name << "\n";
    }
};

class Running : public State {
  public:
    Running() {
        handle("STOP", [](const Event&, StateMachine& fsm) {
            fsm.transitionTo("Idle");
        });
    }
    void onEnter() override { std::cout << "[Running] entered\n"; }
    void onExit()  override { std::cout << "[Running] exited\n"; }
};
```

### 2. Set up and run the FSM

```cpp
#include "MyStates.h"
#include <iostream>

int main() {
    StateMachine fsm;

    // Optional: custom error handler
    fsm.setErrorCallback([](const std::string& err) {
        std::cerr << "[ERROR] " << err << "\n";
    });

    fsm.registerState<Idle>("Idle");
    fsm.registerState<Running>("Running");

    fsm.transitionTo("Idle");                       // triggers Idle::onEnter
    std::cout << fsm.currentStateName() << "\n";    // "Idle"

    fsm.dispatch("START");                          // Idle::onExit → Running::onEnter
    std::cout << fsm.isInState("Running") << "\n";  // true

    // Dispatch with payload
    fsm.dispatch(Event{"STOP", std::string{"user requested"}});

    return 0;
}
```

## API Reference

### `StateMachine`

| Method | Description |
|--------|-------------|
| `registerState<T>(name, args...)` | Register a state factory; `args` are forwarded to `T`'s constructor on each transition |
| `transitionTo(name)` | Exit current state, create new instance, enter it; returns `false` if name is unknown |
| `dispatch(event)` | Forward `Event` to current state's handler table |
| `dispatch(name)` | Convenience overload — wraps name in `Event` with no payload |
| `currentStateName()` | Returns the active state name, or `""` if not started |
| `isInState(name)` | Returns `true` if the current state name matches |
| `setErrorCallback(fn)` | Set a custom `void(const std::string&)` error handler |

### `State`

| Method | Description |
|--------|-------------|
| `onEnter()` | Called after the state becomes active (override as needed) |
| `onExit()` | Called before the state is deactivated (override as needed) |
| `handle(eventName, fn)` | Register a handler in the constructor; `fn` is `void(const Event&, StateMachine&)` |
| `onUnhandledEvent(event, fsm)` | Called when no handler is registered for the event (override for logging/errors) |

### `Event`

```cpp
struct Event {
    std::string name;
    std::any    payload;   // optional — use std::any_cast<T> to extract
};
```

## Building

Requires CMake 3.14+ and a C++17-capable compiler.

```bash
cmake -B build
cmake --build build
./build/examples/demo
```

## Design Notes

- **No `shared_ptr` required** — `StateMachine` is a value type; allocate on the stack or heap as needed.
- **Re-entrant transitions** — calling `transitionTo` from inside a state handler is safe due to `std::recursive_mutex`, but cycles should be guarded by application logic.
- **Payload extraction** — use `std::any_cast<T>` inside handlers; catch `std::bad_any_cast` for resilience.
- **State isolation** — each `transitionTo` call constructs a fresh state instance, so member variables do not persist across re-entries.
