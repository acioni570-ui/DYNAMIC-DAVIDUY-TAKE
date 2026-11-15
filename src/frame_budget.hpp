#pragma once
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <optional>

namespace dynamic {
// Charge setup, native simulation, cleanup and reporting to the same frame.
// This is a cooperative deadline: a native call cannot be preempted.
template <typename Clock>
class FrameBudget {
    using Duration = typename Clock::duration;
    using Time = typename Clock::time_point;
    std::optional<std::uint64_t> frame;
    Duration spent{};
public:
    class Attempt {
        FrameBudget& owner;
        Time begin;
        Time end;
    public:
        Attempt(FrameBudget& owner, Duration allowed)
            : owner(owner), begin(Clock::now()), end(begin + allowed) {}
        Attempt(Attempt const&) = delete;
        Attempt& operator=(Attempt const&) = delete;
        ~Attempt() { owner.spent += Clock::now() - begin; }
        Time started() const { return begin; }
        Time deadline() const { return end; }
    };

    Attempt start(std::uint64_t currentFrame, Duration inputLimit, Duration frameLimit) {
        if (!frame || *frame != currentFrame) {
            frame = currentFrame;
            spent = Duration::zero();
        }
        auto allowed = std::max(Duration::zero(), inputLimit);
        if (frameLimit > Duration::zero())
            allowed = std::min(allowed, std::max(Duration::zero(), frameLimit - spent));
        return Attempt(*this, allowed);
    }
};
}
