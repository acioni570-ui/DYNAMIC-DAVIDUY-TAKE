#pragma once

namespace dynamic {
// These are every ForceBlockGameObject physics setting in GD 2.2081. Copy
// values from the live object each candidate, without replacing native math.
template <typename Force>
void copyForceParameters(Force& dst, Force const& src) {
    dst.m_force = src.m_force;
    dst.m_minForce = src.m_minForce;
    dst.m_maxForce = src.m_maxForce;
    dst.m_relativeForce = src.m_relativeForce;
    dst.m_forceRange = src.m_forceRange;
    dst.m_forceID = src.m_forceID;
}
}
