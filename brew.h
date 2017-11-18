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
}