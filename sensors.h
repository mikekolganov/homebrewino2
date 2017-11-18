#include "constants.h"
#include "variables.h"

inline void sensors_loop(unsigned long now) {
  if (now - lastRun_loopSensors < LOOP_THRESHOLD_SENSORS) return;
  lastRun_loopSensors = now;

  brewing_sensors.requestTemperatures();
  sensor_brewing1 = brewing_sensors.getTempCByIndex(1);
  sensor_brewing2 = brewing_sensors.getTempCByIndex(0);

  fan_sensor.requestTemperatures();
  sensor_fan = fan_sensor.getTempCByIndex(0);
}