#include <gtest/gtest.h>
#include <optional>

#include "scope.hpp"

namespace pzl {

// Ensure that the scope_exit structure is smaller than the scope_fail structure, since the
// scope_exit structure doesn't have to keep track of the exception count when it is constructed.  I
// suppose this doesn't _have_ to be true, but it probably should be.
void dummy() { }
static_assert(sizeof(scope_exit<decltype(&dummy)>) < sizeof(scope_fail<decltype(&dummy)>));

TEST(scope, exit_success) {
    int hit_count = 0;
    {
        auto handler = scope_exit([&]{ ++hit_count; });
        EXPECT_EQ(0, hit_count);
    }
    EXPECT_EQ(1, hit_count);
}

TEST(scope, exit_success_released) {
    int hit_count = 0;
    {
        auto handler = scope_exit([&]{ ++hit_count; });
        EXPECT_EQ(0, hit_count);
        handler.release();
    }
    EXPECT_EQ(0, hit_count);
}

TEST(scope, exit_success_move) {
    int hit_count = 0;
    auto handler_lambda = [&]{ ++hit_count; };
    using scope_exit_type = scope_exit<decltype(handler_lambda)>;

    std::optional<scope_exit_type> exit_holder;

    {
        auto handler = scope_exit(std::move(handler_lambda));
        exit_holder.emplace(std::move(handler));
        EXPECT_EQ(0, hit_count);
    }

    EXPECT_EQ(0, hit_count);
    exit_holder.reset();
    EXPECT_EQ(1, hit_count);
}

TEST(scope, exit_fail) {
    int hit_count = 0;

    try {
        auto handler = scope_exit([&]{ ++hit_count; });
        EXPECT_EQ(0, hit_count);
        throw std::exception();
    } catch (...) {
    }

    EXPECT_EQ(1, hit_count);
}

TEST(scope, exit_fail_released) {
    int hit_count = 0;

    try {
        auto handler = scope_exit([&]{ ++hit_count; });
        EXPECT_EQ(0, hit_count);
        handler.release();
        throw std::exception();
    } catch (...) {
    }

    EXPECT_EQ(0, hit_count);
}

TEST(scope, exit_fail_move) {
    int hit_count = 0;
    auto handler_lambda = [&]{ ++hit_count; };
    using scope_exit_type = scope_exit<decltype(handler_lambda)>;

    std::optional<scope_exit_type> exit_holder;

    try {
        auto handler = scope_exit(std::move(handler_lambda));
        exit_holder.emplace(std::move(handler));
        EXPECT_EQ(0, hit_count);
        throw std::exception();
    } catch (...) {
    }

    EXPECT_EQ(0, hit_count);
    exit_holder.reset();
    EXPECT_EQ(1, hit_count);
}

TEST(scope, fail_success) {
    int hit_count = 0;
    {
        auto handler = scope_fail([&]{ ++hit_count; });
        EXPECT_EQ(0, hit_count);
    }
    EXPECT_EQ(0, hit_count);
}

TEST(scope, fail_success_released) {
    int hit_count = 0;
    {
        auto handler = scope_fail([&]{ ++hit_count; });
        EXPECT_EQ(0, hit_count);
        handler.release();
    }
    EXPECT_EQ(0, hit_count);
}

TEST(scope, fail_success_move) {
    int hit_count = 0;
    auto handler_lambda = [&]{ ++hit_count; };
    using scope_fail_type = scope_fail<decltype(handler_lambda)>;

    std::optional<scope_fail_type> fail_holder;

    {
        auto handler = scope_fail(std::move(handler_lambda));
        fail_holder.emplace(std::move(handler));
        EXPECT_EQ(0, hit_count);
    }

    EXPECT_EQ(0, hit_count);
    fail_holder.reset();
    EXPECT_EQ(0, hit_count);
}

TEST(scope, fail_fail) {
    int hit_count = 0;

    try {
        auto handler = scope_fail([&]{ ++hit_count; });
        EXPECT_EQ(0, hit_count);
        throw std::exception();
    } catch (...) {
    }

    EXPECT_EQ(1, hit_count);
}

TEST(scope, fail_fail_released) {
    int hit_count = 0;

    try {
        auto handler = scope_fail([&]{ ++hit_count; });
        EXPECT_EQ(0, hit_count);
        handler.release();
        throw std::exception();
    } catch (...) {
    }

    EXPECT_EQ(0, hit_count);
}

TEST(scope, fail_fail_move) {
    int hit_count = 0;
    auto handler_lambda = [&]{ ++hit_count; };
    using scope_fail_type = scope_fail<decltype(handler_lambda)>;

    std::optional<scope_fail_type> fail_holder;

    try {
        auto handler = scope_fail(std::move(handler_lambda));
        fail_holder.emplace(std::move(handler));
        EXPECT_EQ(0, hit_count);
        throw std::exception();
    } catch (...) {
    }

    EXPECT_EQ(0, hit_count);
    try {
        auto handler = std::move(*fail_holder);
        throw std::exception();
    } catch (...) {
    }
    EXPECT_EQ(1, hit_count);
}

TEST(scope, success_success) {
    int hit_count = 0;
    {
        auto handler = scope_success([&]{ ++hit_count; });
        EXPECT_EQ(0, hit_count);
    }
    EXPECT_EQ(1, hit_count);
}

TEST(scope, success_success_released) {
    int hit_count = 0;
    {
        auto handler = scope_success([&]{ ++hit_count; });
        EXPECT_EQ(0, hit_count);
        handler.release();
    }
    EXPECT_EQ(0, hit_count);
}

TEST(scope, success_success_move) {
    int hit_count = 0;
    auto handler_lambda = [&]{ ++hit_count; };
    using scope_success_type = scope_success<decltype(handler_lambda)>;

    std::optional<scope_success_type> success_holder;

    {
        auto handler = scope_success(std::move(handler_lambda));
        success_holder.emplace(std::move(handler));
        EXPECT_EQ(0, hit_count);
    }

    EXPECT_EQ(0, hit_count);
    success_holder.reset();
    EXPECT_EQ(1, hit_count);
}

TEST(scope, success_fail) {
    int hit_count = 0;

    try {
        auto handler = scope_success([&]{ ++hit_count; });
        EXPECT_EQ(0, hit_count);
        throw std::exception();
    } catch (...) {
    }

    EXPECT_EQ(0, hit_count);
}

TEST(scope, success_fail_released) {
    int hit_count = 0;

    try {
        auto handler = scope_success([&]{ ++hit_count; });
        EXPECT_EQ(0, hit_count);
        handler.release();
        throw std::exception();
    } catch (...) {
    }

    EXPECT_EQ(0, hit_count);
}

TEST(scope, success_fail_move) {
    int hit_count = 0;
    auto handler_lambda = [&]{ ++hit_count; };
    using scope_success_type = scope_success<decltype(handler_lambda)>;

    std::optional<scope_success_type> success_holder;

    try {
        auto handler = scope_success(std::move(handler_lambda));
        success_holder.emplace(std::move(handler));
        EXPECT_EQ(0, hit_count);
        throw std::exception();
    } catch (...) {
    }

    EXPECT_EQ(0, hit_count);
    try {
        auto handler = std::move(*success_holder);
        throw std::exception();
    } catch (...) {
    }
    EXPECT_EQ(0, hit_count);
}

} // End namespace pzl.
