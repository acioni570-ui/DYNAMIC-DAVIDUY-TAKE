#include "../src/visual_session.hpp"
#include <cassert>
#include <vector>

struct Node {
    struct Children {
        std::vector<Node*> nodes;
        unsigned count() const { return static_cast<unsigned>(nodes.size()); }
        Node* objectAtIndex(unsigned i) { return nodes.at(i); }
    } children;
    bool batch = false;
    bool visible = true;
    unsigned updates = 0;
    int collisionType = 47;
    bool disabled = false;
    void setVisible(bool value) { visible = value; }
    void update() { ++updates; }
    Children* getChildren() { return &children; }
};
int main() {
    dynamic::VisualSession session;
    assert(!session.suppress());
    session.entered(false, true);
    assert(!session.suppress());
    session.entered(true, false);
    assert(!session.suppress());
    session.entered(true, true);
    assert(session.suppress());

    Node root, particle, glow, geometry, unrelated;
    particle.batch = glow.batch = unrelated.batch = true;
    root.children.nodes = {&particle, &geometry};
    geometry.children.nodes = {&glow};
    Node* particleField = &particle;
    Node* glowField = &glow;
    Node* unrelatedField = &unrelated; // live but outside this tree
    Node* geometryField = &geometry; // in tree but not a batch
    Node* nullField = nullptr;
    std::array<std::uintptr_t, 7> layers{
        dynamic::visualAddress(&particleField), dynamic::visualAddress(&glowField),
        dynamic::visualAddress(&unrelatedField), dynamic::visualAddress(&geometryField),
        dynamic::visualAddress(&nullField), dynamic::visualAddress(&particleField),
        static_cast<std::uintptr_t>(0x2020202020202020ULL) // crash report's invalid pointer
    };
    unsigned casts = 0;
    auto asBatch = [&](Node* node) -> Node* {
        ++casts;
        assert(node == &particle || node == &glow || node == &geometry);
        return node->batch ? node : nullptr;
    };
    assert(dynamic::hideVisualLayers(&root, layers, asBatch) == 2);
    assert(casts == 3); // no lookup on invalid or out-of-tree candidates
    assert(!particle.visible && !glow.visible && geometry.visible && unrelated.visible);
    assert(dynamic::hideVisualLayers<Node>(nullptr, layers, asBatch) == 0);
    for (auto* n : {&particle, &glow, &geometry}) {
        n->update();
        assert(n->updates == 1 && n->collisionType == 47 && !n->disabled);
    }
    session.left();
    assert(!session.suppress());
    session.entered(true, false);
    assert(!session.suppress());
}
