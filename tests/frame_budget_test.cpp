#include "../src/frame_budget.hpp"
#include <cassert>
#include <stdexcept>

struct Clock {
    using duration = std::chrono::milliseconds;
    using time_point = std::chrono::time_point<Clock>;
    static inline time_point current{};
    static time_point now() { return current; }
};
using namespace std::chrono_literals;
int main() {
    dynamic::FrameBudget<Clock> budget;
    {
        auto first = budget.start(1, 12ms, 4ms);
        assert(first.deadline() - first.started() == 4ms);
        Clock::current += 3ms;
    }
    {
        auto second = budget.start(1, 12ms, 4ms);
        assert(second.deadline() - second.started() == 1ms);
        Clock::current += 2ms; // native call can overrun
    }
    {
        auto exhausted = budget.start(1, 12ms, 4ms);
        assert(exhausted.started() == exhausted.deadline());
    }
    {
        auto next = budget.start(2, 2ms, 4ms);
        assert(next.deadline() - next.started() == 2ms);
    }
    {
        auto unlimited = budget.start(2, 12ms, 0ms);
        assert(unlimited.deadline() - unlimited.started() == 12ms);
        Clock::current += 20ms;
    }
    try {
        auto exception = budget.start(3, 12ms, 4ms);
        Clock::current += 4ms;
        throw std::runtime_error("interrupted");
    } catch (std::runtime_error const&) {}
    {
        auto exhausted = budget.start(3, 12ms, 4ms);
        assert(exhausted.started() == exhausted.deadline());
    }
    {
        auto wrappedFrame = budget.start(0, 12ms, 4ms);
        assert(wrappedFrame.deadline() - wrappedFrame.started() == 4ms);
    }
}
