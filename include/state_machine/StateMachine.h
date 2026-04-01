#pragma once

#include <state_machine/State.h>

#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <tuple>
#include <unordered_map>

class StateMachine {
  public:
    using ErrorCallback = std::function<void(const std::string&)>;

    // 注册自定义错误处理回调；未设置时默认输出到 stderr
    void setErrorCallback(ErrorCallback callback);

    // 注册状态：每次 transitionTo 时通过工厂创建新实例，避免跨转换的脏状态
    template <typename T, typename... Args>
    void registerState(const std::string& stateName, Args&&... args) {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        auto argTuple = std::make_tuple(std::forward<Args>(args)...);
        stateFactories_[stateName] = [argTuple]() -> std::shared_ptr<State> {
            return std::apply(
                [](const auto&... a) { return std::make_shared<T>(a...); }, argTuple);
        };
    }

    // 转换到指定状态；依次调用当前状态 onExit、构造新状态、调用 onEnter
    // 返回 false 表示目标状态未注册
    bool transitionTo(const std::string& stateName);

    // 将事件分发到当前状态；currentState 为空时触发错误回调
    void dispatch(const Event& event);

    // 便捷重载：仅传事件名，无数据载荷
    void dispatch(const std::string& eventName);

    // 查询当前状态名；状态机未启动时返回空字符串
    const std::string& currentStateName() const;

    // 判断当前是否处于指定状态
    bool isInState(const std::string& stateName) const;

  private:
    // 使用 recursive_mutex：handleEvent -> state::handleEvent -> transitionTo 存在重入
    mutable std::recursive_mutex mutex_;

    std::shared_ptr<State>                                          currentState_;
    std::string                                                     currentStateName_;
    std::unordered_map<std::string, std::function<std::shared_ptr<State>()>> stateFactories_;
    ErrorCallback                                                   errorCallback_;

    void reportError(const std::string& msg);
};
