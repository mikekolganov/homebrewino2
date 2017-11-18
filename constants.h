#ifndef CONSTANTS_H
#define CONSTANTS_H

#define PIN_BUZZER            13
#define PIN_SENSOR_FAN        12
#define PIN_SENSOR_BREWING    11
#define PIN_DISPLAY_BACKLIGHT 10
#define PIN_DISPLAY_1         8
#define PIN_DISPLAY_2         9
#define PIN_DISPLAY_3         4
#define PIN_DISPLAY_4         5
#define PIN_DISPLAY_5         6
#define PIN_DISPLAY_6         7
#define PIN_RELAY_FAN         3 
#define PIN_RELAY_HEATER      2
#define PIN_BUTTON_ESCAPE     A1
#define PIN_BUTTON_KEYS       A0
#define PIN_RELAY_PUMP        A5

#define LOOP_THRESHOLD_KEYBOARD 50
#define LOOP_THRESHOLD_DISPLAY  500
#define LOOP_THRESHOLD_SENSORS  500
#define LOOP_THRESHOLD_STORE    500
#define LOOP_THRESHOLD_BREW     1000
#define LOOP_THRESHOLD_RELAYS   500

#define BUTTON_NONE   0
#define BUTTON_UP     1
#define BUTTON_DOWN   2
#define BUTTON_LEFT   3
#define BUTTON_RIGHT  4
#define BUTTON_ENTER  5
#define BUTTON_ESCAPE 6

#define BUTTON_SHORTPRESS 0
#define BUTTON_LONGPRESS  1
#define BUTTON_HOLDPRESS  2

#define KEYBOARD_VOLTAGE_DEVIATION   10
#define KEYBOARD_DURATION_SHORTPRESS 20
#define KEYBOARD_DURATION_LONGPRESS  500
#define KEYBOARD_DURATION_HOLDPRESS  1500
#define KEYBOARD_REPEAT_HOLDPRESS    300

#define SCREEN_DASHBOARD      1
#define SCREEN_MAIN           2
#define SCREEN_SETTINGS       3
#define SCREEN_PROGRAM        4
#define SCREEN_PROGRAM_EDIT   5
#define SCREEN_BREW_CONTROL   6
#define SCREEN_HEATER_CONTROL 7
#define SCREEN_PUMP_CONTROL   8
#define SCREEN_CREDITS        9

#define SCREEN_BLINK_DELAY 200

#define SCREEN_ITEM_MAIN_PROGRAM 0
#define SCREEN_ITEM_MAIN_BREWING 1
#define SCREEN_ITEM_MAIN_HEATER 2
#define SCREEN_ITEM_MAIN_PUMP 3
#define SCREEN_ITEM_MAIN_SETTINGS 4
#define SCREEN_ITEM_MAIN_CREDITS 5
#define SCREEN_ITEM_SETTINGS_POWER 0
#define SCREEN_ITEM_SETTINGS_TANK 1
#define SCREEN_ITEM_SETTINGS_BACKLIGHT 2
#define SCREEN_ITEM_SETTINGS_FAN_AT 3
#define SCREEN_ITEM_SETTINGS_PUMP_AT 4
#define SCREEN_ITEM_BREW_CONTROL_PLAY_PAUSE 0
#define SCREEN_ITEM_BREW_CONTROL_STOP 1
#define SCREEN_ITEM_HEATER_CONTROL_MODE 0
#define SCREEN_ITEM_HEATER_CONTROL_TOGGLE 1
#define SCREEN_ITEM_PUMP_CONTROL_MODE 0
#define SCREEN_ITEM_PUMP_CONTROL_TOGGLE 1

#define BREW_STATUS_IDLE 0
#define BREW_STATUS_WORKING 1
#define BREW_STATUS_PAUSED 2
#define BREW_STATUS_COMPLETED 3

#define PROGRAM_ITEMS_MAX_COUNT 30

#define PROGRAM_ITEM_MASH 0
#define PROGRAM_ITEM_BOILING 1
#define PROGRAM_ITEM_REMINDER_TIME 2
#define PROGRAM_ITEM_REMINDER_TEMP 3

#define RELAY_MODE_AUTO 0
#define RELAY_MODE_MANUAL 1
#define RELAY_DISABLED 0
#define RELAY_ENABLED 1

#define DEFAULT_HEATER_POWER 32
#define DEFAULT_TANK_VOLUME  28
#define DEFAULT_BACKLIGHT_LEVEL 60
#define DEFAULT_FAN_TEMP 50
#define DEFAULT_PUMP_TEMP_DELTA 15

#define HEATER_POWER_MULTIPLIER 100
#define PUMP_TEMP_DELTA_MULTIPLIER 0.1f

#endif