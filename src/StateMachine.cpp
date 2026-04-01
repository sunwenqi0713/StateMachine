#include <state_machine/StateMachine.h>

#include <iostream>

void StateMachine::setErrorCallback(ErrorCallback callback) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    errorCallback_ = std::move(callback);
}

bool StateMachine::transitionTo(const std::string& stateName) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);

    auto it = stateFactories_.find(stateName);
    if (it == stateFactories_.end()) {
        reportError("Unknown state: " + stateName);
        return false;
    }

    if (currentState_) {
        currentState_->onExit();
    }

    currentState_     = it->second();
    currentStateName_ = stateName;
    currentState_->onEnter();

    return true;
}

void StateMachine::dispatch(const Event& event) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    if (!currentState_) {
        reportError("dispatch(\"" + event.name + "\") called with no active state");
        return;
    }
    currentState_->onEvent(event, *this);
}

void StateMachine::dispatch(const std::string& eventName) {
    dispatch(Event{eventName});
}

const std::string& StateMachine::currentStateName() const {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return currentStateName_;
}

bool StateMachine::isInState(const std::string& stateName) const {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return currentStateName_ == stateName;
}

void StateMachine::reportError(const std::string& msg) {
    if (errorCallback_) {
        errorCallback_(msg);
    } else {
        std::cerr << "[StateMachine] " << msg << std::endl;
    }
}
