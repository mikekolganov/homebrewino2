#include "constants.h"
#include "variables.h"

inline char* display_iterable(char items[][16], byte count, byte activeItem, byte prevActiveItem) {
  char result[32];

  char pointer[1] = { POINTER_SYMBOL };
  if (count == 1) {
    strcpy(result, pointer);
    strcpy(result + 1, items[0]);
    return result;
  }
  if (count == 2) {
    strcpy(result, activeItem == 0 ? pointer : " ");
    strcpy(result + 1, items[0]);
    strcpy(result + 16, activeItem == 1 ? pointer : " ");
    strcpy(result + 17, items[1]);
    return result;
  }

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
      strcpy(result, pointer);
      strcpy(result + 1, items[i]);
      strcpy(result + 16, " ");
      strcpy(result + 17, (i == count - 1) ? items[0] : items[i + 1]);
    }
    else {
      strcpy(result, " ");
      strcpy(result + 1, (i == 0) ? items[count - 1] : items[i - 1]);
      strcpy(result + 16, pointer);
      strcpy(result + 17, items[i]);
    }
  }

  return result;
}

void inline display_dashboard() {
  unsigned long now = millis();
  char s_temp1[7];
  char s_temp2[7];
  dtostrf(sensor_brewing1, 4, 1, s_temp1);
  dtostrf(sensor_brewing2, 4, 1, s_temp2);

  s_temp1[strlen(s_temp1)+1] = '\0';
  s_temp2[strlen(s_temp2)+1] = '\0';
  s_temp1[strlen(s_temp1)] = DEGREE_SYMBOL;
  s_temp2[strlen(s_temp2)] = DEGREE_SYMBOL;

  sprintf(display_firstLine, "%s %s", s_temp1, s_temp2);

  char s2[10] = "";
  strcpy(display_secondLine, s2);

  display_willChange = now + 500;
}

void inline display_main() {
  char items[SCREEN_ITEMS_MAIN][16];
  strcpy(items[SCREEN_ITEM_MAIN_PROGRAM], "PROGRAM");
  strcpy(items[SCREEN_ITEM_MAIN_SETTINGS], "SETTINGS");
  display_iterableCount = 2;
  char* menu = display_iterable(items, SCREEN_ITEMS_MAIN, display_activeIterable[SCREEN_MAIN], display_activeIterablePrevious[SCREEN_MAIN]);
  strncpy(display_firstLine, menu, 16);
  strncpy(display_secondLine, menu+16, 16);
}

void inline display_settings() {
  char items[4][16];
  strcpy(items[SCREEN_ITEM_SETTINGS_POWER], "POWER");
  strcpy(items[SCREEN_ITEM_SETTINGS_TANK], "TANK");
  strcpy(items[SCREEN_ITEM_SETTINGS_BACKLIGHT], "BACKLIGHT");
  strcpy(items[SCREEN_ITEM_SETTINGS_FAN_AT], "FAN AT");
  display_iterableCount = 4;
  char* menu = display_iterable(items, 4, display_activeIterable[SCREEN_SETTINGS], display_activeIterablePrevious[SCREEN_SETTINGS]);
  strncpy(display_firstLine, menu, 16);
  strncpy(display_secondLine, menu+16, 16);
}

char* display_program_print_item_oneline(byte itemIndex) {
  char result[16];
  switch (brew_program[itemIndex][0]) {
    case PROGRAM_ITEM_MASH:
      strcpy(result, "MASH 0:25 62");
      result[strlen(result)+1] = '\0';
      result[strlen(result)] = DEGREE_SYMBOL;
      break;
    case PROGRAM_ITEM_BOILING:
      strcpy(result, "BOIL 1:10");
      break;
    case PROGRAM_ITEM_REMINDER_TIME:
      strcpy(result, "PING +0:15");
      break;
    case PROGRAM_ITEM_REMINDER_TEMP:
      strcpy(result, "PING 56");
      result[strlen(result)+1] = '\0';
      result[strlen(result)] = DEGREE_SYMBOL;
      break;
    default:
      strcpy(result, "FAIL");
  }
  return result;
}

void inline display_program() {
  char items[PROGRAM_ITEMS_MAX_COUNT + 1][16];
  char* menu;

  for (byte i = 0; i < brew_programLength; i++) {
    char* item = display_program_print_item_oneline(i);
    strcpy(items[i], item);
  }
  strcpy(items[brew_programLength], "ADD NEW STEP");
  display_iterableCount = brew_programLength + 1;
  menu = display_iterable(items, brew_programLength + 1, display_activeIterable[SCREEN_PROGRAM], display_activeIterablePrevious[SCREEN_PROGRAM]);

  strncpy(display_firstLine, menu, 16);
  strncpy(display_secondLine, menu+16, 16);
}

