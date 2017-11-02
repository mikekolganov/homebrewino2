#include "constants.h"
#include "variables.h"

inline void display_iterable(char items[][16], byte count, byte activeItem, byte prevActiveItem) {
  char result[32];
  display_iterableCount = count;

  char pointer[1] = { POINTER_SYMBOL };
  if (count == 1) {
    strcpy(result, pointer);
    strcpy(result + 1, items[0]);
    strcpy(result + 16, "                ");
  }
  else if (count == 2) {
    strcpy(result, activeItem == 0 ? pointer : " ");
    strcpy(result + 1, items[0]);
    strcpy(result + 16, activeItem == 1 ? pointer : " ");
    strcpy(result + 17, items[1]);
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
  }

  strncpy(display_firstLine, result, 16);
  strncpy(display_secondLine, result+16, 16);
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
  char items[2][16];
  strcpy(items[SCREEN_ITEM_MAIN_PROGRAM], "PROGRAM");
  strcpy(items[SCREEN_ITEM_MAIN_SETTINGS], "SETTINGS");
  display_iterable(items, 2, display_activeIterable[SCREEN_MAIN], display_activeIterablePrevious[SCREEN_MAIN]);
}

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

void inline display_settings() {
  char items[5][16];
  display_wrapAlign(items[SCREEN_ITEM_SETTINGS_POWER], "POWER", "3200W", 15);
  display_wrapAlign(items[SCREEN_ITEM_SETTINGS_TANK], "TANK", "28L", 15);
  display_wrapAlign(items[SCREEN_ITEM_SETTINGS_BACKLIGHT], "BACKLIGHT", "80%", 15);
  display_wrapAlign(items[SCREEN_ITEM_SETTINGS_FAN_AT], "FAN AT", "60*", 15);
  display_wrapAlign(items[SCREEN_ITEM_SETTINGS_PUMP_AT], "PUMP AT", "1.5*", 15);
  display_iterable(items, 5, display_activeIterable[SCREEN_SETTINGS], display_activeIterablePrevious[SCREEN_SETTINGS]);
}

inline char* formatMinutes(int minutes) {
  char result[5];
  int h = minutes / 60;
  int m = minutes % 60;

  itoa(h, result, 10);
  strcpy(result+strlen(result), ":");
  if (m < 10) strcpy(result+strlen(result), "0");
  itoa(m, result+strlen(result), 10);

  return result;
}

