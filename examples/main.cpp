#include "DemoStates.h"

#include <iostream>

int main() {
    StateMachine fsm;

    fsm.setErrorCallback([](const std::string& error) {
        std::cerr << "[ERROR] " << error << "\n";
    });

    fsm.registerState<StateA>("StateA");
    fsm.registerState<StateB>("StateB");

    // 启动状态机，触发 StateA::onEnter
    fsm.transitionTo("StateA");
    std::cout << "Current state: " << fsm.currentStateName() << "\n\n";

    // 分发带数据载荷的事件
    fsm.dispatch(Event{"EVENT_B", std::string{"hello from main"}});
    std::cout << "Current state: " << fsm.currentStateName() << "\n\n";

    // 分发无载荷事件（便捷重载）
    fsm.dispatch("EVENT_A");
    std::cout << "Current state: " << fsm.currentStateName() << "\n\n";

    // 触发 onUnhandledEvent
    fsm.dispatch("UNKNOWN_EVENT");

    // 状态查询
    std::cout << "\nIn StateA? " << std::boolalpha << fsm.isInState("StateA") << "\n";
    std::cout << "In StateB? " << fsm.isInState("StateB") << "\n";

    // 触发错误回调
    fsm.transitionTo("StateC");

    return 0;
}
