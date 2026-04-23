#include "components/text.hpp"
#include <raylib.h>

void TextComponent::Update() {
  this->size = {
      0, size.scale_y,
      MeasureTextEx(font, text.c_str(), GetAbsoluteSize().y, spacing).x,
      size.offset_y};

  this->absolute_position = GetAbsolutePosition();
  this->absolute_size = GetAbsoluteSize();
}

void TextComponent::Draw() const {
  Rectangle rect = GetAbsoluteBounds();

  if (background_color.a > 0) {
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, background_color);
  }

  DrawTextEx(font, text.c_str(), Vector2{rect.x, rect.y}, absolute_size.y,
             spacing, text_color);
}
