#include "variables.h"
#include "constants.h"

#ifndef RELAYS_H
#define RELAYS_H

void inline relays_turn_on_heater() {
  relay_heaterEnabled = true;
  relay_heaterEnabledTill = 0;
  digitalWrite(PIN_RELAY_HEATER, HIGH);
}

void inline relays_turn_off_heater() {
  relay_heaterEnabled = false;
  digitalWrite(PIN_RELAY_HEATER, LOW);
}

void inline relays_turn_on_heater_for(int time) {
  relay_heaterEnabled = true;
  relay_heaterEnabledTill = millis() + time;
  digitalWrite(PIN_RELAY_HEATER, HIGH);
}

void inline relays_turn_on_pump() {
  relay_pumpEnabled = true;
  relay_pumpEnabledTill = 0;
  digitalWrite(PIN_RELAY_PUMP, LOW);
}

void inline relays_turn_off_pump() {
  relay_pumpEnabled = false;
  digitalWrite(PIN_RELAY_PUMP, HIGH);
}

void inline relays_turn_on_pump_for(int time) {
  relay_pumpEnabled = true;
  relay_pumpEnabledTill = millis() + time;
  digitalWrite(PIN_RELAY_PUMP, LOW);
}

void inline relays_turn_on_fan() {
  relay_fanEnabled = true;
  relay_fanEnabledTill = 0;
  digitalWrite(PIN_RELAY_FAN, LOW);
}

void inline relays_turn_off_fan() {
  relay_fanEnabled = false;
  digitalWrite(PIN_RELAY_FAN, HIGH);
}

void inline relays_turn_on_fan_for(int time) {
  relay_fanEnabled = true;
  relay_fanEnabledTill = millis() + time;
  digitalWrite(PIN_RELAY_FAN, LOW);
}

void inline relays_loop(unsigned long now) {
  if (now - lastRun_loopRelays < LOOP_THRESHOLD_RELAYS) return;
  lastRun_loopRelays = now;
  
  if (relay_heaterEnabledTill > 0 && relay_heaterEnabledTill < now) {
    relay_heaterEnabledTill = 0;
    relays_turn_off_heater();
  }
  if (relay_pumpEnabledTill > 0 && relay_pumpEnabledTill < now) {
    relay_pumpEnabledTill = 0;
    relays_turn_off_pump();
  }
  if (relay_fanEnabledTill > 0 && relay_fanEnabledTill < now) {
    relay_fanEnabledTill = 0;
    relays_turn_off_fan();
  }
}

#endif