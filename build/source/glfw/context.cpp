#include <atomic>

#include "context.hpp"

namespace pzl::glfw {

static std::atomic<bool> global_active = false;

context::context() {
    if (global_active.exchange(true) == true) {
        throw std::runtime
    }

    active_ = true;
}

context::context(rref<context> other)
        : active_(std::exchange(other.active_, false)) {
}

ref<context> context::operator=(rref<context> rhs)
        : active_(std::exchange(rhs.active_, false)) {
}

context::~context() {
    if (active_) {
        if (global_active.exchange(false) == false) {
            // A logic error has occurred. Terminate? Log?
        }
    }
}

} // End namespace pzl::glfw.
