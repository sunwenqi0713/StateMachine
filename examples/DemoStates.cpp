#include "DemoStates.h"

#include <iostream>

// ── StateA ────────────────────────────────────────────────────────────────────

StateA::StateA() {
    handle("EVENT_B", [](const Event& event, StateMachine& fsm) {
        if (event.payload.has_value()) {
            try {
                std::cout << "[StateA] EVENT_B payload: "
                          << std::any_cast<std::string>(event.payload) << "\n";
            } catch (const std::bad_any_cast&) {
                std::cout << "[StateA] EVENT_B unexpected payload type\n";
            }
        }
        std::cout << "[StateA] transitioning to StateB\n";
        fsm.transitionTo("StateB");
    });
}

void StateA::onEnter() { std::cout << "[StateA] onEnter\n"; }
void StateA::onExit()  { std::cout << "[StateA] onExit\n"; }

void StateA::onUnhandledEvent(const Event& event, StateMachine&) {
    std::cout << "[StateA] unhandled event: " << event.name << "\n";
}

// ── StateB ────────────────────────────────────────────────────────────────────

StateB::StateB() {
    handle("EVENT_A", [](const Event& /*event*/, StateMachine& fsm) {
        std::cout << "[StateB] transitioning to StateA\n";
        fsm.transitionTo("StateA");
    });
}

void StateB::onEnter() { std::cout << "[StateB] onEnter\n"; }
void StateB::onExit()  { std::cout << "[StateB] onExit\n"; }

void StateB::onUnhandledEvent(const Event& event, StateMachine&) {
    std::cout << "[StateB] unhandled event: " << event.name << "\n";
}
