#include "constants.h"
#include "variables.h"
#include "relays.h"

inline void fan_loop(unsigned long now) {
  if (now - lastRun_loopFan < LOOP_THRESHOLD_FAN) return;
  lastRun_loopFan = now;

  if (now < 5000) return;

  if (sensor_fan > setting_fanTemp) {
    relays_turn_on_fan_for(5 * 1000);
  }
}