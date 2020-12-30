#include "scene/scene.h"
#include "scene/components.h"
#include "scene/entity.h"


namespace kubarem{
    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = { registry_.create(), this };
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.tag = name.empty() ? "Noname entity" : name;

        return entity;
    }
}