#include "DemoStates.h"
#include <state_machine/StateMachine.h>

#include <memory>

int main() {
    auto fsm = std::make_shared<StateMachine>();
    fsm->registerState<StateA>("StateA");
    fsm->registerState<StateB>("StateB");
    fsm->transitionTo("StateA");

    fsm->handleEvent("EVENT_B");
    fsm->handleEvent("EVENT_A");

    return 0;
}
