#ifndef UTILS_H
#define UTILS_H

inline void display_format_minutes(char *result, int minutes) {
  int h = minutes / 60;
  int m = minutes % 60;

  itoa(h, result + strlen(result), 10);
  strcpy(result + strlen(result), ":");
  if (m < 10) strcpy(result + strlen(result), "0");
  itoa(m, result + strlen(result), 10);
}

inline void display_format_seconds(char *result, int seconds) {
  int h = seconds / 3600;
  int m = seconds % 3600 / 60;
  int s = seconds % 3600 % 60;

  itoa(h, result + strlen(result), 10);
  strcpy(result + strlen(result), ":");
  if (m < 10) strcpy(result + strlen(result), "0");
  itoa(m, result + strlen(result), 10);
  strcpy(result + strlen(result), ":");
  if (s < 10) strcpy(result + strlen(result), "0");
  itoa(s, result + strlen(result), 10);
}

#endif