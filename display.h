#include "constants.h"
#include "variables.h"
#include "relays.h"
#include "brew.h"
#include "utils.h"

inline void display_wrapAlign(char *result, char left[15], char right[15], byte resultLength) {
  for(byte i = 0; i < resultLength; i++) {
    if (i < strlen(left)) {
      result[i] = left[i];
    }
    else if (resultLength - i > strlen(right)) {
      result[i] = ' ';
    }
    else {
      result[i] = right[strlen(right) - (resultLength - i)];
    }
  }
  result[resultLength] = '\0';
}

inline void display_render_iterable_menu(char items[][17], byte count, byte activeItem, byte prevActiveItem) {
  display_iterableCount = count;

  if (count == 1) {
    strcpy(display_firstLine, pointerSymbol);
    strcpy(display_firstLine + 1, items[0]);
    strcpy(display_secondLine, "                ");
  }
  else if (count == 2) {
    strcpy(display_firstLine, activeItem == 0 ? pointerSymbol : " ");
    strcpy(display_firstLine + 1, items[0]);
    strcpy(display_secondLine, activeItem == 1 ? pointerSymbol : " ");
    strcpy(display_secondLine + 1, items[1]);
  }
  else {
    for (byte i = 0; i < count; i++) {
      if (i != activeItem) continue;
      bool upDirection = true;
      if (activeItem == prevActiveItem) {
        upDirection = true;
      }
      else if (activeItem == 0 && prevActiveItem == (count - 1)) {
        upDirection = false;
      }
      else if (activeItem == (count - 1) && prevActiveItem == 0) {
        upDirection = true;
      }
      else {
        upDirection = prevActiveItem > activeItem;
      }

      if (upDirection) {
        strcpy(display_firstLine, pointerSymbol);
        strcpy(display_firstLine + 1, items[i]);
        strcpy(display_secondLine, " ");
        strcpy(display_secondLine + 1, (i == count - 1) ? items[0] : items[i + 1]);
      }
      else {
        strcpy(display_firstLine, " ");
        strcpy(display_firstLine + 1, (i == 0) ? items[count - 1] : items[i - 1]);
        strcpy(display_secondLine, pointerSymbol);
        strcpy(display_secondLine + 1, items[i]);
      }
    }
  }
}

