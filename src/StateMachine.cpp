#include <state_machine/StateMachine.h>

#include <iostream>

void StateMachine::setState(std::shared_ptr<State> state) { currentState = std::move(state); }

void StateMachine::handleEvent(const std::string& event) {
    if (currentState) {
        currentState->handleEvent(event, shared_from_this());
    }
}

void StateMachine::transitionTo(const std::string& stateName) {
    auto it = stateMap.find(stateName);
    if (it != stateMap.end()) {
        it->second(shared_from_this());
    } else {
        std::cerr << "Unknown state: " << stateName << std::endl;
    }
}
