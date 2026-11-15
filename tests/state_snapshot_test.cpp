#include "../src/scoped_restore.hpp"
#include <cassert>
#include <map>
#include <memory>
#include <cstdio>
#include <stdexcept>
#include <vector>

inline unsigned allocations = 0;
template <typename T>
struct CountingAllocator {
    using value_type = T;
    CountingAllocator() = default;
    template <typename U> CountingAllocator(CountingAllocator<U> const&) {}
    T* allocate(std::size_t n) { ++allocations; return std::allocator<T>{}.allocate(n); }
    void deallocate(T* p, std::size_t n) { std::allocator<T>{}.deallocate(p, n); }
    bool operator==(CountingAllocator const&) const { return true; }
};
struct Counted {
    static inline unsigned copies = 0;
    std::vector<int, CountingAllocator<int>> values = decltype(values)(1024, 7);
    Counted() = default;
    Counted(Counted const& rhs) : values(rhs.values) { ++copies; }
    Counted& operator=(Counted const&) = default;
    Counted(Counted&&) = default;
    Counted& operator=(Counted&&) = default;
};
struct State {
    State() = default;
    State(State const&) = delete;
    int mode = 1;
    Counted objects;
    std::map<int, bool> active{{7, true}};
};
int main() {
    State baseline, optimized;
    Counted::copies = 0;
    allocations = 0;
    for (int i = 0; i < 65; ++i) {
        dynamic::ScopedRestore guard(baseline, &State::objects);
        baseline.objects.values.assign(2048, i);
    }
    assert(Counted::copies == 65);
    auto baselineAllocations = allocations;
    assert(baselineAllocations == 130);
    allocations = 0;
    Counted::copies = 0;
    dynamic::StateSnapshot snapshot(optimized, &State::mode, &State::objects, &State::active);
    for (int i = 0; i < 65; ++i) {
        try {
            auto guard = snapshot.guard();
            assert(optimized.mode == 1 && optimized.objects.values[0] == 7);
            assert(optimized.active.size() == 1 && optimized.active.at(7));
            optimized.mode = i;
            optimized.objects.values.assign(2048, i);
            optimized.active.clear();
            optimized.active[9] = false;
            if (i % 2) throw std::runtime_error("deadline or unknown interaction");
        } catch (std::runtime_error const&) {}
        assert(optimized.mode == 1);
        assert(optimized.objects.values == baseline.objects.values);
        assert(optimized.active.size() == 1 && optimized.active.at(7));
    }
    assert(Counted::copies == 1); // single backup, identical restored state
    assert(allocations == 2); // backup plus one growth, then reuse capacity
    std::printf("65-candidate fixture: vector allocations %u -> %u; backup constructions 65 -> 1\n",
        baselineAllocations, allocations);
}
