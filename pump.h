#include "constants.h"
#include "variables.h"

void inline pump_toggle_relay() {
  if (pump_relayEnabled) {
    digitalWrite(PIN_RELAY_PUMP, LOW);
  }
  else {
    digitalWrite(PIN_RELAY_PUMP, HIGH);
  }
}

void inline pump_loop(unsigned long now) {
  pump_toggle_relay();

  if (pump_relayMode == RELAY_MODE_AUTO) {
    pump_relayEnabled = false;
    if (brew_status == BREW_STATUS_WORKING) {
      float delta = fabs(sensor_brewing1 - sensor_brewing2);
      if (delta > setting_pumpTempDelta * PUMP_TEMP_DELTA_MULTIPLIER) {
        pump_relayEnabled = true;
      }
    }
  }
}