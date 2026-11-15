#include "../src/object_catalog.hpp"
#include "../src/collision_policy.hpp"
#include <bit>
#include <cassert>
#include <initializer_list>
#include <limits>

enum class Type { Solid = 0, Hazard = 2, Decoration = 7, Slope = 25, AnimatedHazard = 47 };

int main() {
    unsigned count = 0;
    for (auto word : dynamic::objectCatalogBits) count += std::popcount(word);
    assert(count == 3993 && count == dynamic::objectCatalogCount);
    for (int id : {1, 8, 39, 660, 1594, 1755, 2069, 2866, 2926, 3645, 4539,
                   29, 30, 105, 744, 900, 915}) assert(dynamic::knownObjectID(id));
    for (int id : {-1, 0, 14, 4540, 99999, std::numeric_limits<int>::min(),
                   std::numeric_limits<int>::max()}) assert(!dynamic::knownObjectID(id));
    // Catalog membership is never permission to ignore or reinterpret an
    // engine collision. The same ID can have a different resolved mod type.
    for (int id : {8, 1594, 99999}) {
        (void)dynamic::knownObjectID(id);
        assert(dynamic::collisionCategory(Type::Hazard) == dynamic::CollisionCategory::geometry);
        assert(dynamic::collisionCategory(static_cast<Type>(40)) == dynamic::CollisionCategory::unsupported);
    }
}
