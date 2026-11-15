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
    unsigned hidden = 0;
    std::vector<Node*> pending{root};
    while (!pending.empty()) {
        auto* node = pending.back();
        pending.pop_back();
        auto address = reinterpret_cast<std::uintptr_t>(node);
        if (std::binary_search(candidates.begin(), candidates.end(), address)) {
            if (auto* batch = asBatch(node)) {
                batch->setVisible(false);
                ++hidden;
            }
        }
        auto* children = node->getChildren();
        if (!children) continue;
        for (unsigned i = 0; i < children->count(); ++i) {
            auto* child = static_cast<Node*>(children->objectAtIndex(i));
            if (child) pending.push_back(child);
        }
    }
    return hidden;
}
}
