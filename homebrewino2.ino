#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROMex.h>

#include "constants.h"

OneWire brew_wire(PIN_SENSOR_BREWING);
OneWire fan_wire(PIN_SENSOR_FAN);
DallasTemperature brewing_sensors(&brew_wire);
DallasTemperature fan_sensor(&fan_wire);
LiquidCrystal lcd(PIN_DISPLAY_1, PIN_DISPLAY_2, PIN_DISPLAY_3, PIN_DISPLAY_4, PIN_DISPLAY_5, PIN_DISPLAY_6);

#include "brew.h"
#include "buzzer.h"
#include "fan.h"
#include "keyboard.h"
#include "pump.h"
#include "store.h"
#include "display.h"
#include "sensors.h"
#include "variables.h"
#include "relays.h"

void setup() {
  // Wipe out all data from EEPROM, uncomment in case of fire
  // for (int i = 0 ; i < 1024 ; i++) EEPROM.write(i, 0);
  Serial.begin(9600);
  lcd.createChar(1, DELTA_SYMBOL_1);
  lcd.createChar(2, LOCK_SYMBOL_2);
  lcd.createChar(3, PLAY_SYMBOL_3);
  lcd.createChar(4, PAUSE_SYMBOL_4);
  lcd.createChar(5, STOP_SYMBOL_5);
  lcd.noAutoscroll();
  lcd.begin(16, 2);
  pinMode(PIN_RELAY_HEATER, OUTPUT);
  pinMode(PIN_RELAY_FAN, OUTPUT);
  pinMode(PIN_RELAY_PUMP, OUTPUT);
  digitalWrite(PIN_RELAY_FAN, HIGH);
  digitalWrite(PIN_RELAY_PUMP, HIGH);
  EEPROM.setMemPool(0, EEPROMSizeUno);
  EEPROM.setMaxAllowedWrites(2000);
  store_setup();
  analogWrite(PIN_DISPLAY_BACKLIGHT, round(255 / 100 * setting_backlightLevel));
}

void loop() {
  unsigned long now = millis();
  brew_loop(now);
  buzzer_loop(now);
  display_loop(now);
  keyboard_loop(now);
  pump_loop(now);
  relays_loop(now);
  sensors_loop(now);
  fan_loop(now);
  store_loop(now);
}