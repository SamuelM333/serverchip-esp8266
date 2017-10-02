#include <Arduino.h>
#include "ArduinoJson.h"


#ifndef SERVERCHIP_ESP8266_HELPERS_H
#define SERVERCHIP_ESP8266_HELPERS_H

#define LEN(x)  (sizeof(x) / sizeof((x)[0]))

// https://forum.arduino.cc/index.php?topic=124974.0
#define runEvery(t) for (static uint16_t _lasttime;\
                         (uint16_t)((uint16_t)millis() - _lasttime) >= (t);\
                         _lasttime += (t))


JsonObject &get_json_object(const char *payload, size_t bufferSize = 4000);

void setup_new_task(JsonObject &task);

boolean debounceButton(boolean state, uint8_t pin);

#endif //SERVERCHIP_ESP8266_HELPERS_H
