#pragma once

#include <exception>
#include <utility>

namespace pzl {

template <scope_exit_type TExitType, typename TAction>
scope_guard<TExitType, TAction>::scope_guard(rref<TAction> action)
        : action_(std::forward<TAction>(action)) 
        , active_(true) {
    if constexpr (TExitType != scope_exit_type::exit) {
        exception_count_ = std::uncaught_exceptions();
    }
}

template <scope_exit_type TExitType, typename TAction>
scope_guard<TExitType, TAction>::scope_guard(rref<scope_guard> source)
        : action_(std::move(source.action_))
        , active_(source.release()) {
    if constexpr (TExitType != scope_exit_type::exit) {
        exception_count_ = source.exception_count_;
    }
}

template <scope_exit_type TExitType, typename TAction>
scope_guard<TExitType, TAction>::~scope_guard() {
    if (active_) {
        if constexpr (TExitType == scope_exit_type::success) {
            if (std::uncaught_exceptions() > exception_count_) {
                return;
            }
        } else if constexpr (TExitType == scope_exit_type::fail) {
            if (std::uncaught_exceptions() <= exception_count_) {
                return;
            }
        }

        action_();
    }
}

template <scope_exit_type TExitType, typename TAction>
bool scope_guard<TExitType, TAction>::release() {
    return std::exchange(active_, false);
}

} // End namespace pzl.
