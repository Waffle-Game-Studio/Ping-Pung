#include "components/button.hpp"

void TextButtonComponent::Update() {
  TextComponent::Update();

  Rectangle bounds = {
      absolute_position.x,
      absolute_position.y,
      absolute_size.x,
      absolute_size.y,
  };

  const bool is_hovered = CheckCollisionPointRec(GetMousePosition(), bounds);

  if (is_hovered && !was_hovered) {
    for (const Callback &cb : on_hover_enter) {
      cb();
    }
  } else if (!is_hovered && was_hovered) {
    for (const Callback &cb : on_hover_exit) {
      cb();
    }
  }

  if (is_hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    for (const Callback &cb : on_click) {
      cb();
    }
  }

  was_hovered = is_hovered;
}
