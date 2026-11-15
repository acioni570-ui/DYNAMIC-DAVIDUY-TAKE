#include "../src/scheduling_gate.hpp"
#include <cassert>

struct Node { Node* parent = nullptr; };
Node root, live;
bool simulating = false;
bool owned(void* raw) {
    if (!simulating) return false;
    for (auto* n = static_cast<Node*>(raw); n; n = n->parent)
        if (n == &root) return true;
    return false;
}
int main() {
    dynamic::SchedulingGate gate;
    Node child{&root}, nested{&child}, normalChild{&live};
    assert(!gate.suppress(&root)); // companion inert without handshake
    gate.connect(&owned);
    assert(!gate.suppress(&root) && !gate.suppress(nullptr));
    simulating = true;
    assert(gate.suppress(&root) && gate.suppress(&nested));
    assert(!gate.suppress(&normalChild) && !gate.suppress(&live));
    assert(gate.count() == 2);
    simulating = false;
    assert(!gate.suppress(&nested) && gate.count() == 2);
    gate.connect(nullptr);
    simulating = true;
    assert(!gate.suppress(&nested));
}
