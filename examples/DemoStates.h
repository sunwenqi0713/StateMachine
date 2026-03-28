#pragma once

#include <state_machine/State.h>
#include <state_machine/StateMachine.h>

#include <memory>
#include <string>

class StateA : public State {
  public:
    void handleEvent(const std::string& event, std::shared_ptr<StateMachine> fsm) override;
};

class StateB : public State {
  public:
    void handleEvent(const std::string& event, std::shared_ptr<StateMachine> fsm) override;
};
