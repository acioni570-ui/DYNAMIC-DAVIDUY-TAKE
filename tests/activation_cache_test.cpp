#include "../src/activation_cache.hpp"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>

struct Allocations { static inline unsigned count = 0; };
template <typename T> struct Allocator {
    using value_type = T;
    Allocator() = default;
    template <typename U> Allocator(Allocator<U> const&) {}
    T* allocate(std::size_t n) { ++Allocations::count; return std::allocator<T>{}.allocate(n); }
    void deallocate(T* p, std::size_t n) { std::allocator<T>{}.deallocate(p, n); }
    template <typename U> bool operator==(Allocator<U> const&) const { return true; }
};

int main() {
    using Map = std::unordered_map<int, std::uint64_t, std::hash<int>, std::equal_to<int>,
        Allocator<std::pair<int const, std::uint64_t>>>;
    std::unordered_set<int, std::hash<int>, std::equal_to<int>, Allocator<int>> baseline;
    Map cache;
    baseline.reserve(24); cache.reserve(24);
    Allocations::count = 0;
    // Compare the original clear-and-replay contract against candidate epochs,
    // including different overlap orders and a second player replay.
    for (std::uint64_t generation = 1; generation <= 130; ++generation) {
        baseline.clear();
        for (int step = 0; step < 96; ++step) {
            int id = (step + static_cast<int>(generation)) % 24;
            assert(baseline.insert(id).second == dynamic::firstActivation(cache, id, generation));
        }
    }
    // Count separately to avoid mixing baseline and optimized allocations.
    cache.clear(); Allocations::count = 0;
    for (std::uint64_t generation = 1; generation <= 65; ++generation)
        for (int id = 0; id < 24; ++id)
            assert(dynamic::firstActivation(cache, id, generation));
    auto optimized = Allocations::count;
    baseline.clear(); Allocations::count = 0;
    for (unsigned attempt = 0; attempt < 65; ++attempt) {
        baseline.clear();
        for (int id = 0; id < 24; ++id) assert(baseline.insert(id).second);
    }
    assert(optimized == 24 && Allocations::count == 65 * 24);
    std::cout << "Portal node allocations: " << Allocations::count << " -> " << optimized << '\n';
    // Search reset releases keys; a reused object address/key is fresh even
    // if the caller reuses a previous generation after resetting the search.
    cache.clear();
    assert(dynamic::firstActivation(cache, 0, std::uint64_t{1}));
    assert(!dynamic::firstActivation(cache, 0, std::uint64_t{1}));
    assert(dynamic::firstActivation(cache, 0, std::uint64_t{2}));
    assert(dynamic::firstActivation(cache, 99, std::uint64_t{2}));
}
