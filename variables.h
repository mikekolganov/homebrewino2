#ifndef VARIABLES_H
#define VARIABLES_H

#include "constants.h"

unsigned long lastRun_loopBrew     = 0;
unsigned long lastRun_loopBuzzer   = 0;
unsigned long lastRun_loopFan      = 0;
unsigned long lastRun_loopKeyboard = 0;
unsigned long lastRun_loopPump     = 0;
unsigned long lastRun_loopDisplay  = 0;
unsigned long lastRun_loopSensors  = 0;
unsigned long lastRun_loopStore    = 0;

float sensor_brewing1 = 0;
float sensor_brewing2 = 0;
float sensor_fan = 0;

unsigned long keyboard_lastKeyAt = 0;
unsigned long keyboard_holdPressAt = 0;
byte keyboard_lastKey = BUTTON_NONE;
bool keyboard_holdPress  = false;
bool keyboard_shortPress = false;
bool keyboard_longPress  = false;

bool keyboard_upPressed     = false;
bool keyboard_downPressed   = false;
bool keyboard_leftPressed   = false;
bool keyboard_rightPressed  = false;
bool keyboard_enterPressed  = false;
bool keyboard_escapePressed = false;

bool relay_heaterEnabled = false;
bool relay_fanEnabled    = false;
bool relay_pumpEnabled   = false;

unsigned long display_willChange = 0;
char display_firstLine[16];
char display_secondLine[16];
char display_firstLinePrevious[16];
char display_secondLinePrevious[16];
byte display_screenCurrent = SCREEN_DASHBOARD;
byte display_screenPrevious;
byte display_iterableCount = 0;
char display_activeIterable[6] = { 0, 0, 0, 0, 0, 0 };
char display_activeIterablePrevious[6] = { 0, 0, 0, 0, 0, 0 };

byte display_programEditItem = 0;
int display_programEdit_A = 0;
int display_programEdit_B = 0;
int display_programEdit_C = 0;

byte setting_heaterPower;
byte setting_tankVolume;
byte setting_backlightLevel;
byte setting_fanTemp;

byte brew_status;
word brew_timeProcessed;
byte brew_program[10][3];
byte brew_programLength = 0;

byte heater_relayMode    = RELAY_MODE_AUTO;
bool heater_relayEnabled = false;
byte pump_relayMode      = RELAY_MODE_AUTO;
bool pump_relayEnabled   = false;
byte fan_relayMode       = RELAY_MODE_AUTO;
bool fan_relayEnabled    = false;

const char POINTER_SYMBOL = char(165);
const char DEGREE_SYMBOL  = char(223);

#endif