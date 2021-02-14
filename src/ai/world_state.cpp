#include <algorithm>

#include "world_state.h"
#include "log.h"


namespace goap {
    void WorldState::setFact(const char* uuid_v4, const char* name, bool value) {
        for (auto& v : facts_)
            if (v.entity_uuid == uuid_v4 and v.name == name){
                facts_.erase(v);
                break;
            }
        facts_.emplace(WorldFact(uuid_v4, name, value));
    }


    bool WorldState::isFactExist(const char *uuid_v4, const char *name) const {
        for (auto const &value : facts_)
            if (value.entity_uuid == uuid_v4 and value.name == name)
                return true;
        return false;
    }


    bool WorldState::operator==(const WorldState &other) const {
        return (facts_ == other.facts_);
    }


    bool WorldState::meetsGoal(const WorldState &goal_state) const {
        std::set<WorldFact> diff;
        std::set_difference(goal_state.facts_.begin(), goal_state.facts_.end(),
                            facts_.begin(), facts_.end(),
                            std::inserter(diff, diff.begin()));
        return diff.empty();
    }


    int WorldState::distanceTo(const WorldState &goal_state) const {
        std::set<WorldFact> diff;
        std::set_difference(goal_state.facts_.begin(), goal_state.facts_.end(),
                            facts_.begin(), facts_.end(),
                            std::inserter(diff, diff.begin()));
        return diff.size();
    }

    WorldState WorldState::distanceState(const WorldState& goal_state) const {
        std::set<WorldFact> diff;
        std::set_difference(goal_state.facts_.begin(), goal_state.facts_.end(),
                            facts_.begin(), facts_.end(),
                            std::inserter(diff, diff.begin()));
        std::string label(std::string("Difference ") + std::string(this->name_) + std::string(" - ") + std::string(goal_state.name_));
        return WorldState(diff, label);
    };

    void WorldState::logDistance(const WorldState& goal_state) const {
        WorldState diff = this->distanceState(goal_state);
        log_dbg("Meets goal: %d, Difference: %d", this->meetsGoal(goal_state), this->distanceTo(goal_state));
        for(auto& f : diff.facts_) {
            log_dbg("Diff fact: %s %s %d", f.entity_uuid.c_str(), f.name.c_str(), f.value);
        }
        log_dbg("-------------------");
    }

    void WorldState::log() const{
        for(auto& f : this->facts_) {
            log_dbg("%s %s %d", f.entity_uuid.c_str(), f.name.c_str(), f.value);
        }
        log_dbg("-------------------");
    }
}
