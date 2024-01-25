#include "Axis2.h"

namespace P1::inputs {
  Axis2::Axis2(Axis& horizontal, Axis& vertical) {
    this->horizontal = &horizontal;
    this->vertical = &vertical;
  }

  math::vector2 Axis2::get_raw() const {
    return math::vector2{ static_cast<double>(horizontal->get_raw()), static_cast<double>(vertical->get_raw()) }.normalized();
  }
}
