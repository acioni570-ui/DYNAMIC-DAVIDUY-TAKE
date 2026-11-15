#pragma once
#include <cstdint>

// Versioned C ABI shared by the optional Windows companion and Dynamic Assist.
// The predicate receives a CCObject scheduler target; neither side transfers ownership.
namespace dynamic::optimizer {
using Predicate = bool (*)(void*);
using Connect = void (*)(Predicate);
using Count = std::uint64_t (*)();
inline constexpr char connectSymbol[] = "dynamic_optimizer_connect_v1";
inline constexpr char countSymbol[] = "dynamic_optimizer_count_v1";
}