inline void display_program_print_item_oneline(char *result, byte itemIndex) {
  itoa(itemIndex + 1, result + strlen(result), 10);
  strcpy(result + strlen(result), " ");

  switch (brew_program[itemIndex][0]) {
    case PROGRAM_ITEM_MASH:
      display_format_minutes(result, brew_program[itemIndex][1]);
      strcpy(result + strlen(result), " AT ");
      itoa(brew_program[itemIndex][2], result + strlen(result), 10);
      strcpy(result + strlen(result), degreeSymbol);
      break;
    case PROGRAM_ITEM_BOILING:
      strcpy(result + strlen(result), "BOIL ");
      display_format_minutes(result, brew_program[itemIndex][1]);
      break;
    case PROGRAM_ITEM_REMINDER_TIME:
      strcpy(result + strlen(result), "ALARM +");
      display_format_minutes(result, brew_program[itemIndex][1]);
      break;
    case PROGRAM_ITEM_REMINDER_TEMP:
      strcpy(result + strlen(result), "ALARM ");
      itoa(brew_program[itemIndex][1], result + strlen(result), 10);
      strcpy(result + strlen(result), degreeSymbol);
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void display_render_dashboard() {
  unsigned long now = millis();

  switch (brew_status) {
    case BREW_STATUS_IDLE:
      strcpy(display_firstLine, stopSymbol); break;
    case BREW_STATUS_WORKING:
      strcpy(display_firstLine, playSymbol); break;
    case BREW_STATUS_PAUSED:
      strcpy(display_firstLine, pauseSymbol); break;
  }

  if (sensor_brewing1 > 0 && sensor_brewing2 > 0) {
    char brew_temp1[8];
    char brew_temp2[8];
    dtostrf(sensor_brewing1, 4, 1, brew_temp1);
    dtostrf(sensor_brewing2, 4, 1, brew_temp2);
    strcpy(brew_temp1 + strlen(brew_temp1), degreeSymbol);
    strcpy(brew_temp2 + strlen(brew_temp2), degreeSymbol);
    sprintf(display_firstLine + strlen(display_firstLine), " %s %s", brew_temp1, brew_temp2);
  }
  else {
    strcpy(display_firstLine + strlen(display_firstLine), " NO SENSORS");
  }

  if (display_messagesCount > 0) {
    if (display_activeIterable[SCREEN_DASHBOARD] >= display_messagesCount) {
      display_activeIterable[SCREEN_DASHBOARD] = 0;
    }
    strcpy(display_secondLine, display_messages[display_activeIterable[SCREEN_DASHBOARD]]);
  }
  else {
    strcpy(display_secondLine, "");
  }

  display_willChange = now + 1000;
}

inline void display_render_main() {
  char items[6][17];
  strcpy(items[SCREEN_ITEM_MAIN_PROGRAM],  "PROGRAM");
  strcpy(items[SCREEN_ITEM_MAIN_BREWING],  "BREWING");
  strcpy(items[SCREEN_ITEM_MAIN_HEATER],   "HEATER CONTROL");
  strcpy(items[SCREEN_ITEM_MAIN_PUMP],     "PUMP CONTROL");
  strcpy(items[SCREEN_ITEM_MAIN_SETTINGS], "SETTINGS");
  strcpy(items[SCREEN_ITEM_MAIN_CREDITS],  "CREDITS");
  display_render_iterable_menu(items, 6, display_activeIterable[SCREEN_MAIN], display_activeIterablePrevious[SCREEN_MAIN]);
}

inline void display_render_settings() {
  char powerValue[6];
  itoa(setting_heaterPower, powerValue, 10);
  strcpy(powerValue + strlen(powerValue), "W");

  char tankValue[6];
  itoa(setting_tankVolume, tankValue, 10);
  strcpy(tankValue + strlen(tankValue), "L");

  char backlightValue[6];
  itoa(setting_backlightLevel, backlightValue, 10);
  strcpy(backlightValue + strlen(backlightValue), "%");

  char fanAtValue[6];
  itoa(setting_fanTemp, fanAtValue, 10);
  strcpy(fanAtValue + strlen(fanAtValue), degreeSymbol);

  char pumpAtValue[6];
  strcpy(pumpAtValue, deltaSymbol);
  dtostrf(setting_pumpTempDelta, 3, 1, pumpAtValue + strlen(pumpAtValue));
  strcpy(pumpAtValue + strlen(pumpAtValue), degreeSymbol);

  char waterBoilTempValue[7];
  dtostrf(setting_waterBoilTemp, 3, 1, waterBoilTempValue);
  strcpy(waterBoilTempValue + strlen(waterBoilTempValue), degreeSymbol);

  char items[5][17];
  display_wrapAlign(items[SCREEN_ITEM_SETTINGS_POWER], "POWER", powerValue, 15);
  display_wrapAlign(items[SCREEN_ITEM_SETTINGS_TANK], "TANK", tankValue, 15);
  display_wrapAlign(items[SCREEN_ITEM_SETTINGS_BACKLIGHT], "BACKLIGHT", backlightValue, 15);
  display_wrapAlign(items[SCREEN_ITEM_SETTINGS_FAN_AT], "FAN AT", fanAtValue, 15);
  display_wrapAlign(items[SCREEN_ITEM_SETTINGS_PUMP_AT], "PUMP AT", pumpAtValue, 15);
  display_wrapAlign(items[SCREEN_ITEM_SETTINGS_BOIL_TEMP], "BOIL AT", waterBoilTempValue, 15);
  display_render_iterable_menu(items, 6, display_activeIterable[SCREEN_SETTINGS], display_activeIterablePrevious[SCREEN_SETTINGS]);
}

inline void display_program() {
  char items[PROGRAM_ITEMS_MAX_COUNT + 1][17];

  for (byte i = 0; i < brew_programLength; i++) {
    items[i][0] = '\0';
    display_program_print_item_oneline(items[i], i);
  }
  strcpy(items[brew_programLength], "ADD NEW STEP");
  display_render_iterable_menu(items, brew_programLength + 1, display_activeIterable[SCREEN_PROGRAM], display_activeIterablePrevious[SCREEN_PROGRAM]);
}

inline void display_program_edit() {
  char items[5][17];

  bool existing = brew_programLength >= (display_programEditItem + 1);
  
  if (existing) {
    strcpy(display_firstLine, "#");
    itoa(display_programEditItem + 1, display_firstLine + strlen(display_firstLine), 10);
    strcpy(display_firstLine + strlen(display_firstLine), ": ");
  }
  else {
    strcpy(display_firstLine, "ADD: ");
  }

  strcpy(display_firstLine + strlen(display_firstLine), display_activeIterable[SCREEN_PROGRAM_EDIT] == 0 ? pointerSymbol : " ");

  switch (display_programEdit_A) {
    case PROGRAM_ITEM_MASH:
      display_iterableCount = 3;
      strcpy(display_firstLine + strlen(display_firstLine), "MASH");
      strcpy(display_secondLine, display_activeIterable[SCREEN_PROGRAM_EDIT] == 1 ? pointerSymbol : " ");
      display_format_minutes(display_secondLine, display_programEdit_B);
      strcpy(display_secondLine + strlen(display_secondLine), " AT ");
      strcpy(display_secondLine + strlen(display_secondLine), display_activeIterable[SCREEN_PROGRAM_EDIT] == 2 ? pointerSymbol : " ");
      itoa(display_programEdit_C, display_secondLine + strlen(display_secondLine), 10);
      strcpy(display_secondLine + strlen(display_secondLine), degreeSymbol);
      break;
      
    case PROGRAM_ITEM_BOILING:
      display_iterableCount = 2;
      strcpy(display_firstLine + strlen(display_firstLine), "BOIL");
      strcpy(display_secondLine, display_activeIterable[SCREEN_PROGRAM_EDIT] == 1 ? pointerSymbol : " ");
      display_format_minutes(display_secondLine, display_programEdit_B);
      strcpy(display_secondLine + strlen(display_secondLine), " AT 100");
      strcpy(display_secondLine + strlen(display_secondLine), degreeSymbol);
      break;

    case PROGRAM_ITEM_REMINDER_TIME:
      display_iterableCount = 2;
      strcpy(display_firstLine + strlen(display_firstLine), "ALARM");
      strcpy(display_secondLine, "AFTER ");
      strcpy(display_secondLine + strlen(display_secondLine), display_activeIterable[SCREEN_PROGRAM_EDIT] == 1 ? pointerSymbol : " ");
      display_format_minutes(display_secondLine, display_programEdit_B);
      break;
      
    case PROGRAM_ITEM_REMINDER_TEMP:
      display_iterableCount = 2;
      strcpy(display_firstLine + strlen(display_firstLine), "ALARM");
      strcpy(display_secondLine, "ON ");
      strcpy(display_secondLine + strlen(display_secondLine), display_activeIterable[SCREEN_PROGRAM_EDIT] == 1 ? pointerSymbol : " ");
      itoa(display_programEdit_B, display_secondLine + strlen(display_secondLine), 10);
      strcpy(display_secondLine + strlen(display_secondLine), degreeSymbol);
      break;
  }
}

inline void display_brew_control() {
  char items[2][17];
  byte itemsCount = 1;

  if (brew_status == BREW_STATUS_IDLE) {
    strcpy(items[SCREEN_ITEM_BREW_CONTROL_PLAY_PAUSE], "START BREW");
  }
  else if (brew_status == BREW_STATUS_WORKING) {
    strcpy(items[SCREEN_ITEM_BREW_CONTROL_PLAY_PAUSE], "PAUSE BREW");
  }
  else if (brew_status == BREW_STATUS_PAUSED) {
    strcpy(items[SCREEN_ITEM_BREW_CONTROL_PLAY_PAUSE], "CONTINUE BREW");
  }

  if (brew_status != BREW_STATUS_IDLE) {
    itemsCount = 2;
    strcpy(items[SCREEN_ITEM_BREW_CONTROL_STOP], "STOP BREW");
  }
  else {
    display_activeIterable[SCREEN_BREW_CONTROL] = SCREEN_ITEM_BREW_CONTROL_PLAY_PAUSE;
  }
  display_render_iterable_menu(items, itemsCount, display_activeIterable[SCREEN_BREW_CONTROL], display_activeIterablePrevious[SCREEN_BREW_CONTROL]);
}

inline void display_heater_control() {
  char items[2][17];
  byte itemsCount = 1;

  char heaterModeValue[7];
  switch (heater_relayMode) {
    case RELAY_MODE_AUTO:   strcpy(heaterModeValue, "AUTO");   break;
    case RELAY_MODE_MANUAL: strcpy(heaterModeValue, "MANUAL"); break;
  }

  char heaterValue[4];
  strcpy(heaterValue, relay_heaterEnabled ? "ON" : "OFF");

  if (heater_relayMode == RELAY_MODE_MANUAL) {
    itemsCount = 2;
  }
  else {
    display_activeIterable[SCREEN_HEATER_CONTROL] = SCREEN_ITEM_HEATER_CONTROL_MODE;
  }

  display_wrapAlign(items[SCREEN_ITEM_HEATER_CONTROL_MODE], "HEATER", heaterModeValue, 15);
  display_wrapAlign(items[SCREEN_ITEM_HEATER_CONTROL_TOGGLE], "STATE", heaterValue, 15);
  display_render_iterable_menu(items, itemsCount, display_activeIterable[SCREEN_HEATER_CONTROL], display_activeIterablePrevious[SCREEN_HEATER_CONTROL]);
}

inline void display_pump_control() {
  char items[2][17];
  byte itemsCount = 1;

  char pumpModeValue[7];
  switch (pump_relayMode) {
    case RELAY_MODE_AUTO:   strcpy(pumpModeValue, "AUTO");   break;
    case RELAY_MODE_MANUAL: strcpy(pumpModeValue, "MANUAL"); break;
  }

  char pumpValue[4];
  strcpy(pumpValue, relay_pumpEnabled ? "ON" : "OFF");

  if (pump_relayMode == RELAY_MODE_MANUAL) {
    itemsCount = 2;
  }
  else {
    display_activeIterable[SCREEN_PUMP_CONTROL] = SCREEN_ITEM_PUMP_CONTROL_MODE;
  }

  display_wrapAlign(items[SCREEN_ITEM_PUMP_CONTROL_MODE], "PUMP", pumpModeValue, 15);
  display_wrapAlign(items[SCREEN_ITEM_PUMP_CONTROL_TOGGLE], "STATE", pumpValue, 15);
  display_render_iterable_menu(items, itemsCount, display_activeIterable[SCREEN_PUMP_CONTROL], display_activeIterablePrevious[SCREEN_PUMP_CONTROL]);}

inline void display_credits() {
  strcpy(display_firstLine, "DEVOTED TO");
  strcpy(display_secondLine, "MIKE POMYTKIN");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void display_render() {
  switch (display_screenCurrent) {
    case SCREEN_DASHBOARD:      display_render_dashboard(); break;
    case SCREEN_MAIN:           display_render_main();      break;
    case SCREEN_SETTINGS:       display_render_settings();  break;
    case SCREEN_PROGRAM:        display_program();          break;
    case SCREEN_PROGRAM_EDIT:   display_program_edit();     break;
    case SCREEN_BREW_CONTROL:   display_brew_control();     break;
    case SCREEN_HEATER_CONTROL: display_heater_control();   break;
    case SCREEN_PUMP_CONTROL:   display_pump_control();     break;
    case SCREEN_CREDITS:        display_credits();          break;
  }

  if (strcmp(display_firstLine, display_firstLine_previous) != 0) {
    byte length = strlen(display_firstLine);
    if (length < 16) for (byte i = length; i < 16; i++) strcpy(display_firstLine + strlen(display_firstLine), " ");
    strcpy(display_firstLine_previous, display_firstLine);
    lcd.setCursor(0, 0);
    lcd.print(display_firstLine);
  }

  if (strcmp(display_secondLine, display_secondLine_previous) != 0) {
    byte length = strlen(display_secondLine);
    if (length < 16) for (byte i = length; i < 16; i++) strcpy(display_secondLine + strlen(display_secondLine), " ");
    strcpy(display_secondLine_previous, display_secondLine);
    lcd.setCursor(0, 1);
    lcd.print(display_secondLine);
  }
}

inline void display_changeScreen(byte screen) {
  display_screenCurrent = screen; 
  display_render();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void display_dashboard_listeners() {
  if (keyboard_escapePressed && keyboard_shortPress) {
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_MAIN);
  }
  else if (keyboard_enterPressed && keyboard_shortPress) {
    if (brew_programLength > 0) {
      display_screenBack = SCREEN_DASHBOARD;
      display_changeScreen(SCREEN_BREW_CONTROL);
    }
    else {
      buzzer_error();
    }
    keyboard_releaseKeys();
  }
  else if (keyboard_upPressed && keyboard_shortPress) {
    keyboard_releaseKeys();
    display_screenBack = SCREEN_DASHBOARD;
    display_changeScreen(SCREEN_HEATER_CONTROL);
  }
  else if (keyboard_downPressed && keyboard_shortPress) {
    keyboard_releaseKeys();
    display_screenBack = SCREEN_DASHBOARD;
    display_changeScreen(SCREEN_PUMP_CONTROL);
  }
  else if ((keyboard_leftPressed || keyboard_rightPressed) && keyboard_shortPress) {
    keyboard_rightPressed ? display_activeIterable[SCREEN_DASHBOARD]++ : display_activeIterable[SCREEN_DASHBOARD]--;
    if (display_activeIterable[SCREEN_DASHBOARD] >= display_messagesCount) { display_activeIterable[SCREEN_DASHBOARD] = 0; }
    else if (display_activeIterable[SCREEN_DASHBOARD] < 0) { display_activeIterable[SCREEN_DASHBOARD] = display_messagesCount - 1; }
    keyboard_releaseKeys();
  }
}

inline void display_main_listeners() {
  if (keyboard_escapePressed && keyboard_shortPress) {
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_DASHBOARD);
  }
  else if ((keyboard_upPressed || keyboard_downPressed) && keyboard_shortPress) {
    display_activeIterablePrevious[SCREEN_MAIN] = display_activeIterable[SCREEN_MAIN];
    keyboard_downPressed ? display_activeIterable[SCREEN_MAIN]++ : display_activeIterable[SCREEN_MAIN]--;
    if (display_activeIterable[SCREEN_MAIN] >= display_iterableCount) { display_activeIterable[SCREEN_MAIN] = 0; }
    else if (display_activeIterable[SCREEN_MAIN] < 0) { display_activeIterable[SCREEN_MAIN] = display_iterableCount - 1; }
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_MAIN);
  }
  else if (display_activeIterable[SCREEN_MAIN] == SCREEN_ITEM_MAIN_PROGRAM && keyboard_enterPressed && keyboard_shortPress) {
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_PROGRAM);
  }
  else if (display_activeIterable[SCREEN_MAIN] == SCREEN_ITEM_MAIN_BREWING && keyboard_enterPressed && keyboard_shortPress) {
    if (brew_programLength > 0) {
      display_screenBack = SCREEN_MAIN;
      display_changeScreen(SCREEN_BREW_CONTROL);
    }
    else {
      buzzer_error();
    }
    keyboard_releaseKeys();
  }
  else if (display_activeIterable[SCREEN_MAIN] == SCREEN_ITEM_MAIN_HEATER && keyboard_enterPressed && keyboard_shortPress) {
    keyboard_releaseKeys();
    display_screenBack = SCREEN_MAIN;
    display_changeScreen(SCREEN_HEATER_CONTROL);
  }
  else if (display_activeIterable[SCREEN_MAIN] == SCREEN_ITEM_MAIN_PUMP && keyboard_enterPressed && keyboard_shortPress) {
    keyboard_releaseKeys();
    display_screenBack = SCREEN_MAIN;
    display_changeScreen(SCREEN_PUMP_CONTROL);
  }
  else if (display_activeIterable[SCREEN_MAIN] == SCREEN_ITEM_MAIN_SETTINGS && keyboard_enterPressed && keyboard_shortPress) {
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_SETTINGS);
  }
  else if (display_activeIterable[SCREEN_MAIN] == SCREEN_ITEM_MAIN_CREDITS && keyboard_enterPressed && keyboard_shortPress) {
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_CREDITS);
  }
}

