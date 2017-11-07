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

void setup() {
  Serial.begin(9600);
  lcd.createChar(1, DELTA_SYMBOL_1);
  lcd.begin(16, 2);
  pinMode(PIN_RELAY_HEATER, OUTPUT);
  pinMode(PIN_RELAY_FAN, OUTPUT);
  digitalWrite(PIN_RELAY_FAN, HIGH);
  EEPROM.setMemPool(0, EEPROMSizeUno);
  EEPROM.setMaxAllowedWrites(2000);
  store_setup();
  analogWrite(PIN_DISPLAY_BACKLIGHT, round(255 / 100 * setting_backlightLevel));
}

void loop() {
  unsigned long now = millis();
  brew_loop(now);
  buzzer_loop(now);
  fan_loop(now);
  keyboard_loop(now);
  pump_loop(now);
  display_loop(now);
  sensors_loop(now);
  store_loop(now);
}