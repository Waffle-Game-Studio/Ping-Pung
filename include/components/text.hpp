#pragma once

#include "components/component.hpp"
#include "types/math.hpp"
#include <raylib.h>
#include <string>

class TextComponent : public Component {
public:
  std::string text = "";
  Font font = GetFontDefault();
  float spacing = 0.0f;
  Color text_color = WHITE;
  Color background_color = {0, 0, 0, 0};
  UDim2 background_padding = {0.0f, 0.0f, 0.0f, 0.0f};
  UDim border_size = {0.0f, 0.0f};
  Color border_color = {0, 0, 0, 0};

  TextComponent(const std::string &text, const UDim2 &position,
                const Font &font, const UDim &font_size, float spacing,
                const Color &text_color, const Color &background_color)
      : text(text), font(font), spacing(spacing), text_color(text_color),
        background_color(background_color) {
    this->position = position;
    this->size = {
        0, font_size.scale,
        MeasureTextEx(font, text.c_str(), font_size.offset, spacing).x,
        font_size.offset};
  }

  void Update() override;
  void Draw() const override;
};