inline void display_settings_listeners() {  
  if ((keyboard_escapePressed || keyboard_enterPressed) && keyboard_shortPress) {
    event_settingsChanged = true;
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_MAIN);
  }
  else if ((keyboard_upPressed || keyboard_downPressed) && keyboard_shortPress) {
    display_activeIterablePrevious[SCREEN_SETTINGS] = display_activeIterable[SCREEN_SETTINGS];
    keyboard_downPressed ? display_activeIterable[SCREEN_SETTINGS]++ : display_activeIterable[SCREEN_SETTINGS]--;
    if (display_activeIterable[SCREEN_SETTINGS] >= display_iterableCount) { display_activeIterable[SCREEN_SETTINGS] = 0; }
    else if (display_activeIterable[SCREEN_SETTINGS] < 0) { display_activeIterable[SCREEN_SETTINGS] = display_iterableCount - 1; }
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_SETTINGS);
  }
  else if ((keyboard_leftPressed || keyboard_rightPressed) && keyboard_shortPress) {
    switch (display_activeIterable[SCREEN_SETTINGS]) {
      case SCREEN_ITEM_SETTINGS_POWER:
        if      (keyboard_leftPressed  && setting_heaterPower > 500)  setting_heaterPower = setting_heaterPower - 50;
        else if (keyboard_rightPressed && setting_heaterPower < 4400) setting_heaterPower = setting_heaterPower + 50;
        else buzzer_error();
        break;
      case SCREEN_ITEM_SETTINGS_TANK:
        if      (keyboard_leftPressed  && setting_tankVolume > 5)  setting_tankVolume--;
        else if (keyboard_rightPressed && setting_tankVolume < 60) setting_tankVolume++;
        else buzzer_error();
        break;
      case SCREEN_ITEM_SETTINGS_BACKLIGHT:
        if      (keyboard_leftPressed  && setting_backlightLevel > 0)   setting_backlightLevel = setting_backlightLevel - 5;
        else if (keyboard_rightPressed && setting_backlightLevel < 100) setting_backlightLevel = setting_backlightLevel + 5;
        else buzzer_error();
        analogWrite(PIN_DISPLAY_BACKLIGHT, round(255 / 100 * setting_backlightLevel));
        break;
      case SCREEN_ITEM_SETTINGS_FAN_AT:
        if      (keyboard_leftPressed  && setting_fanTemp > 20) setting_fanTemp = setting_fanTemp - 5;
        else if (keyboard_rightPressed && setting_fanTemp < 80) setting_fanTemp = setting_fanTemp + 5;
        else buzzer_error();
        break;
      case SCREEN_ITEM_SETTINGS_PUMP_AT:
        if      (keyboard_leftPressed  && setting_pumpTempDelta > 0.1) setting_pumpTempDelta = setting_pumpTempDelta - 0.1;
        else if (keyboard_rightPressed && setting_pumpTempDelta < 2.0) setting_pumpTempDelta = setting_pumpTempDelta + 0.1;
        else buzzer_error();
        break;
      case SCREEN_ITEM_SETTINGS_BOIL_TEMP:
        if      (keyboard_leftPressed  && setting_waterBoilTemp > 95.0)  setting_waterBoilTemp = setting_waterBoilTemp - 0.1;
        else if (keyboard_rightPressed && setting_waterBoilTemp < 105.0) setting_waterBoilTemp = setting_waterBoilTemp + 0.1;
        else buzzer_error();
        break;
    }
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_SETTINGS);
  }
}

