#include <algorithm>

#include "log.h"
#include "world_state.h"

namespace goap {
void WorldState::setFact(const char* uuid_v4, const char* name, bool value)
{
    for (auto& v : facts)
        if (v.entity_uuid == uuid_v4 and v.name == name) {
            facts.erase(v);
            break;
        }
    facts.emplace(WorldFact(uuid_v4, name, value));
}

bool WorldState::isFactExist(const char* uuid_v4, const char* name) const
{
    for (auto const& value : facts)
        if (value.entity_uuid == uuid_v4 and value.name == name)
            return true;
    return false;
}

bool WorldState::operator==(const WorldState& other) const
{
    return (facts == other.facts);
}

bool WorldState::meetsGoal(const WorldState& goal_state) const
{
    std::set<WorldFact> diff;
    std::set_difference(goal_state.facts.begin(), goal_state.facts.end(),
        facts.begin(), facts.end(),
        std::inserter(diff, diff.begin()));
    return diff.empty();
}

int WorldState::distanceTo(const WorldState& goal_state) const
{
    std::set<WorldFact> diff;
    std::set_difference(goal_state.facts.begin(), goal_state.facts.end(),
        facts.begin(), facts.end(),
        std::inserter(diff, diff.begin()));
    return diff.size();
}

WorldState WorldState::distanceState(const WorldState& goal_state) const
{
    std::set<WorldFact> diff;
    std::set_difference(goal_state.facts.begin(), goal_state.facts.end(),
        facts.begin(), facts.end(),
        std::inserter(diff, diff.begin()));
    std::string label(std::string("Difference ") + std::string(this->name) + std::string(" - ") + std::string(goal_state.name));
    return WorldState(diff, label);
};

void WorldState::logDistance(const WorldState& goal_state) const
{
    WorldState diff = this->distanceState(goal_state);
    log_dbg("Meets goal: %d, Difference: %d", this->meetsGoal(goal_state), this->distanceTo(goal_state));
    for (auto& f : diff.facts) {
        log_dbg("Diff fact: %s %s %d", f.entity_uuid.c_str(), f.name.c_str(), f.value);
    }
    log_dbg("-------------------");
}

void WorldState::log() const
{
    for (auto& f : this->facts) {
        log_dbg("%s %s %d", f.entity_uuid.c_str(), f.name.c_str(), f.value);
    }
    log_dbg("-------------------");
}
}
