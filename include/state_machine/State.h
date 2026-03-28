#pragma once

#include <memory>
#include <string>

class StateMachine;

class State {
  public:
    virtual ~State() = default;
    virtual void handleEvent(const std::string& event, std::shared_ptr<StateMachine> fsm) = 0;
};
