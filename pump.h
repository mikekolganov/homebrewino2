#include "constants.h"
#include "variables.h"
#include "relays.h"

void inline pump_loop(unsigned long now) {
  if (now - lastRun_loopPump < LOOP_THRESHOLD_PUMP) return;
  lastRun_loopPump = now;

  if (brew_status == BREW_STATUS_WORKING) {
    float delta = fabs(sensor_brewing1 - sensor_brewing2);
    if (delta > setting_pumpTempDelta) {
      relays_turn_on_pump_for(5 * 1000);
    }
  }
}