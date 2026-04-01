#pragma once

#include <any>
#include <functional>
#include <string>
#include <unordered_map>

class StateMachine;

// 事件：携带名称和可选的任意类型数据载荷
struct Event {
    std::string name;
    std::any    payload;

    explicit Event(std::string name, std::any payload = {})
        : name(std::move(name)), payload(std::move(payload)) {}
};

class State {
  public:
    using Handler = std::function<void(const Event&, StateMachine&)>;

    virtual ~State() = default;
    virtual void onEnter() {}
    virtual void onExit() {}

    // 将事件分发到已注册的处理函数；子类不可覆盖，通过 on() 注册处理逻辑
    virtual void onEvent(const Event& event, StateMachine& fsm) final {
        auto it = handlers_.find(event.name);
        if (it != handlers_.end()) {
            it->second(event, fsm);
        } else {
            onUnhandledEvent(event, fsm);
        }
    }

  protected:
    // 在构造函数中调用，注册事件名到处理函数的映射
    void handle(const std::string& eventName, Handler handler) {
        handlers_[eventName] = std::move(handler);
    }

    // 未注册事件的默认处理；子类可覆盖以实现自定义日志或错误上报
    virtual void onUnhandledEvent(const Event& /*event*/, StateMachine& /*fsm*/) {}

  private:
    std::unordered_map<std::string, Handler> handlers_;
};
