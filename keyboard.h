#include "constants.h"
#include "variables.h"

void inline keyboard_releaseKeys() {
  keyboard_upPressed     = false;
  keyboard_downPressed   = false;
  keyboard_leftPressed   = false;
  keyboard_rightPressed  = false;
  keyboard_enterPressed  = false;
  keyboard_escapePressed = false;
  
  keyboard_shortPress = false;
  keyboard_longPress  = false;
}

void inline keyboard_event(byte key, byte eventType) {
  keyboard_releaseKeys();
  switch (key) {
    case BUTTON_UP:     keyboard_upPressed     = true; break;
    case BUTTON_DOWN:   keyboard_downPressed   = true; break;
    case BUTTON_LEFT:   keyboard_leftPressed   = true; break;
    case BUTTON_RIGHT:  keyboard_rightPressed  = true; break;
    case BUTTON_ENTER:  keyboard_enterPressed  = true; break;
    case BUTTON_ESCAPE: keyboard_escapePressed = true; break;
  }

  switch (eventType) {
    case BUTTON_SHORTPRESS: keyboard_shortPress = true; break;
    case BUTTON_LONGPRESS:  keyboard_longPress  = true; break;
  }
}

byte inline keyboard_getPressedKey() {
  word escape = analogRead(PIN_BUTTON_ESCAPE);
  if (0 <= escape && escape < KEYBOARD_VOLTAGE_ESC + KEYBOARD_VOLTAGE_DEVIATION) {
    return BUTTON_ESCAPE;
  }

  word keypad = analogRead(PIN_BUTTON_KEYS);
  if (KEYBOARD_VOLTAGE_UP - KEYBOARD_VOLTAGE_DEVIATION < keypad && keypad < KEYBOARD_VOLTAGE_UP + KEYBOARD_VOLTAGE_DEVIATION) { 
    return BUTTON_UP; 
  }
  if (KEYBOARD_VOLTAGE_DOWN - KEYBOARD_VOLTAGE_DEVIATION < keypad && keypad < KEYBOARD_VOLTAGE_DOWN + KEYBOARD_VOLTAGE_DEVIATION) { 
    return BUTTON_DOWN; 
  }
  if (KEYBOARD_VOLTAGE_LEFT - KEYBOARD_VOLTAGE_DEVIATION < keypad && keypad < KEYBOARD_VOLTAGE_LEFT + KEYBOARD_VOLTAGE_DEVIATION) { 
    return BUTTON_LEFT; 
  }
  if (0 < keypad && keypad < KEYBOARD_VOLTAGE_RIGHT + KEYBOARD_VOLTAGE_DEVIATION) { 
    return BUTTON_RIGHT; 
  }
  if (KEYBOARD_VOLTAGE_ENTER - KEYBOARD_VOLTAGE_DEVIATION < keypad && keypad < KEYBOARD_VOLTAGE_ENTER + KEYBOARD_VOLTAGE_DEVIATION) { 
    return BUTTON_ENTER; 
  }
  return BUTTON_NONE;
}

void inline keyboard_loop(unsigned long now) {
  if (now - lastRun_loopKeyboard < LOOP_THRESHOLD_KEYBOARD) return;
  lastRun_loopKeyboard = now;

  byte key = keyboard_getPressedKey();

  if (key != keyboard_lastKey) {
    if (key == BUTTON_NONE && keyboard_lastKey != BUTTON_NONE) {
      if (! keyboard_holdPress && now - keyboard_lastKeyAt > KEYBOARD_DURATION_LONGPRESS) {
        keyboard_event(keyboard_lastKey, BUTTON_LONGPRESS);
      }
      else if (! keyboard_holdPress && now - keyboard_lastKeyAt > KEYBOARD_DURATION_SHORTPRESS) {
        keyboard_event(keyboard_lastKey, BUTTON_SHORTPRESS);
      }
    }
    keyboard_lastKey = key;
    keyboard_lastKeyAt = now; 
    keyboard_holdPress = false;   
  }
  else if (key != BUTTON_NONE) {
    if (now - keyboard_lastKeyAt > KEYBOARD_DURATION_HOLDPRESS) {
      if (now - keyboard_holdPressAt > KEYBOARD_REPEAT_HOLDPRESS) {
        keyboard_holdPress = true;
        keyboard_holdPressAt = now;
        keyboard_event(key, BUTTON_HOLDPRESS);
      }
    }
  }
}