inline void display_program_listeners() {
  if (keyboard_escapePressed && keyboard_shortPress) {
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_MAIN);
  }
  else if ((keyboard_upPressed || keyboard_downPressed) && keyboard_shortPress) {
    display_activeIterablePrevious[SCREEN_PROGRAM] = display_activeIterable[SCREEN_PROGRAM];
    keyboard_downPressed ? display_activeIterable[SCREEN_PROGRAM]++ : display_activeIterable[SCREEN_PROGRAM]--;
    if (display_activeIterable[SCREEN_PROGRAM] >= display_iterableCount) { display_activeIterable[SCREEN_PROGRAM] = 0; }
    else if (display_activeIterable[SCREEN_PROGRAM] < 0) { display_activeIterable[SCREEN_PROGRAM] = display_iterableCount - 1; }
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_PROGRAM);
  }
  else if ((keyboard_upPressed || keyboard_downPressed) && keyboard_longPress) {
    bool isProgramItem = display_activeIterable[SCREEN_PROGRAM] <= brew_programLength - 1;
    if (isProgramItem) {
      int temp[3];
      byte currentIndex = display_activeIterable[SCREEN_PROGRAM];
      char moveToIndex = keyboard_upPressed ? currentIndex - 1 : currentIndex + 1;
      if (moveToIndex >= brew_programLength) { moveToIndex = 0; }
      else if (moveToIndex < 0) { moveToIndex = brew_programLength - 1; }

      memcpy(temp, brew_program[moveToIndex], sizeof(brew_program[moveToIndex]));
      memcpy(brew_program[moveToIndex], brew_program[currentIndex], sizeof(brew_program[currentIndex]));
      memcpy(brew_program[currentIndex], temp, sizeof(temp));

      display_activeIterable[SCREEN_PROGRAM] = moveToIndex;
      display_activeIterablePrevious[SCREEN_PROGRAM] = currentIndex;

      event_programChanged = true;
      keyboard_releaseKeys();
      display_changeScreen(SCREEN_PROGRAM);
    }
  }
  else if (keyboard_enterPressed && keyboard_shortPress) {
    bool existing = brew_programLength >= (display_programEditItem + 1);

    display_activeIterable[SCREEN_PROGRAM_EDIT] = 0;

    if (existing) {
      display_programEditItem = display_activeIterable[SCREEN_PROGRAM];
      display_programEdit_A = brew_program[display_programEditItem][0];
      display_programEdit_B = brew_program[display_programEditItem][1];
      display_programEdit_C = brew_program[display_programEditItem][2];
    }

    display_programEditItem = display_activeIterable[SCREEN_PROGRAM];
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_PROGRAM_EDIT);
  }
}

