#pragma once

#include "components/text.hpp"
#include <functional>
#include <raylib.h>
#include <string>
#include <vector>

class TextButtonComponent : public TextComponent {
public:
  using Callback = std::function<void()>;

private:
  bool was_hovered = false;
  std::vector<Callback> on_click;
  std::vector<Callback> on_hover_enter;
  std::vector<Callback> on_hover_exit;

public:
  TextButtonComponent(const std::string &text, const UDim2 &position,
                      const Font &font, const UDim &font_size, float spacing,
                      const Color &color)
      : TextComponent(text, position, font, font_size, spacing, color) {}

  void Update();

  void OnClick(Callback cb) { on_click.push_back(std::move(cb)); }
  void OnHoverEnter(Callback cb) { on_hover_enter.push_back(std::move(cb)); }
  void OnHoverExit(Callback cb) { on_hover_exit.push_back(std::move(cb)); }
};
