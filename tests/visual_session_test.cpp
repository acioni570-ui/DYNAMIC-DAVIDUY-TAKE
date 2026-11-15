#include "../src/visual_session.hpp"
#include <cassert>

struct Node {
    bool visible = true;
    unsigned updates = 0;
    int collisionType = 47;
    bool disabled = false;
    void setVisible(bool value) { visible = value; }
    void update() { ++updates; }
};
int main() {
    dynamic::VisualSession session;
    assert(!session.suppress());
    session.entered(false, true); // failed startup must not affect the menu
    assert(!session.suppress());
    session.entered(true, false);
    assert(!session.suppress());
    session.entered(true, true);
    assert(session.suppress());
    Node particle, glow, geometry;
    std::array<Node*, 3> layers{&particle, nullptr, &glow};
    assert(dynamic::hideVisualLayers(layers) == 2);
    assert(!particle.visible && !glow.visible && geometry.visible);
    for (auto* n : {&particle, &glow, &geometry}) {
        n->update();
        assert(n->updates == 1 && n->collisionType == 47 && !n->disabled);
    }
    session.left();
    assert(!session.suppress());
    // Configuration is refreshed on re-entry, never inherited from a level.
    session.entered(true, false);
    assert(!session.suppress());
}