inline void display_program_edit_listeners() {
  if (keyboard_escapePressed && keyboard_shortPress) {
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_PROGRAM);
  }
  else if ((keyboard_upPressed || keyboard_downPressed) && keyboard_shortPress) {
    keyboard_downPressed ? display_activeIterable[SCREEN_PROGRAM_EDIT]++ : display_activeIterable[SCREEN_PROGRAM_EDIT]--;
    if (display_activeIterable[SCREEN_PROGRAM_EDIT] >= display_iterableCount) { display_activeIterable[SCREEN_PROGRAM_EDIT] = 0; }
    else if (display_activeIterable[SCREEN_PROGRAM_EDIT] < 0) { display_activeIterable[SCREEN_PROGRAM_EDIT] = display_iterableCount - 1; }
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_PROGRAM_EDIT);
  }
  else if ((keyboard_leftPressed || keyboard_rightPressed) && keyboard_shortPress) {
    // TODO: replace to switch()
    if (display_activeIterable[SCREEN_PROGRAM_EDIT] == 0) {
      keyboard_leftPressed ? display_programEdit_A-- : display_programEdit_A++;
      if (display_programEdit_A < 0) display_programEdit_A = 4 - 1;
      else if (display_programEdit_A >= 4) display_programEdit_A = 0;
    }
    else if (display_activeIterable[SCREEN_PROGRAM_EDIT] == 1) {
      keyboard_leftPressed ? display_programEdit_B-- : display_programEdit_B++;
    } 
    else if (display_activeIterable[SCREEN_PROGRAM_EDIT] == 2) {
      keyboard_leftPressed ? display_programEdit_C-- : display_programEdit_C++;
    }

    if (display_programEdit_B < 0) display_programEdit_B = 0;
    if (display_programEdit_C < 0) display_programEdit_C = 0;

    keyboard_releaseKeys();
    display_changeScreen(SCREEN_PROGRAM_EDIT);
  }
  else if ((keyboard_leftPressed || keyboard_rightPressed) && (keyboard_holdPress || keyboard_longPress)) {
    if (display_activeIterable[SCREEN_PROGRAM_EDIT] == 1) {
      display_programEdit_B = keyboard_leftPressed 
        ? display_programEdit_B - 10 
        : display_programEdit_B + 10;
    } 
    else if (display_activeIterable[SCREEN_PROGRAM_EDIT] == 2) {
      display_programEdit_C = keyboard_leftPressed 
        ? display_programEdit_C - 10 
        : display_programEdit_C + 10;
    }

    if (display_programEdit_B < 0) display_programEdit_B = 0;
    if (display_programEdit_C < 0) display_programEdit_C = 0;

    keyboard_releaseKeys();
    display_changeScreen(SCREEN_PROGRAM_EDIT);
  }
  else if (keyboard_enterPressed && keyboard_longPress) {
    bool existing = brew_programLength >= (display_programEditItem + 1);
    
    brew_program[display_programEditItem][0] = display_programEdit_A;
    brew_program[display_programEditItem][1] = display_programEdit_B;
    brew_program[display_programEditItem][2] = display_programEdit_C;

    if (! existing) {
      brew_programLength++;
    }

    event_programChanged = true;
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_PROGRAM);
  }
  else if (keyboard_escapePressed && keyboard_longPress) {
    bool existing = brew_programLength >= (display_programEditItem + 1);

    if (existing) {
      if (display_programEditItem < brew_programLength - 1) {
        for (byte i = display_programEditItem; i < brew_programLength - 1; i++) {
          brew_program[i][0] = brew_program[i+1][0];
          brew_program[i][1] = brew_program[i+1][1];
          brew_program[i][2] = brew_program[i+1][2];
        }
      }

      brew_program[brew_programLength - 1][0] = 0;
      brew_program[brew_programLength - 1][1] = 0;
      brew_program[brew_programLength - 1][2] = 0;
      brew_programLength--;

      event_programChanged = true;
      keyboard_releaseKeys();
      display_changeScreen(SCREEN_PROGRAM);
    }
  }
}

