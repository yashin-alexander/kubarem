#include "ai/actions/ai_action_follow.h"

bool AIActionFollow::perform(kubarem::Entity& applier, kubarem::Entity& target) const
{
    float speed = 0.5f;
    if (target.getComponent<kubarem::UuidComponent>().uuid != target_entity_id_)
        return false;
    auto& applier_transform = applier.getComponent<kubarem::TransformComponent>();
    auto& target_transform = target.getComponent<kubarem::TransformComponent>();
    glm::vec3 distance = glm::vec3(target_transform.position) - glm::vec3(applier_transform.position);
    if (glm::length(distance) >= speed) {
        applier_transform.is_looking_at = true;
        applier_transform.look_at = target_transform.position;
        applier_transform.position += glm::normalize(distance) * speed;
        return false;
    }
    return true;
}