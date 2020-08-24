#include "object.h"


Object_::Object_(GLfloat screen_scale,
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


Object_::~Object_()
{}


void Object_::Render()
{}


void Object_::DoCollisions()
{}
