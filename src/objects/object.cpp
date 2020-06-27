#include "object.h"


Object_::Object_(Shader * shader_program,
                 Camera * camera,
                 glm::vec3 light_point,
                 glm::vec3 position,
                 glm::vec3 size):
  shader_program_(shader_program),
  camera_(camera),
  light_point_(light_point),
  position(position),
  size(size)
{}


Object_::~Object_()
{}


void Object_::Render()
{}


void Object_::DoCollisions()
{}
