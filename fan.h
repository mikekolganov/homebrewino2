#include "constants.h"
#include "variables.h"

inline void fan_loop(unsigned long now) {
  if (sensor_fan > setting_fanTemp) {
    digitalWrite(PIN_RELAY_FAN, LOW);
  }
  else {
    digitalWrite(PIN_RELAY_FAN, HIGH);
  }
}