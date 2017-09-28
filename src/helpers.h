#include "Arduino.h"
#include "ArduinoJson.h"

#ifndef SERVERCHIP_ESP8266_HELPERS_H
#define SERVERCHIP_ESP8266_HELPERS_H

#define LEN(x)  (sizeof(x) / sizeof((x)[0]))

JsonObject &get_json_object(const char *payload, const size_t bufferSize);


#endif //SERVERCHIP_ESP8266_HELPERS_H
