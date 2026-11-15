#pragma once
#include "optimizer_api.hpp"

namespace dynamic {
class SchedulingGate {
    optimizer::Predicate predicate = nullptr;
    std::uint64_t suppressed = 0;
public:
    void connect(optimizer::Predicate value) { predicate = value; }
    bool suppress(void* node) {
        if (!predicate || !node || !predicate(node)) return false;
        ++suppressed;
        return true;
    }
    std::uint64_t count() const { return suppressed; }
};
}
