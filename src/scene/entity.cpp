#include "entity.h"

namespace kubarem {
    Entity::Entity(entt::entity handle, Scene *scene)
            : entityHandle_(handle), scene_(scene) {};

}
