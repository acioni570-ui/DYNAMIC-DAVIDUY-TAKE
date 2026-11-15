#pragma once

#include <cstddef>
#include <deque>

namespace dynamic {
// Each active call gets its own buffer, including nested collision callbacks.
// Clearing retains capacity for subsequent simulation steps.
template <typename Buffer>
class ScratchBuffers {
    std::deque<Buffer> buffers;
    std::size_t depth = 0;

public:
    class Lease {
        ScratchBuffers& owner;
        Buffer& buffer;

    public:
        explicit Lease(ScratchBuffers& pool)
            : owner(pool), buffer(pool.next()) {}
        Lease(Lease const&) = delete;
        Lease& operator=(Lease const&) = delete;
        ~Lease() { buffer.clear(); --owner.depth; }
        Buffer& get() { return buffer; }
    };

    Lease acquire() { return Lease(*this); }

private:
    Buffer& next() {
        if (depth == buffers.size()) buffers.emplace_back();
        return buffers[depth++];
    }
};
}
