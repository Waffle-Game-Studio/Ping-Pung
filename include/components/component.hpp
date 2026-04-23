#pragma once

#include "types/math.hpp"
#include <raylib.h>

class Component {
public:
  bool enabled = true;
  bool visible = true;

  Vector2 pivot = {0.0f, 0.0f};
  UDim2 position;
  UDim2 size;

  Component() = default;
  virtual ~Component() = default;

  virtual void Draw() const = 0;
  virtual void Update() = 0;

  Vector2 GetAbsolutePosition() const;
  Vector2 GetAbsoluteSize() const;
};
