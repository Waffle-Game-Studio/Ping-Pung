#include "components/button.hpp"

void TextButtonComponent::Update() {
  TextComponent::Update();

  Rectangle bounds = {
      absolute_position.x,
      absolute_position.y,
      absolute_size.x,
      absolute_size.y,
  };

  if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
    is_hovered = true;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      is_clicked = true;
    } else {
      is_clicked = false;
    }
  } else {
    is_hovered = false;
    is_clicked = false;
  }
}
