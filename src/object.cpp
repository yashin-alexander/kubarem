#include "object.h"


Object::Object()
        : Position (0, 0),
          Size     (1, 1),
          Color    (1.0f),
          Rotation (0.0f),
          IsSolid  (false),
          Destroyed(false) { }


Object::Object(glm::vec2 pos, glm::vec2 size, glm::vec3 color)
        : Position (pos),
          Size     (size),
          Color    (color),
          Rotation (0.0f),
          IsSolid  (false),
          Destroyed(false) { }


void Object::Init()
{

}