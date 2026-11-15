#include "../src/scratch_buffers.hpp"

#include <cassert>
#include <memory>
#include <stdexcept>
#include <vector>

struct AllocationCount { static inline unsigned value = 0; };
template <typename T>
struct CountingAllocator {
    using value_type = T;
    CountingAllocator() = default;
    template <typename U> CountingAllocator(CountingAllocator<U> const&) {}
    T* allocate(std::size_t n) {
        ++AllocationCount::value;
        return std::allocator<T>{}.allocate(n);
    }
    void deallocate(T* ptr, std::size_t n) { std::allocator<T>{}.deallocate(ptr, n); }
    template <typename U> bool operator==(CountingAllocator<U> const&) const { return true; }
};

int main() {
    using Buffer = std::vector<int, CountingAllocator<int>>;
    dynamic::ScratchBuffers<Buffer> pool;

    for (int step = 0; step < 1000; ++step) {
        auto lease = pool.acquire();
        auto& buffer = lease.get();
        assert(buffer.empty());
        buffer.reserve(64);
        buffer.push_back(step);
    }
    assert(AllocationCount::value == 1);

    {
        auto outer = pool.acquire();
        outer.get().push_back(7);
        try {
            auto inner = pool.acquire();
            assert(inner.get().empty());
            inner.get().reserve(64);
            inner.get().push_back(42);
            assert(outer.get().size() == 1 && outer.get()[0] == 7);
            throw std::runtime_error("nested collision interrupted");
        } catch (std::runtime_error const&) {}
        auto inner = pool.acquire();
        assert(inner.get().empty());
        inner.get().push_back(9);
        assert(outer.get()[0] == 7);
    }
    assert(AllocationCount::value == 2);
    auto last = pool.acquire();
    assert(last.get().empty());
    last.get().push_back(1);
    assert(AllocationCount::value == 2);
}
