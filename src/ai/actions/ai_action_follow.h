#pragma once

#include "ai/action.h"
#include "scene/components.h"
#include "glm/glm.hpp"


class AIActionFollow : public goap::Action {
public:
    AIActionFollow(std::string name, int cost) : goap::Action(std::move(name), cost) {};

    AIActionFollow(std::string name, int cost, kubarem::Entity &target_entity) :
            goap::Action(std::move(name), cost, target_entity) {
        setPrecondition(target_entity.getComponent<kubarem::UuidComponent>().uuid.c_str(), "Dead", false);
        setEffect(target_entity.getComponent<kubarem::UuidComponent>().uuid.c_str(), "Dead", true);
    };

    bool perform(kubarem::Entity &applier, kubarem::Entity &target) const override;
};