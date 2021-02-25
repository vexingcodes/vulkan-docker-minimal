#pragma once

#include <type_traits>

#include "language.hpp"

namespace pzl {

enum class scope_exit_type {
    exit,
    success,
    fail
};

template <scope_exit_type TExitType, typename TAction>
class scope_guard
{
public:
    [[nodiscard]] scope_guard(TAction&& action);

    scope_guard(ref<const scope_guard>) = delete;
    auto operator=(ref<const scope_guard>) -> ref<scope_guard> = delete;
    [[nodiscard]] scope_guard(rref<scope_guard> source);
    auto operator=(rref<scope_guard>) -> ref<scope_guard> = delete;

    ~scope_guard();

    auto release() -> bool;

private:
    struct empty {};
    using exception_count_type = std::conditional_t<TExitType == scope_exit_type::exit, empty, int>;

    [[no_unique_address]] exception_count_type exception_count_;
    TAction action_;
    bool active_;
};

template <typename TAction>
using scope_exit = scope_guard<scope_exit_type::exit, TAction>;

template <typename TAction>
using scope_success = scope_guard<scope_exit_type::success, TAction>;

template <typename TAction>
using scope_fail = scope_guard<scope_exit_type::fail, TAction>;

} // End namespace pzl.

#include "scope.ipp"
