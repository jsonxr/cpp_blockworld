// NOLINTNEXTLINE(modernize-deprecated-headers)
#include "math.h"

namespace math {

auto mapRange(float val, float inMin, float inMax, float outMin, float outMax)
    -> float {
  return ((val - inMin) * (outMax - outMin) / (inMax - inMin)) + outMin;
}

}  // namespace math
