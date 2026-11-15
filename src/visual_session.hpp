#pragma once
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

namespace dynamic {
class VisualSession {
    bool active = false;
public:
    void entered(bool success, bool enabled) { active = success && enabled; }
    void left() { active = false; }
    bool suppress() const { return active; }
};

// Some native layer fields can contain non-null garbage. Read only their byte
// representation: never dereference a candidate until a live tree node with
// the same address has been found. A null check cannot validate native pointers.
template <typename T>
std::uintptr_t visualAddress(T* const* field) {
    static_assert(sizeof(T*) == sizeof(std::uintptr_t));
    std::uintptr_t address = 0;
    std::memcpy(&address, field, sizeof(address));
    return address;
}

// Traverse nodes supplied by the engine, not values read from layer fields.
// `asBatch` checks type before visibility is changed. All storage is temporary
// and this runs once after level init, never per frame.
template <typename Node, std::size_t N, typename AsBatch>
unsigned hideVisualLayers(Node* root, std::array<std::uintptr_t, N> candidates, AsBatch asBatch) {
    if (!root) return 0;
    std::sort(candidates.begin(), candidates.end());
    auto end = std::unique(candidates.begin(), candidates.end());
    auto begin = std::upper_bound(candidates.begin(), end, std::uintptr_t{0});
    if (begin == end) return 0;
    unsigned hidden = 0;
    // Keep only ancestor continuations. The old stack enqueued every child,
    // allocating in proportion to wide sprite batches (thousands of nodes).
    struct Frame {
        decltype(root->getChildren()) children;
        unsigned next;
        unsigned count;
    };
    std::vector<Frame> pending;
    auto visit = [&](Node* node) {
        auto address = reinterpret_cast<std::uintptr_t>(node);
        auto found = std::lower_bound(begin, end, address);
        if (found != end && *found == address) {
            if (auto* batch = asBatch(node)) {
                batch->setVisible(false);
                ++hidden;
            }
            end = std::move(found + 1, end, found);
            if (begin == end) return true;
        }
        auto* children = node->getChildren();
        if (children && children->count())
            pending.push_back({children, 0, children->count()});
        return false;
    };
    if (visit(root)) return hidden;
    while (!pending.empty()) {
        auto& frame = pending.back();
        if (frame.next == frame.count) {
            pending.pop_back();
            continue;
        }
        auto* child = static_cast<Node*>(frame.children->objectAtIndex(frame.next++));
        if (child && visit(child)) break;
    }
    return hidden;
}
}
