#pragma once

namespace dynamic {
// Use the game's resolved type, never the editor ID: many IDs share one
// collision behavior, and different objects can have very different effects.
// Color-only triggers identified by GameObject::isColorTrigger in the pinned
// SDK. Do not include move/toggle/spawn or unknown Modifier IDs here.
constexpr bool colorOnlyTrigger(int id) {
    return id == 29 || id == 30 || id == 105 || id == 744 ||
        id == 899 || id == 900 || id == 915;
}

// Audited GD 2.2081 PlayerObject::touchedObject cases. Native physics owns
// contact counters, stopping dash and force application; use private copies.
constexpr bool nativeForceBlock(int id) {
    return id == 2069 || id == 3645;
}
template <typename Type>
constexpr bool nativeContactBlock(Type type, int id) {
    return type == Type::Special && (id == 1755 || id == 1813 ||
        id == 1829 || id == 1859 || id == 2866 || nativeForceBlock(id));
}

enum class CollisionCategory { geometry, slope, decoration, unsupported };

template <typename Type>
constexpr CollisionCategory collisionCategory(Type type) {
    if (type == Type::Solid || type == Type::Hazard || type == Type::AnimatedHazard)
        return CollisionCategory::geometry;
    if (type == Type::Slope) return CollisionCategory::slope;
    if (type == Type::Decoration) return CollisionCategory::decoration;
    return CollisionCategory::unsupported;
}
}
