#include "DemoStates.h"

#include <iostream>

void StateA::handleEvent(const std::string& event, std::shared_ptr<StateMachine> fsm) {
    if (event == "EVENT_B") {
        std::cout << "StateA handling EVENT_B, transitioning to StateB" << std::endl;
        fsm->transitionTo("StateB");
    } else {
        std::cout << "StateA received unknown event: " << event << std::endl;
    }
}

void StateB::handleEvent(const std::string& event, std::shared_ptr<StateMachine> fsm) {
    if (event == "EVENT_A") {
        std::cout << "StateB handling EVENT_A, transitioning to StateA" << std::endl;
        fsm->transitionTo("StateA");
    } else {
        std::cout << "StateB received unknown event: " << event << std::endl;
    }
}