inline void display_brew_control_listeners() {
  if (keyboard_escapePressed && keyboard_shortPress) {
    keyboard_releaseKeys();
    display_changeScreen(display_screenBack);
  }
  else if (keyboard_enterPressed && keyboard_shortPress) {
    if (brew_programLength > 0) {
      switch (display_activeIterable[SCREEN_BREW_CONTROL]) {
        case SCREEN_ITEM_BREW_CONTROL_PLAY_PAUSE:
          if (brew_status == BREW_STATUS_IDLE || brew_status == BREW_STATUS_PAUSED) {
            display_messagesCount = 0;
            brew_start();
          }
          else if (brew_status == BREW_STATUS_WORKING) {
            display_messagesCount = 0;
            brew_pause();
          }
          break;
        case SCREEN_ITEM_BREW_CONTROL_STOP:
          display_messagesCount = 0;
          brew_stop();
          break;
      }
    }
    else {
      buzzer_error();
    }

    keyboard_releaseKeys();
    display_changeScreen(display_screenBack);
  }
  else if ((keyboard_upPressed || keyboard_downPressed) && keyboard_shortPress) {
    display_activeIterablePrevious[SCREEN_BREW_CONTROL] = display_activeIterable[SCREEN_BREW_CONTROL];
    keyboard_downPressed ? display_activeIterable[SCREEN_BREW_CONTROL]++ : display_activeIterable[SCREEN_BREW_CONTROL]--;
    if (display_activeIterable[SCREEN_BREW_CONTROL] >= display_iterableCount) { display_activeIterable[SCREEN_BREW_CONTROL] = 0; }
    else if (display_activeIterable[SCREEN_BREW_CONTROL] < 0) { display_activeIterable[SCREEN_BREW_CONTROL] = display_iterableCount - 1; }
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_BREW_CONTROL);
  }
}

