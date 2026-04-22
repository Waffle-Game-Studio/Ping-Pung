#include "components/text.hpp"
#include <raylib.h>

TextComponent::TextComponent(const std::string &text, const UDim2 &position,
                             const Font &font, const UDim &font_size,
                             float spacing, const Color &color)
    : text(text), position(position), font(font), font_size(font_size),
      spacing(spacing), color(color) {}

void TextComponent::Update() {
  int screen_width = GetScreenWidth();
  int screen_height = GetScreenHeight();

  absolute_font_size = screen_height * font_size.scale + font_size.offset;

  Vector2 text_size =
      MeasureTextEx(font, text.c_str(), absolute_font_size, spacing);

  absolute_position = Vector2{
      screen_width * position.relative_x + position.absolute_x -
          text_size.x / 2.0f,
      screen_height * position.relative_y + position.absolute_y -
          text_size.y / 2.0f,
  };
}

void TextComponent::Draw() {
  DrawTextEx(font, text.c_str(), absolute_position, absolute_font_size, spacing,
             color);
}
