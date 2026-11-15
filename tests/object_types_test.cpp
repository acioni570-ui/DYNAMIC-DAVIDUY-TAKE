#include "../src/object_types.hpp"
#include <cassert>
#include <optional>

struct Base { virtual ~Base() = default; };
struct Effect : virtual Base { int group = 0; };
struct Enhanced : virtual Base { int activated = 0; };
struct Ring : Effect, Enhanced { int claim = 0; };
struct Force : Effect {};
struct Dash : Ring { int speed = 0; };
using Types = dynamic::ObjectTypes<Effect, Enhanced, Ring, Dash, Force>;

int main() {
    Dash source, copy;
    Base plain;
    unsigned casts = 0;
    auto inspect = [&]<typename T>(Base* object) -> T* {
        ++casts;
        return dynamic_cast<T*>(object);
    };
    std::optional<Types> srcTypes, dstTypes;
    for (int candidate = 0; candidate < 65; ++candidate) {
        for (int step = 0; step < 32; ++step) {
            if (!srcTypes) srcTypes = Types::inspect(static_cast<Base*>(&source), inspect);
            if (!dstTypes) dstTypes = Types::inspect(static_cast<Base*>(&copy), inspect);
            source.group = candidate * 32 + step;
            dstTypes->effect->group = srcTypes->effect->group;
            assert(copy.group == source.group); // fields are not cached
        }
    }
    assert(casts == 10); // five casts per object, not per simulation step
    assert(srcTypes->ring == static_cast<Ring*>(&source));
    assert(srcTypes->enhanced == static_cast<Enhanced*>(&source));
    assert(srcTypes->dash == &source && dstTypes->dash == &copy);
    auto missing = Types::inspect(&plain, inspect);
    assert(!missing.effect && !missing.enhanced && !missing.ring && !missing.dash && !missing.force);
    Force force;
    auto forceTypes = Types::inspect(static_cast<Base*>(&force), inspect);
    assert(forceTypes.force == &force && forceTypes.effect == static_cast<Effect*>(&force));
    assert(!forceTypes.ring && !forceTypes.dash);
    Effect effectOnly;
    auto partial = Types::inspect(static_cast<Base*>(&effectOnly), inspect);
    assert(partial.effect == &effectOnly && !partial.enhanced && !partial.ring && !partial.dash && !partial.force);
}
