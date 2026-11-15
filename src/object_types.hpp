#pragma once

namespace dynamic {
// A level object and its retained private copy keep the same C++ types for
// their lifetimes. Cache only type identity; read mutable fields every time.
template <typename Effect, typename Enhanced, typename Ring, typename Dash>
struct ObjectTypes {
    Effect* effect;
    Enhanced* enhanced;
    Ring* ring;
    Dash* dash;

    template <typename Base, typename Cast>
    static ObjectTypes inspect(Base* object, Cast&& cast) {
        return {cast.template operator()<Effect>(object),
                cast.template operator()<Enhanced>(object),
                cast.template operator()<Ring>(object),
                cast.template operator()<Dash>(object)};
    }
};
}