inline void display_heater_control_listeners() {
  if (keyboard_escapePressed && keyboard_shortPress) {
    keyboard_releaseKeys();
    display_changeScreen(display_screenBack);
  }
  else if ((keyboard_upPressed || keyboard_downPressed) && keyboard_shortPress) {
    display_activeIterablePrevious[SCREEN_HEATER_CONTROL] = display_activeIterable[SCREEN_HEATER_CONTROL];
    keyboard_downPressed ? display_activeIterable[SCREEN_HEATER_CONTROL]++ : display_activeIterable[SCREEN_HEATER_CONTROL]--;
    if (display_activeIterable[SCREEN_HEATER_CONTROL] >= display_iterableCount) { display_activeIterable[SCREEN_HEATER_CONTROL] = 0; }
    else if (display_activeIterable[SCREEN_HEATER_CONTROL] < 0) { display_activeIterable[SCREEN_HEATER_CONTROL] = display_iterableCount - 1; }
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_HEATER_CONTROL);
  }
  else if ((keyboard_leftPressed || keyboard_rightPressed) && keyboard_shortPress) {
    switch (display_activeIterable[SCREEN_HEATER_CONTROL]) {
      case SCREEN_ITEM_HEATER_CONTROL_MODE:
        if (brew_status == BREW_STATUS_WORKING) {
          buzzer_error();
        }
        else {
          heater_relayMode = heater_relayMode == RELAY_MODE_AUTO ? RELAY_MODE_MANUAL : RELAY_MODE_AUTO;
        }
        break;
      case SCREEN_ITEM_HEATER_CONTROL_TOGGLE:
        if (brew_status == BREW_STATUS_WORKING) {
          buzzer_error();
        }
        else {
          relay_heaterEnabled ? relays_turn_off_heater() : relays_turn_on_heater();
        }
        break;
    }

    keyboard_releaseKeys();
    display_changeScreen(SCREEN_HEATER_CONTROL);
  }
}

