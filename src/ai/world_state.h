#pragma once

#include <map>
#include <set>
#include <cstdio>
#include <string>
#include <ostream>
#include <functional>

#include "log.h"


namespace goap {
    struct WorldFact {
    private:
        std::string id_;
    public:
        std::string entity_uuid;
        std::string name;
        bool value;

        WorldFact(const char * uuid_v4 , const char * name, bool value){
            this->entity_uuid = uuid_v4;
            this->name = name;
            this->value = value;
            id_ = this->entity_uuid + this->name + std::to_string(this->value);
        }

        bool operator==(const WorldFact& other) const{
            return other.id_ == this->id_;
        }

        bool operator<(const WorldFact& rhs) const
        {
            std::hash<std::string> hasher;
            return hasher(id_) < hasher(rhs.id_);
        }
    };

    struct WorldState {
        std::set<WorldFact>facts_;
        std::string name_;

        explicit WorldState(std::string name):
        facts_(std::set<WorldFact>()), name_(std::move(name)) {};

        explicit WorldState(std::set<WorldFact> world_facts, std::string name):
        facts_(std::move(world_facts)), name_(std::move(name)) {};

        void setFact(const char * uuid_v4 , const char * name, bool value);
        bool isFactExist(const char * uuid_v4 , const char * name) const;

        bool meetsGoal(const WorldState& goal_state) const;
        int distanceTo(const WorldState& goal_state) const;
        WorldState distanceState(const WorldState& goal_state) const;
        void logDistance(const WorldState& goal_state) const;
        void log() const;

        bool operator==(const WorldState& other) const;
    };
}