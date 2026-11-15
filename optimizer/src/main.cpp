#include <Geode/Geode.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include "../../src/scheduling_gate.hpp"

using namespace geode::prelude;
namespace { dynamic::SchedulingGate gate; }

extern "C" __declspec(dllexport) void dynamic_optimizer_connect_v1(dynamic::optimizer::Predicate predicate) {
    gate.connect(predicate);
}
extern "C" __declspec(dllexport) std::uint64_t dynamic_optimizer_count_v1() {
    return gate.count();
}

// Private prediction timers are removed before returning to the scheduler.
// Avoid their insertion. Non-node targets and live nodes always pass through.
class $modify(DynamicOptimizerScheduler, CCScheduler) {
    void scheduleSelector(SEL_SCHEDULE selector, CCObject* target, float interval, bool paused) {
        if (!gate.suppress(target)) CCScheduler::scheduleSelector(selector, target, interval, paused);
    }
    void scheduleSelector(SEL_SCHEDULE selector, CCObject* target, float interval,
                          unsigned repeat, float delay, bool paused) {
        if (!gate.suppress(target))
            CCScheduler::scheduleSelector(selector, target, interval, repeat, delay, paused);
    }
};
