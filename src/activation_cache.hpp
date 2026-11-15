#pragma once

namespace dynamic {
// The map lives for one input search. Candidate generations keep activation
// independent without freeing and reallocating a hash node for every replay.
// Clear at the search boundary to bound retained entries to that search.
template <typename Map, typename Key, typename Generation>
bool firstActivation(Map& seen, Key const& key, Generation generation) {
    auto [entry, inserted] = seen.try_emplace(key, generation);
    if (inserted) return true;
    if (entry->second == generation) return false;
    entry->second = generation;
    return true;
}
}
