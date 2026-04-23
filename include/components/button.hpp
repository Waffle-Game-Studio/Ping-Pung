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

  std::vector<EventHandler> click_handlers;
  std::vector<EventHandler> hover_enter_handlers;
  std::vector<EventHandler> hover_exit_handlers;
  std::vector<EventHandler> pressed_handlers;
  std::vector<EventHandler> released_handlers;

  ButtonComponent(const std::string &text, const UDim2 &position,
                  const UDim2 &size, const Font &font, float spacing,
                  const Color &text_color, const Color &background_color)
      : TextComponent(text, position, font, UDim{size.scale_y, size.offset_y},
                      spacing, text_color, background_color) {
    this->size = size;
  }

  void Update() override;
  void Draw() const override;
};
