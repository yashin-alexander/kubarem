#include <algorithm>

#include "action.h"
#include "world_state.h"


namespace goap {
    Action::Action(std::string name, int cost): name_(std::move(name)), cost_(cost) {
        this->preconditions_ = new WorldState("");
        this->effects_ = new WorldState("");
    }

    bool Action::operableOn(const WorldState& ws) const {
        return ws.meetsGoal(*this->preconditions_);
    }

    WorldState Action::actOn(const WorldState& ws) const {
        if (! operableOn(ws)){
            log_err("AI: action '%s' can't operate on '%s'", name().c_str(), ws.name.c_str());
        }

        WorldState tmp(ws);
        for (const auto& effect : effects_->facts) {
            tmp.setFact(effect.entity_uuid.c_str(), effect.name.c_str(), effect.value);
        }
        return tmp;
    }

    void Action::log() const {
        log_dbg("%s Preconditions:", name_.c_str());
        this->preconditions_->log();
        log_dbg("%s Effects:", name_.c_str());
        this->effects_->log();
    }
}
