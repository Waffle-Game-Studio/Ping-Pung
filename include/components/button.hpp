#pragma once

#include "components/text.hpp"
#include <raylib.h>
#include <string>

class TextButtonComponent : public TextComponent {
private:
  bool is_hovered = false;
  bool is_clicked = false;

public:
  TextButtonComponent(const std::string &text, const UDim2 &position,
                      const Font &font, const UDim &font_size, float spacing,
                      const Color &color)
      : TextComponent(text, position, font, font_size, spacing, color) {}

  void Update();

  bool IsHovered() const { return is_hovered; }
  bool IsClicked() const { return is_clicked; }
};
