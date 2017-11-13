#ifndef VARIABLES_H
#define VARIABLES_H

#include "constants.h"
#include "symbols.h"

unsigned long lastRun_loopBrew     = 0;
unsigned long lastRun_loopBuzzer   = 0;
unsigned long lastRun_loopFan      = 0;
unsigned long lastRun_loopKeyboard = 0;
unsigned long lastRun_loopPump     = 0;
unsigned long lastRun_loopDisplay  = 0;
unsigned long lastRun_loopSensors  = 0;
unsigned long lastRun_loopStore    = 0;

bool event_settingsChanged = false;
bool event_programChanged = false;

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
char display_firstLine[17];
char display_secondLine[17];
byte display_screenCurrent = SCREEN_DASHBOARD;
byte display_screenPrevious;
byte display_iterableCount = 0;
char display_activeIterable[6] = { 0, 0, 0, 0, 0, 0 };
char display_activeIterablePrevious[6] = { 0, 0, 0, 0, 0, 0 };

byte display_programEditItem = 0;
int  display_programEdit_A = 0;
int  display_programEdit_B = 0;
int  display_programEdit_C = 0;

byte setting_heaterPower;
byte setting_tankVolume;
byte setting_backlightLevel;
byte setting_fanTemp;
byte setting_pumpTempDelta;

byte brew_status;
unsigned long brew_timeProcessed;
byte brew_programLength = 0;
int  brew_program[PROGRAM_ITEMS_MAX_COUNT][3];

byte heater_relayMode    = RELAY_MODE_AUTO;
bool heater_relayEnabled = false;
byte pump_relayMode      = RELAY_MODE_AUTO;
bool pump_relayEnabled   = false;
byte fan_relayMode       = RELAY_MODE_AUTO;
bool fan_relayEnabled    = false;

int buzzer_sequence[20];
byte buzzer_sequenceIndex = 0;
byte buzzer_sequenceLength = 0;
unsigned long buzzer_nextToneTime = 0;

const char POINTER_SYMBOL = char(165);
const char DEGREE_SYMBOL  = char(223);
char pointerSymbol[2] = { POINTER_SYMBOL, '\0' };
char degreeSymbol[2] = { DEGREE_SYMBOL, '\0' };
char deltaSymbol[2] = { 1, '\0' };
char lockSymbol[2] = { 2, '\0'};

#endif