#pragma once

namespace pzl::glfw {

struct context {
public:
    context();
    context(ref<const context>) = delete;
    auto operator=(ref<const context>) -> ref<context> = delete;
    context(rref<context> other);
    auto operator=(rref<context> rhs) -> ref<context>;
    ~context();

private:
    bool active_;
};

} // End namespace pzl::glfw.
