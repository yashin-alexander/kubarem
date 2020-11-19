#include "object.h"


Object::Object(GLfloat screen_scale,
               Shader * shader_program,
               Camera * camera,
               const glm::vec3 * light_point,
               glm::vec3 position,
               glm::vec3 size):
  screen_scale_(screen_scale),
  shader_program_(shader_program),
  camera_(camera),
  light_point_(light_point),
  position(position),
  size(size)
{}


Object::~Object()
{}


void Object::Render()
{}


void Object::DoCollisions()
{}
