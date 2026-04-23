#include "components/text.hpp"
#include <raylib.h>

void TextComponent::Update() {
  if (!enabled)
    return;

  this->size = {
      0, size.scale_y,
      MeasureTextEx(font, text.c_str(), GetAbsoluteSize().y, spacing).x,
      size.offset_y};

  this->absolute_position = GetAbsolutePosition();
  this->absolute_size = GetAbsoluteSize();
}

void TextComponent::Draw() const {
  if (!visible)
    return;

  Rectangle rect = GetAbsoluteBounds();

  float screen_w = static_cast<float>(GetScreenWidth());
  float screen_h = static_cast<float>(GetScreenHeight());

  float pad_x =
      screen_w * background_padding.scale_x + background_padding.offset_x;
  float pad_y =
      screen_h * background_padding.scale_y + background_padding.offset_y;

  Rectangle bg_rect = {rect.x - pad_x * 0.5f, rect.y - pad_y * 0.5f,
                       rect.width + pad_x, rect.height + pad_y};

  if (background_color.a > 0) {
    DrawRectangleRec(bg_rect, background_color);
  }

  float border = screen_h * border_size.scale + border_size.offset;
  if (border > 0.0f && border_color.a > 0) {
    DrawRectangleLinesEx(bg_rect, border, border_color);
  }

  DrawTextEx(font, text.c_str(), Vector2{rect.x, rect.y}, absolute_size.y,
             spacing, text_color);
}
