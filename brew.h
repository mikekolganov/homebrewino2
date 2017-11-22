#include "constants.h"
#include "variables.h"
#include "relays.h"

#ifndef BREW_H
#define BREW_H

inline void brew_start() {
  heater_relayMode = RELAY_MODE_AUTO;
  pump_relayMode   = RELAY_MODE_AUTO;
  relays_turn_off_heater();
  relays_turn_off_pump();
  brew_status = BREW_STATUS_WORKING;
  event_brewStateChanged = true;
}

inline void brew_pause() {
  relays_turn_off_heater();
  relays_turn_off_pump();
  brew_status = BREW_STATUS_PAUSED;
  event_brewStateChanged = true;
}

inline void brew_stop() {
  relays_turn_off_heater();
  relays_turn_off_pump();
  brew_status = BREW_STATUS_IDLE;
  brew_timeProcessed = 0;
  event_brewStateChanged = true;
}

inline byte brew_get_current_step() {}

inline byte brew_get_previous_step() {}

inline byte brew_get_current_alert() {}

inline byte brew_get_upcoming_alert() {}

word estimate_heating_time(byte fromTemp, byte toTemp) {
  if (fromTemp > toTemp) { return 0; }
  float hours = (setting_tankVolume * 1.163 * (toTemp - fromTemp)) / setting_heaterPower;
  return hours * 60 * 60;
}

inline void brew_loop(unsigned long now) {
  if (now - lastRun_loopBrew < LOOP_THRESHOLD_BREW) return;
  lastRun_loopBrew = now;

  display_messagesCount = 0;

  if (brew_status == BREW_STATUS_IDLE) {
    display_messagesCount = 1;
    strcpy(display_messages[0], "IDLE, GO BREW!");
  }
  else if (brew_status == BREW_STATUS_PAUSED) {
    display_messagesCount = 1;
    strcpy(display_messages[0], "BREW PAUSED...");
  }
  else if (brew_status == BREW_STATUS_WORKING) {
    display_messagesCount = 1;
    strcpy(display_messages[0], "BREWING!");

    // TODO: Save brew state only if passed time changed
    event_brewStateChanged = true;
    relays_turn_on_heater();
  }
}

#endif