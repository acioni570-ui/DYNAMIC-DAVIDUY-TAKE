#include "../src/collision_policy.hpp"
#include <cassert>
#include <initializer_list>

// Values and names from Geode 5.11.2's GameObjectType. Production code
// instantiates the same policy with the actual SDK enum.
enum class ObjectType {
    Solid = 0, Hazard = 2, Decoration = 7, Slope = 25, Special = 40, AnimatedHazard = 47
};

int main() {
    for (int id : {29, 30, 105, 744, 899, 900, 915}) assert(dynamic::colorOnlyTrigger(id));
    for (int id : {901, 1049, 1006, 1007, 1268, 1817, 2067, -1, 99999})
        assert(!dynamic::colorOnlyTrigger(id));
    // Audited player contact blocks get native contact handling. Other
    // Special IDs and mismatched object types remain unsupported.
    for (int id : {1755, 1813, 1829, 1859, 2069, 2866, 3645}) {
        assert(dynamic::nativeContactBlock(ObjectType::Special, id));
        for (int type = -1; type <= 48; ++type)
            if (type != 40) assert(!dynamic::nativeContactBlock(static_cast<ObjectType>(type), id));
    }
    for (int id : {-1, 1754, 1812, 1830, 2070, 2867, 3646, 99999})
        assert(!dynamic::nativeContactBlock(ObjectType::Special, id));
    using dynamic::CollisionCategory;
    using dynamic::collisionCategory;
    assert(collisionCategory(ObjectType::Solid) == CollisionCategory::geometry);
    assert(collisionCategory(ObjectType::Hazard) == CollisionCategory::geometry);
    assert(collisionCategory(ObjectType::AnimatedHazard) == CollisionCategory::geometry);
    assert(collisionCategory(ObjectType::Slope) == CollisionCategory::slope);
    assert(collisionCategory(ObjectType::Decoration) == CollisionCategory::decoration);
    // Portals, rings, pads, breakable objects and unknown future types must not
    // be mistaken for safe geometry or decoration.
    for (int value = -1; value <= 48; ++value) {
        if (value == 0 || value == 2 || value == 7 || value == 25 || value == 47) continue;
        assert(collisionCategory(static_cast<ObjectType>(value)) == CollisionCategory::unsupported);
    }
    assert(collisionCategory(static_cast<ObjectType>(9999)) == CollisionCategory::unsupported);
}
