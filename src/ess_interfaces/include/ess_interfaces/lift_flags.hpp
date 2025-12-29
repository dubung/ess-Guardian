#ifdef __ESS_INTERFACE_LEFT_FLAGS_H__
#define __ESS_INTERFACE_LEFT_FLAGS_H__
#include <cstdint>

namespace ess_interfaces::lift_flags {
  constexpr uint8_t STOP = 0;
  constexpr uint8_t TOFLOOR1 = 1 << 0;
  constexpr uint8_t TOFLOOR2 = 1 << 1;
  constexpr uint8_t TOFLOOR3 = 1 << 2;
  constexpr uint8_t GOUP   = 1 << 3;
  constexpr uint8_t GODOWN = 1 << 4;
  constexpr uint8_t UP	= 1 << 5;
}
#endif
