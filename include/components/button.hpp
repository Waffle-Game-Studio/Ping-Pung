#pragma once

#include "components/text.hpp"
#include "types/math.hpp"
#include <functional>
#include <raylib.h>
#include <string>
#include <vector>

class ButtonComponent : public TextComponent {
public:
  using EventHandler = std::function<void(ButtonComponent &)>;

  bool hovered = false;
  bool pressed = false;

  UDim font_size = {0.0f, 16.0f};
  UDim2 padding = {0.0f, 0.0f, 16.0f, 10.0f};

  std::vector<EventHandler> click_handlers;
  std::vector<EventHandler> hover_enter_handlers;
  std::vector<EventHandler> hover_exit_handlers;
  std::vector<EventHandler> pressed_handlers;
  std::vector<EventHandler> released_handlers;

  ButtonComponent(const std::string &text, const UDim2 &position,
                  const UDim &font_size, const UDim2 &padding, const Font &font,
                  float spacing, const Color &text_color,
                  const Color &background_color)
      : TextComponent(text, position, font, font_size, spacing, text_color,
                      background_color),
        font_size(font_size), padding(padding) {}

  void Update() override;
  void Draw() const override;
};
