#include "components/component.hpp"
#include <raylib.h>

Vector2 Component::GetAbsolutePosition() const {
  int screen_width = GetScreenWidth();
  int screen_height = GetScreenHeight();

  return Vector2{screen_width * position.scale_x + position.offset_x,
                 screen_height * position.scale_y + position.offset_y};
}

Vector2 Component::GetAbsoluteSize() const {
  int screen_width = GetScreenWidth();
  int screen_height = GetScreenHeight();

  return Vector2{screen_width * size.scale_x + size.offset_x,
                 screen_height * size.scale_y + size.offset_y};
}

Rectangle Component::GetAbsoluteBounds() const {
  Vector2 absolute_position = GetAbsolutePosition();
  Vector2 absolute_size = GetAbsoluteSize();

  return Rectangle{absolute_position.x - absolute_size.x * pivot.x,
                   absolute_position.y - absolute_size.y * pivot.y,
                   absolute_size.x, absolute_size.y};
}