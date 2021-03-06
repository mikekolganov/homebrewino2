#include "constants.h"
#include "variables.h"
#include "relays.h"
#include "utils.h"

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
  brew_previousStep = 0;
  event_brewStateChanged = true;
}

inline byte brew_get_current_step_index(unsigned long timeProcessed) {
  unsigned long duration = 0;
  for (byte i = 0; i < brew_programLength; i++) {
    switch (brew_program[i][PROGRAM_A]) {
      case PROGRAM_ITEM_MASH:
      case PROGRAM_ITEM_BOILING:
        duration += brew_program[i][PROGRAM_B] * 60;
        if (timeProcessed <= duration) {
          return i;
        }
    }
  }
}

inline byte brew_get_step_temperature(byte index) {
  switch (brew_program[index][PROGRAM_A]) {
    case PROGRAM_ITEM_MASH:
      return brew_program[index][PROGRAM_C];
    case PROGRAM_ITEM_BOILING:
      return 100;
  }
}

inline bool brew_is_temp_reached(byte index, float temp) {
  switch (brew_program[index][PROGRAM_A]) {
    case PROGRAM_ITEM_MASH:
      return brew_program[index][PROGRAM_C] <= temp;
    case PROGRAM_ITEM_BOILING:
      return setting_waterBoilTemp <= temp;
  }
}

word brew_estimate_heating_time(byte fromTemp, byte toTemp) {
  if (fromTemp > toTemp) { return 0; }
  float hours = (setting_tankVolume * 1.163 * (toTemp - fromTemp)) / setting_heaterPower;
  return hours * 60 * 60;
}

word brew_get_brewing_time_left(byte index, unsigned long timeProcessed) {
  unsigned long duration = 0;
  for (byte i = 0; i < brew_programLength; i++) {
    switch (brew_program[i][PROGRAM_A]) {
      case PROGRAM_ITEM_MASH:
      case PROGRAM_ITEM_BOILING:
        if (i == index) {
          return timeProcessed - duration;
        }
        else {
          duration += brew_program[i][PROGRAM_B] * 60;
        }
    }
  }
}

unsigned long brew_get_total_program_time() {
  unsigned long duration = 0;
  for (byte i = 0; i < brew_programLength; i++) {
    switch (brew_program[i][PROGRAM_A]) {
      case PROGRAM_ITEM_MASH:
      case PROGRAM_ITEM_BOILING:
        duration += brew_program[i][PROGRAM_B] * 60;
    }
  }
  return duration;
}

unsigned long brew_get_time_left_in_step(byte index, unsigned long timeProcessed) {
  unsigned long duration = 0;
  for (byte i = 0; i < brew_programLength; i++) {
    switch (brew_program[i][PROGRAM_A]) {
      case PROGRAM_ITEM_MASH:
      case PROGRAM_ITEM_BOILING:
        duration += brew_program[i][PROGRAM_B] * 60;
        if (i == index) {
          return duration - timeProcessed;
        }
    }
  }
}

unsigned long brew_get_total_heat_time() {
  return brew_estimate_heating_time(sensor_brewingAverage, 100);
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
    display_messagesCount = 4;

    byte currentStepIndex = brew_get_current_step_index(brew_timeProcessed);
    byte stepTemperature = brew_get_step_temperature(currentStepIndex);
    unsigned long totalProgramTime = brew_get_total_program_time();
    unsigned long totalHeatTime = brew_get_total_heat_time();

    if (brew_previousStep != currentStepIndex) {
      if (brew_program[currentStepIndex][PROGRAM_A] == PROGRAM_ITEM_BOILING) {
        brew_pause();
      }
      brew_previousStep = currentStepIndex;
    }

    if (brew_timeProcessed >= totalProgramTime) {
      brew_stop();
    }

    if (brew_is_temp_reached(currentStepIndex, sensor_brewingAverage)) {
      word brewingTimeLeft = brew_get_time_left_in_step(currentStepIndex, brew_timeProcessed);
      brew_timeProcessed++;
      event_brewStateChanged = true;
      relays_turn_off_heater();
      strcpy(display_messages[0], "B. ");
      itoa(stepTemperature, display_messages[0] + strlen(display_messages[0]), 10);
      strcpy(display_messages[0] + strlen(display_messages[0]), degreeSymbol);
      strcpy(display_messages[0] + strlen(display_messages[0]), " ");
      display_format_seconds(display_messages[0], brewingTimeLeft);
    }
    else {
      word estimatedHeatingTime = brew_estimate_heating_time(sensor_brewingAverage, stepTemperature);
      relays_turn_on_heater_for(1000);
      strcpy(display_messages[0], "H. ");
      itoa(stepTemperature, display_messages[0] + strlen(display_messages[0]), 10);
      strcpy(display_messages[0] + strlen(display_messages[0]), degreeSymbol);
      strcpy(display_messages[0] + strlen(display_messages[0]), " ");
      display_format_seconds(display_messages[0], estimatedHeatingTime);
    }

    strcpy(display_messages[1], "MASH ");
    display_format_seconds(display_messages[1], totalProgramTime - brew_timeProcessed);

    strcpy(display_messages[2], "HEAT ");
    display_format_seconds(display_messages[2], totalHeatTime);

    strcpy(display_messages[3], "TOTAL ");
    display_format_seconds(display_messages[3], totalProgramTime - brew_timeProcessed + totalHeatTime);
  }
}

#endif