void inline display_program_edit() {
  byte counter;
  char items[5][16];
  char degree[1] = { DEGREE_SYMBOL };

  bool existing = brew_programLength >= (display_programEditItem + 1);
  
  if (existing) {
    strcpy(display_firstLine, "#");
    itoa(display_programEditItem + 1, display_firstLine + strlen(display_firstLine), 10);
    strcpy(display_firstLine + strlen(display_firstLine), ": ");
  }
  else {
    strcpy(display_firstLine, "NEW: ");
  }

  counter = strlen(display_firstLine);
  display_firstLine[counter] = display_activeIterable[SCREEN_PROGRAM_EDIT] == 0 ? POINTER_SYMBOL : ' ';
  display_firstLine[++counter] = '\0';

  display_iterableCount = 3;

  switch (display_programEdit_A) {
    case PROGRAM_ITEM_MASH:
      strcpy(display_firstLine + strlen(display_firstLine), "MASH");
      break;
    case PROGRAM_ITEM_BOILING:
      strcpy(display_firstLine + strlen(display_firstLine), "BOILING");
      break;
    case PROGRAM_ITEM_REMINDER_TIME:
      strcpy(display_firstLine + strlen(display_firstLine), "AT TIME");
      break;
    case PROGRAM_ITEM_REMINDER_TEMP:
      strcpy(display_firstLine + strlen(display_firstLine), "AT TEMP");
      break;
  }

  display_secondLine[0] = display_activeIterable[SCREEN_PROGRAM_EDIT] == 1 ? POINTER_SYMBOL : ' ';
  display_secondLine[1] = '\0';
  itoa(display_programEdit_B, display_secondLine + strlen(display_secondLine), 10);

  strcpy(display_secondLine + strlen(display_secondLine), " AT ");

  counter = strlen(display_secondLine);
  display_secondLine[counter] = display_activeIterable[SCREEN_PROGRAM_EDIT] == 2 ? POINTER_SYMBOL : ' ';
  display_secondLine[++counter] = '\0';
  itoa(display_programEdit_C, display_secondLine + strlen(display_secondLine), 10);

  counter = strlen(display_secondLine);
  display_secondLine[counter] = DEGREE_SYMBOL;
  display_secondLine[++counter] = '\0';
}

void inline display_render() {
  switch (display_screenCurrent) {
    case SCREEN_DASHBOARD:    display_dashboard();    break;
    case SCREEN_MAIN:         display_main();         break;
    case SCREEN_SETTINGS:     display_settings();     break;
    case SCREEN_PROGRAM:      display_program();      break;
    case SCREEN_PROGRAM_EDIT: display_program_edit(); break;
  }
  lcd.clear();
  lcd.print(display_firstLine);
  lcd.setCursor(0, 1);
  lcd.print(display_secondLine);
}

void inline display_changeScreen(byte screen) {
  display_screenCurrent = screen; 
  display_render();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void inline display_dashboard_listeners() {
  if (keyboard_escapePressed && keyboard_shortPress) {
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_MAIN);
  }
}

void inline display_main_listeners() {
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
  else if (display_activeIterable[SCREEN_MAIN] == SCREEN_ITEM_MAIN_SETTINGS && keyboard_enterPressed && keyboard_shortPress) {
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_SETTINGS);
  }
  else if (display_activeIterable[SCREEN_MAIN] == SCREEN_ITEM_MAIN_PROGRAM && keyboard_enterPressed && keyboard_shortPress) {
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_PROGRAM);
  }
}

void inline display_settings_listeners() {  
  if (keyboard_escapePressed && keyboard_shortPress) {
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
}

void inline display_program_listeners() {
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
  else if (keyboard_enterPressed && keyboard_shortPress) {
    display_programEditItem = display_activeIterable[SCREEN_PROGRAM];
    display_changeScreen(SCREEN_PROGRAM_EDIT);
  }
}

void inline display_program_edit_listeners() {
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
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_PROGRAM);
  }
  else if (keyboard_escapePressed && keyboard_longPress) {
    Serial.println("DELETE");
    Serial.println(brew_programLength);
    Serial.println(display_programEditItem);
    Serial.println("---");
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_PROGRAM);
  }
}

void inline display_loop(unsigned long now) {
  if (now - lastRun_loopDisplay < LOOP_THRESHOLD_DISPLAY) return;
  lastRun_loopDisplay = now;

  switch (display_screenCurrent) {
    case SCREEN_DASHBOARD:    display_dashboard_listeners();    break;
    case SCREEN_MAIN:         display_main_listeners();         break;
    case SCREEN_SETTINGS:     display_settings_listeners();     break;
    case SCREEN_PROGRAM:      display_program_listeners();      break;
    case SCREEN_PROGRAM_EDIT: display_program_edit_listeners(); break;
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