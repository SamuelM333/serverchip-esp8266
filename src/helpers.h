#include <Arduino.h>
#include "ArduinoJson.h"


#ifndef SERVERCHIP_ESP8266_HELPERS_H
#define SERVERCHIP_ESP8266_HELPERS_H

#define LEN(x)  (sizeof(x) / sizeof((x)[0]))

JsonObject &get_json_object(const char *payload, size_t bufferSize = 4000);

void setup_new_task(JsonObject &task);

void pin_state_change(uint8_t pin);

void handler_pin0();

void handler_pin2();

void handler_pin3();

void handler_pin4();

void handler_pin5();

void handler_pin10();

void handler_pin12();

void handler_pin13();

void handler_pin14();

void handler_pin15();

boolean debounceButton(boolean state, uint8_t pin);

#endif //SERVERCHIP_ESP8266_HELPERS_H