inline char* display_program_print_item_oneline(byte itemIndex) {
  char result[16] = "";
  char degree[1] = { char(223) };

  itoa(itemIndex + 1, result + strlen(result), 10);
  strcpy(result + strlen(result), " ");

  switch (brew_program[itemIndex][0]) {
    case PROGRAM_ITEM_MASH:
      strcpy(result + strlen(result), formatMinutes(brew_program[itemIndex][1]));
      strcpy(result + strlen(result), " AT ");
      itoa(brew_program[itemIndex][2], result + strlen(result), 10);
      strncpy(result + strlen(result), degree, 1);
      break;
    case PROGRAM_ITEM_BOILING:
      strcpy(result + strlen(result), "BOIL ");
      strcpy(result + strlen(result), formatMinutes(brew_program[itemIndex][1]));
      break;
    case PROGRAM_ITEM_REMINDER_TIME:
      strcpy(result + strlen(result), "ALARM +");
      strcpy(result + strlen(result), formatMinutes(brew_program[itemIndex][1]));
      break;
    case PROGRAM_ITEM_REMINDER_TEMP:
      strcpy(result + strlen(result), "ALARM ");
      itoa(brew_program[itemIndex][1], result + strlen(result), 10);
      strncpy(result + strlen(result), degree, 1);
      break;
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
  display_iterable(items, brew_programLength + 1, display_activeIterable[SCREEN_PROGRAM], display_activeIterablePrevious[SCREEN_PROGRAM]);
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
    strcpy(display_firstLine, "ADD: ");
  }

  counter = strlen(display_firstLine);
  display_firstLine[counter] = display_activeIterable[SCREEN_PROGRAM_EDIT] == 0 ? POINTER_SYMBOL : ' ';
  display_firstLine[++counter] = '\0';

  switch (display_programEdit_A) {
    case PROGRAM_ITEM_MASH:
      display_iterableCount = 3;
      strcpy(display_firstLine + strlen(display_firstLine), "MASH");
      
      display_secondLine[0] = display_activeIterable[SCREEN_PROGRAM_EDIT] == 1 ? POINTER_SYMBOL : ' ';
      display_secondLine[1] = '\0';
      strcpy(display_secondLine + strlen(display_secondLine), formatMinutes(display_programEdit_B));
      strcpy(display_secondLine + strlen(display_secondLine), " AT ");
    
      counter = strlen(display_secondLine);
      display_secondLine[counter] = display_activeIterable[SCREEN_PROGRAM_EDIT] == 2 ? POINTER_SYMBOL : ' ';
      display_secondLine[++counter] = '\0';
      itoa(display_programEdit_C, display_secondLine + strlen(display_secondLine), 10);
    
      counter = strlen(display_secondLine);
      display_secondLine[counter] = DEGREE_SYMBOL;
      display_secondLine[++counter] = '\0';
      break;
      
    case PROGRAM_ITEM_BOILING:
      display_iterableCount = 2;
      strcpy(display_firstLine + strlen(display_firstLine), "BOIL");

      display_secondLine[0] = display_activeIterable[SCREEN_PROGRAM_EDIT] == 1 ? POINTER_SYMBOL : ' ';
      display_secondLine[1] = '\0';
      strcpy(display_secondLine + strlen(display_secondLine), formatMinutes(display_programEdit_B));
      strcpy(display_secondLine + strlen(display_secondLine), " AT 100");
      counter = strlen(display_secondLine);
      display_secondLine[counter] = DEGREE_SYMBOL;
      display_secondLine[++counter] = '\0';
      break;

    case PROGRAM_ITEM_REMINDER_TIME:
      display_iterableCount = 2;
      strcpy(display_firstLine + strlen(display_firstLine), "ALARM");

      strcpy(display_secondLine, "AFTER ");
      counter = strlen(display_secondLine);
      display_secondLine[counter] = display_activeIterable[SCREEN_PROGRAM_EDIT] == 1 ? POINTER_SYMBOL : ' ';
      display_secondLine[++counter] = '\0';
      strcpy(display_secondLine + strlen(display_secondLine), formatMinutes(display_programEdit_B));
      break;
      
    case PROGRAM_ITEM_REMINDER_TEMP:
      display_iterableCount = 2;
      strcpy(display_firstLine + strlen(display_firstLine), "ALARM");

      strcpy(display_secondLine, "ON ");
      counter = strlen(display_secondLine);
      display_secondLine[counter] = display_activeIterable[SCREEN_PROGRAM_EDIT] == 1 ? POINTER_SYMBOL : ' ';
      display_secondLine[++counter] = '\0';
      itoa(display_programEdit_B, display_secondLine + strlen(display_secondLine), 10);
      counter = strlen(display_secondLine);
      display_secondLine[counter] = DEGREE_SYMBOL;
      display_secondLine[++counter] = '\0';
      break;
  }
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
    buzzer_buttonShort();
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_MAIN);
  }
  else if (keyboard_upPressed && keyboard_shortPress) {
    buzzer_saved();
    keyboard_releaseKeys();
  }
  else if (keyboard_downPressed && keyboard_shortPress) {
    buzzer_deleted();
    keyboard_releaseKeys();
  }
  else if (keyboard_leftPressed && keyboard_shortPress) {

  }
  else if (keyboard_rightPressed && keyboard_shortPress) {

  }
}

void inline display_main_listeners() {
  if (keyboard_escapePressed && keyboard_shortPress) {
    buzzer_buttonShort();
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_DASHBOARD);
  }
  else if ((keyboard_upPressed || keyboard_downPressed) && keyboard_shortPress) {
    display_activeIterablePrevious[SCREEN_MAIN] = display_activeIterable[SCREEN_MAIN];
    keyboard_downPressed ? display_activeIterable[SCREEN_MAIN]++ : display_activeIterable[SCREEN_MAIN]--;
    if (display_activeIterable[SCREEN_MAIN] >= display_iterableCount) { display_activeIterable[SCREEN_MAIN] = 0; }
    else if (display_activeIterable[SCREEN_MAIN] < 0) { display_activeIterable[SCREEN_MAIN] = display_iterableCount - 1; }
    buzzer_buttonShort();
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_MAIN);
  }
  else if (display_activeIterable[SCREEN_MAIN] == SCREEN_ITEM_MAIN_SETTINGS && keyboard_enterPressed && keyboard_shortPress) {
    buzzer_buttonShort();
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_SETTINGS);
  }
  else if (display_activeIterable[SCREEN_MAIN] == SCREEN_ITEM_MAIN_PROGRAM && keyboard_enterPressed && keyboard_shortPress) {
    buzzer_buttonShort();
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_PROGRAM);
  }
}

