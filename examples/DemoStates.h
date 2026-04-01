#pragma once

#include <state_machine/State.h>
#include <state_machine/StateMachine.h>

class StateA : public State {
  public:
    StateA();
    void onEnter() override;
    void onExit() override;

  protected:
    void onUnhandledEvent(const Event& event, StateMachine& fsm) override;
};

class StateB : public State {
  public:
    StateB();
    void onEnter() override;
    void onExit() override;

  protected:
    void onUnhandledEvent(const Event& event, StateMachine& fsm) override;
};
