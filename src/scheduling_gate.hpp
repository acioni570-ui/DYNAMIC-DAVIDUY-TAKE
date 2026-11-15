#pragma once
#include "optimizer_api.hpp"

namespace dynamic {
class SchedulingGate {
    optimizer::Predicate predicate = nullptr;
    bool const* active = nullptr;
    std::uint64_t suppressed = 0;
public:
    void connect(optimizer::Predicate value, bool const* scope = nullptr) {
        predicate = value;
        active = scope;
    }
    bool suppress(void* node) {
        if (!predicate || !node || (active && !*active) || !predicate(node)) return false;
        ++suppressed;
        return true;
    }
    std::uint64_t count() const { return suppressed; }
};
}