inline void display_pump_control_listeners() {
  if (keyboard_escapePressed && keyboard_shortPress) {
    keyboard_releaseKeys();
    display_changeScreen(display_screenBack);
  }
  else if ((keyboard_upPressed || keyboard_downPressed) && keyboard_shortPress) {
    display_activeIterablePrevious[SCREEN_PUMP_CONTROL] = display_activeIterable[SCREEN_PUMP_CONTROL];
    keyboard_downPressed ? display_activeIterable[SCREEN_PUMP_CONTROL]++ : display_activeIterable[SCREEN_PUMP_CONTROL]--;
    if (display_activeIterable[SCREEN_PUMP_CONTROL] >= display_iterableCount) { display_activeIterable[SCREEN_PUMP_CONTROL] = 0; }
    else if (display_activeIterable[SCREEN_PUMP_CONTROL] < 0) { display_activeIterable[SCREEN_PUMP_CONTROL] = display_iterableCount - 1; }
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_PUMP_CONTROL);
  }
  else if ((keyboard_leftPressed || keyboard_rightPressed) && keyboard_shortPress) {
    switch (display_activeIterable[SCREEN_PUMP_CONTROL]) {
      case SCREEN_ITEM_PUMP_CONTROL_MODE:
        if (brew_status == BREW_STATUS_WORKING) {
          buzzer_error();
        }
        else {
          pump_relayMode = pump_relayMode == RELAY_MODE_AUTO ? RELAY_MODE_MANUAL : RELAY_MODE_AUTO;
        }
        break;
      case SCREEN_ITEM_PUMP_CONTROL_TOGGLE:
        if (brew_status == BREW_STATUS_WORKING) {
          buzzer_error();
        }
        else {
          relay_pumpEnabled ? relays_turn_off_pump() : relays_turn_on_pump();
        }
        break;
    }

    keyboard_releaseKeys();
    display_changeScreen(SCREEN_PUMP_CONTROL);
  }
}

inline void display_credits_listeners() {
  if ((keyboard_escapePressed || keyboard_enterPressed) && keyboard_shortPress) {
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_MAIN);
  }
}

inline void display_loop(unsigned long now) {
  if (now - lastRun_loopDisplay < LOOP_THRESHOLD_DISPLAY) return;
  lastRun_loopDisplay = now;

  switch (display_screenCurrent) {
    case SCREEN_DASHBOARD:      display_dashboard_listeners();      break;
    case SCREEN_MAIN:           display_main_listeners();           break;
    case SCREEN_SETTINGS:       display_settings_listeners();       break;
    case SCREEN_PROGRAM:        display_program_listeners();        break;
    case SCREEN_PROGRAM_EDIT:   display_program_edit_listeners();   break;
    case SCREEN_BREW_CONTROL:   display_brew_control_listeners();   break;
    case SCREEN_HEATER_CONTROL: display_heater_control_listeners(); break;
    case SCREEN_PUMP_CONTROL:   display_pump_control_listeners();   break;
    case SCREEN_CREDITS:        display_credits_listeners();        break;
  }

  if (display_willChange > 0 && now > display_willChange) {
    display_willChange = 0;
    display_render();
  }

  if (display_screenCurrent != display_screenPrevious) {
    display_screenPrevious = display_screenCurrent;
    display_changeScreen(display_screenCurrent);
  }
}