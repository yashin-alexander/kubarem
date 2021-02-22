#pragma once

#include "ai/action.h"
#include "scene/components.h"

class AIActionNone : public goap::Action {
public:
    AIActionNone(std::string name, int cost) : goap::Action(std::move(name), cost) {};

    bool perform(kubarem::Entity &applier, kubarem::Entity &target) const override;
};

