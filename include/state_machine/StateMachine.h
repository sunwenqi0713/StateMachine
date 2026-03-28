#pragma once

#include <state_machine/State.h>

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

class StateMachine : public std::enable_shared_from_this<StateMachine> {
  public:
    void setState(std::shared_ptr<State> state);
    void handleEvent(const std::string& event);

    template <typename T, typename... Args> void registerState(const std::string& stateName, Args&&... args) {
        auto state = std::make_shared<T>(std::forward<Args>(args)...);
        stateMap[stateName] = [state](std::shared_ptr<StateMachine> fsm) { fsm->setState(state); };
    }

    void transitionTo(const std::string& stateName);

  private:
    std::shared_ptr<State> currentState;
    std::unordered_map<std::string, std::function<void(std::shared_ptr<StateMachine>)>> stateMap;
};
