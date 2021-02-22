#include "ai/actions/ai_action_none.h"

bool AIActionNone::perform(kubarem::Entity &applier, kubarem::Entity &target) const {
    if (target.getComponent<kubarem::UuidComponent>().uuid != target_entity_id_) {
        return false;
    }
    log_err("AI: Action: perform function is not defined");
    return false;
}

