#pragma once
#include <array>
#include <cstddef>

namespace dynamic {
// Level-entry configuration: no settings lookup on a rendering/effect call.
class VisualSession {
    bool active = false;
public:
    void entered(bool success, bool enabled) { active = success && enabled; }
    void left() { active = false; }
    bool suppress() const { return active; }
};

// Configure native batch layers once. Do not modify any object/physics flags.
template <typename Node, std::size_t N>
unsigned hideVisualLayers(std::array<Node*, N> const& layers) {
    unsigned hidden = 0;
    for (auto* layer : layers) {
        if (!layer) continue;
        layer->setVisible(false);
        ++hidden;
    }
    return hidden;
}
}
