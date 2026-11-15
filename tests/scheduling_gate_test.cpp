#include "../src/scheduling_gate.hpp"
#include <cassert>

struct Node { Node* parent = nullptr; };
Node root, live;
bool simulating = false;
unsigned callbacks = 0;
bool owned(void* raw) {
    ++callbacks;
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
    gate.connect(&owned, &simulating);
    simulating = false;
    auto before = callbacks;
    for (unsigned i = 0; i < 10000; ++i) assert(!gate.suppress(&nested));
    assert(callbacks == before); // V2 never crosses the ABI outside a prediction
    simulating = true;
    assert(gate.suppress(&nested) && !gate.suppress(&normalChild));
    assert(callbacks == before + 2 && gate.count() == 3);
    gate.connect(&owned); // older Assist retains V1 behavior
    simulating = false;
    assert(!gate.suppress(&nested) && callbacks == before + 3);
}
