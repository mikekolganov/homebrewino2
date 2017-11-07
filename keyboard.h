#include "constants.h"
#include "variables.h"

inline void keyboard_releaseKeys() {
  keyboard_upPressed     = false;
  keyboard_downPressed   = false;
  keyboard_leftPressed   = false;
  keyboard_rightPressed  = false;
  keyboard_enterPressed  = false;
  keyboard_escapePressed = false;
  
  keyboard_shortPress = false;
  keyboard_longPress  = false;
  keyboard_holdPress = false;
}

inline void keyboard_event(byte key, byte eventType) {
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
    case BUTTON_HOLDPRESS:  keyboard_holdPress  = true; break;
  }
}

word keypad;
word escape;

byte inline keyboard_getPressedKey() {
  escape = analogRead(PIN_BUTTON_ESCAPE);
  keypad = analogRead(PIN_BUTTON_KEYS);

  if (0 <= escape && escape < 30 && keypad > 900) {
    return BUTTON_ESCAPE;
  }
  else if (130 < keypad && keypad < 200) { 
    return BUTTON_UP; 
  }
  else if (300 < keypad && keypad < 390) { 
    return BUTTON_DOWN; 
  }
  else if (470 < keypad && keypad < 490) { 
    return BUTTON_LEFT; 
  }
  else if (0 <= keypad && keypad < 110) { 
    return BUTTON_RIGHT; 
  }
  else if (710 < keypad && keypad < 730) { 
    return BUTTON_ENTER; 
  }
  else {
    return BUTTON_NONE;
  }
}

inline void keyboard_loop(unsigned long now) {
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