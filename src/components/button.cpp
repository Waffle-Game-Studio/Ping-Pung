#include "components/button.hpp"
#include <raylib.h>

static void
FireHandlers(const std::vector<ButtonComponent::EventHandler> &handlers,
             ButtonComponent &self) {
  for (const ButtonComponent::EventHandler &handler : handlers) {
    if (handler)
      handler(self);
  }
}

void ButtonComponent::Update() {
  this->absolute_position = GetAbsolutePosition();
  this->absolute_size = GetAbsoluteSize();

  if (!enabled) {
    hovered = false;
    pressed = false;
    return;
  }

  Rectangle bounds = GetAbsoluteBounds();
  Vector2 mouse = GetMousePosition();
  bool now_hovered = CheckCollisionPointRec(mouse, bounds);

  if (now_hovered && !hovered) {
    FireHandlers(hover_enter_handlers, *this);
  } else if (!now_hovered && hovered) {
    FireHandlers(hover_exit_handlers, *this);
  }
  hovered = now_hovered;

  if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    pressed = true;
    FireHandlers(pressed_handlers, *this);
  }

  if (pressed && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    pressed = false;
    FireHandlers(released_handlers, *this);
    if (hovered) {
      FireHandlers(click_handlers, *this);
    }
  }

  if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    pressed = false;
  }
}

void ButtonComponent::Draw() const {
  if (!visible)
    return;

  Rectangle rect = GetAbsoluteBounds();

  if (background_color.a > 0) {
    DrawRectangleRec(rect, background_color);
  }

  float fsize = absolute_size.y;
  Vector2 text_size = MeasureTextEx(font, text.c_str(), fsize, spacing);
  Vector2 text_pos = {rect.x + (rect.width - text_size.x) * 0.5f,
                      rect.y + (rect.height - text_size.y) * 0.5f};

  DrawTextEx(font, text.c_str(), text_pos, fsize, spacing, text_color);
}
