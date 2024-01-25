#include <memory>

#include "Axis.h"
#include "../math/Vector2.h"

namespace P1::inputs {
  class Axis2 {
  private:
    Axis* horizontal;
    Axis* vertical;

  public:
    Axis2(Axis& horizontal, Axis& vertical);

    math::vector2 get_raw() const;
  };
}
