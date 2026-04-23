#include "components/component.hpp"
#include <raylib.h>

Vector2 Component::GetAbsolutePosition() const {
  int screen_width = GetScreenWidth();
  int screen_height = GetScreenHeight();
  Vector2 absolute_size = GetAbsoluteSize();

  return Vector2{screen_width * position.relative_x + position.absolute_x +
                     absolute_size.x * pivot.x,
                 screen_height * position.relative_y + position.absolute_y +
                     absolute_size.y * pivot.y};
}

Vector2 Component::GetAbsoluteSize() const {
  int screen_width = GetScreenWidth();
  int screen_height = GetScreenHeight();

  return Vector2{screen_width * size.relative_x + size.absolute_x,
                 screen_height * size.relative_y + size.absolute_y};
}
