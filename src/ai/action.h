#pragma once

#include <string>
#include <unordered_map>

#include "world_state.h"


namespace goap {
    class Action {
    private:
        std::string name_;
        int cost_;

    public:
        WorldState * effects_;
        WorldState * preconditions_;
        Action(std::string name, int cost);

        bool operableOn(const goap::WorldState& ws) const;

        WorldState actOn(const WorldState& ws) const;

        void setPrecondition(const char * uuid_v4 , const char * name, bool value) const{
            preconditions_->setFact(uuid_v4, name, value);
        }

        void setEffect(const char * uuid_v4 , const char * name, bool value) const{
            effects_->setFact(uuid_v4, name, value);
        }

        int cost() const { return cost_; }
        std::string name() const { return name_; }

        void log() const;
    };
}