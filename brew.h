#include "constants.h"
#include "variables.h"

void inline brew_toggle_relay() {
  if (heater_relayEnabled) {
    digitalWrite(PIN_RELAY_HEATER, HIGH);
  }
  else {
    digitalWrite(PIN_RELAY_HEATER, LOW);
  }
}

inline void brew_loop(unsigned long now) {
  brew_toggle_relay();

  if (heater_relayMode == RELAY_MODE_AUTO) {
    heater_relayEnabled = false;
    if (brew_status == BREW_STATUS_WORKING) {
      heater_relayEnabled = true;
    }
  }
}