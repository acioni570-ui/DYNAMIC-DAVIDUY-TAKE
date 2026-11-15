#include "../src/force_parameters.hpp"
#include <cassert>

struct Force {
    float m_force = 0, m_minForce = 0, m_maxForce = 0;
    bool m_relativeForce = false, m_forceRange = false;
    int m_forceID = 0;
    int activation = 0;
};
int main() {
    Force live{-12.5f, -4.f, 8.f, true, true, 42, 7};
    Force candidate;
    dynamic::copyForceParameters(candidate, live);
    assert(candidate.m_force == -12.5f && candidate.m_minForce == -4.f);
    assert(candidate.m_maxForce == 8.f && candidate.m_relativeForce && candidate.m_forceRange);
    assert(candidate.m_forceID == 42 && candidate.activation == 0 && live.activation == 7);
    candidate.m_force = 90.f;
    assert(live.m_force == -12.5f);
    // Changes to live configuration are read next time; no stale cached values.
    live = Force{3.f, 1.f, 6.f, false, false, 8, 9};
    dynamic::copyForceParameters(candidate, live);
    assert(candidate.m_force == 3.f && candidate.m_minForce == 1.f && candidate.m_maxForce == 6.f);
    assert(!candidate.m_relativeForce && !candidate.m_forceRange && candidate.m_forceID == 8);
}
