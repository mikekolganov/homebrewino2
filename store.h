#include <EEPROMex.h>
#include "constants.h"
#include "variables.h"

word address_brew_status        = EEPROM.getAddress(sizeof(brew_status));
word address_brew_timeProcessed = EEPROM.getAddress(sizeof(brew_timeProcessed));
word address_brew_programLength = EEPROM.getAddress(sizeof(brew_programLength));
word address_brew_program       = EEPROM.getAddress(sizeof(brew_program));

word adress_setting_heaterPower    = EEPROM.getAddress(sizeof(byte));
word adress_setting_tankVolume     = EEPROM.getAddress(sizeof(byte));
word adress_setting_backlightLevel = EEPROM.getAddress(sizeof(byte));
word adress_setting_fanTemp        = EEPROM.getAddress(sizeof(byte));
word adress_setting_pumpTempDelta  = EEPROM.getAddress(sizeof(byte));

inline void store_save_brewing_state() {
  
}

inline void store_save_settings() {
  EEPROM.writeByte(adress_setting_heaterPower,    setting_heaterPower);
  EEPROM.writeByte(adress_setting_tankVolume,     setting_tankVolume);
  EEPROM.writeByte(adress_setting_backlightLevel, setting_backlightLevel);
  EEPROM.writeByte(adress_setting_fanTemp,        setting_fanTemp);
  EEPROM.writeByte(adress_setting_pumpTempDelta,  setting_pumpTempDelta);
}

inline void store_read_settings() {
  setting_heaterPower    = EEPROM.readByte(adress_setting_heaterPower);
  setting_tankVolume     = EEPROM.readByte(adress_setting_tankVolume);
  setting_backlightLevel = EEPROM.readByte(adress_setting_backlightLevel);
  setting_fanTemp        = EEPROM.readByte(adress_setting_fanTemp);
  setting_pumpTempDelta  = EEPROM.readByte(adress_setting_pumpTempDelta);

  if (setting_heaterPower    == 0) setting_heaterPower    = DEFAULT_HEATER_POWER;
  if (setting_tankVolume     == 0) setting_tankVolume     = DEFAULT_TANK_VOLUME;
  if (setting_backlightLevel == 0) setting_backlightLevel = DEFAULT_BACKLIGHT_LEVEL;
  if (setting_fanTemp        == 0) setting_fanTemp        = DEFAULT_FAN_TEMP;
  if (setting_pumpTempDelta  == 0) setting_pumpTempDelta  = DEFAULT_PUMP_TEMP_DELTA;
}

inline void store_setup() {
  store_read_settings();
}

inline void store_loop(unsigned long now) {
  if (now - lastRun_loopStore < LOOP_THRESHOLD_STORE) return;
  lastRun_loopStore = now;
}