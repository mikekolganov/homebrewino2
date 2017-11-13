#include "constants.h"
#include "variables.h"

// byte buzzer_sequence[20];
// byte buzzer_sequenceIndex = 0;
// byte buzzer_sequenceLength = 0;
// unsigned int buzzer_lastToneTime = 0;

inline void buzzer_loop(unsigned long now) {
  if (buzzer_sequenceIndex < buzzer_sequenceLength) {
    if (buzzer_nextToneTime == 0 || buzzer_nextToneTime < now) {
      int frequency = buzzer_sequence[buzzer_sequenceIndex * 2];
      int duration  = buzzer_sequence[buzzer_sequenceIndex * 2 + 1];
      buzzer_nextToneTime = now + duration + 10;
      buzzer_sequenceIndex++;
      tone(PIN_BUZZER, frequency, duration);
    }
  }
}

inline void buzzer_reset() {
  buzzer_nextToneTime = 0;
  buzzer_sequenceIndex = 0;
}

inline void buzzer_buttonShort() {
  buzzer_reset();
  buzzer_sequenceLength = 1;
  buzzer_sequence[0] = 2093;
  buzzer_sequence[1] = 20;
}

inline void buzzer_saved() {
  buzzer_reset();
  buzzer_sequenceLength = 3;
  buzzer_sequence[0] = 1760;
  buzzer_sequence[1] = 50;
  buzzer_sequence[2] = 2093;
  buzzer_sequence[3] = 50;
  buzzer_sequence[4] = 2637;
  buzzer_sequence[5] = 50;
}

inline void buzzer_deleted() {
  buzzer_sequenceLength = 3;
  buzzer_sequence[0] = 2637;
  buzzer_sequence[1] = 50;
  buzzer_sequence[2] = 2093;
  buzzer_sequence[3] = 50;
  buzzer_sequence[4] = 1760;
  buzzer_sequence[5] = 50;
  buzzer_reset();
}

inline void buzzer_error() {
  buzzer_reset();
  buzzer_sequenceLength = 6;
  buzzer_sequence[0] = 1760;
  buzzer_sequence[1] = 30;
  buzzer_sequence[2] = 0;
  buzzer_sequence[3] = 30;
  buzzer_sequence[4] = 1760;
  buzzer_sequence[5] = 30;
  buzzer_sequence[6] = 0;
  buzzer_sequence[7] = 30;
  buzzer_sequence[8] = 1760;
  buzzer_sequence[9] = 30;
  buzzer_sequence[10] = 0;
  buzzer_sequence[11] = 30;
}