void inline display_settings_listeners() {  
  if (keyboard_escapePressed && keyboard_shortPress) {
    buzzer_buttonShort();
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_MAIN);
  }
  else if ((keyboard_upPressed || keyboard_downPressed) && keyboard_shortPress) {
    display_activeIterablePrevious[SCREEN_SETTINGS] = display_activeIterable[SCREEN_SETTINGS];
    keyboard_downPressed ? display_activeIterable[SCREEN_SETTINGS]++ : display_activeIterable[SCREEN_SETTINGS]--;
    if (display_activeIterable[SCREEN_SETTINGS] >= display_iterableCount) { display_activeIterable[SCREEN_SETTINGS] = 0; }
    else if (display_activeIterable[SCREEN_SETTINGS] < 0) { display_activeIterable[SCREEN_SETTINGS] = display_iterableCount - 1; }
    buzzer_buttonShort();
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_SETTINGS);
  }
}

void inline display_program_listeners() {
  if (keyboard_escapePressed && keyboard_shortPress) {
    buzzer_buttonShort();
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_MAIN);
  }
  else if ((keyboard_upPressed || keyboard_downPressed) && keyboard_shortPress) {
    display_activeIterablePrevious[SCREEN_PROGRAM] = display_activeIterable[SCREEN_PROGRAM];
    keyboard_downPressed ? display_activeIterable[SCREEN_PROGRAM]++ : display_activeIterable[SCREEN_PROGRAM]--;
    if (display_activeIterable[SCREEN_PROGRAM] >= display_iterableCount) { display_activeIterable[SCREEN_PROGRAM] = 0; }
    else if (display_activeIterable[SCREEN_PROGRAM] < 0) { display_activeIterable[SCREEN_PROGRAM] = display_iterableCount - 1; }
    buzzer_buttonShort();
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_PROGRAM);
  }
  else if ((keyboard_upPressed || keyboard_downPressed) && keyboard_longPress) {
    bool isProgramItem = display_activeIterable[SCREEN_PROGRAM] <= brew_programLength - 1;
    if (isProgramItem) {
      bool upDirection = display_activeIterable[SCREEN_PROGRAM] < display_activeIterablePrevious[SCREEN_PROGRAM];

      int temp[3];
      byte currentIndex = display_activeIterable[SCREEN_PROGRAM];
      byte moveToIndex = upDirection ? currentIndex - 1 : currentIndex + 1;
      if (moveToIndex >= brew_programLength) { moveToIndex = 0; }
      else if (moveToIndex < 0) { moveToIndex = brew_programLength - 1; }

      memcpy(temp, brew_program[moveToIndex], sizeof(brew_program[moveToIndex]));
      memcpy(brew_program[moveToIndex], brew_program[currentIndex], sizeof(brew_program[currentIndex]));
      memcpy(brew_program[currentIndex], temp, sizeof(temp));

      display_activeIterable[SCREEN_PROGRAM] = moveToIndex;
      display_activeIterablePrevious[SCREEN_PROGRAM] = currentIndex;

      buzzer_saved();
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
    buzzer_buttonShort();
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_PROGRAM_EDIT);
  }
}

void inline display_program_edit_listeners() {
  if (keyboard_escapePressed && keyboard_shortPress) {
    buzzer_buttonShort();
    keyboard_releaseKeys();
    display_changeScreen(SCREEN_PROGRAM);
  }
  else if ((keyboard_upPressed || keyboard_downPressed) && keyboard_shortPress) {
    keyboard_downPressed ? display_activeIterable[SCREEN_PROGRAM_EDIT]++ : display_activeIterable[SCREEN_PROGRAM_EDIT]--;
    if (display_activeIterable[SCREEN_PROGRAM_EDIT] >= display_iterableCount) { display_activeIterable[SCREEN_PROGRAM_EDIT] = 0; }
    else if (display_activeIterable[SCREEN_PROGRAM_EDIT] < 0) { display_activeIterable[SCREEN_PROGRAM_EDIT] = display_iterableCount - 1; }
    buzzer_buttonShort();
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

    if (display_programEdit_B < 0) display_programEdit_B = 0;
    if (display_programEdit_C < 0) display_programEdit_C = 0;

    buzzer_buttonShort();
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

    buzzer_buttonShort();
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

    buzzer_saved();
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
      buzzer_deleted();
      keyboard_releaseKeys();
      display_changeScreen(SCREEN_PROGRAM);
    }
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