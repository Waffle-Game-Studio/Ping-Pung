#pragma once

#include "types/math.hpp"
#include <raylib.h>
#include <string>

class TextComponent {
private:
  std::string text;
  UDim2 position;
  Font font;
  UDim font_size;
  float spacing;
  Color color;

  Vector2 absolute_position;
  Vector2 absolute_size;
  float absolute_font_size;

public:
  TextComponent(const std::string &text, const UDim2 &position,
                const Font &font, const UDim &font_size, float spacing,
                const Color &color);

  void Update();

  void Draw();
};
