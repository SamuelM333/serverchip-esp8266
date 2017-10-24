#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <Hash.h>
#include "SocketIoClient.h"

#ifndef SERVERCHIP_ESP8266_MAIN_H
#define SERVERCHIP_ESP8266_MAIN_H

// TODO OTA change
#define SSID "Red Deshabilitada"
#define PASS ""
#define WS_SERVER_IP "192.168.1.123"
#define WS_SERVER_PORT 5000

extern ESP8266WiFiMulti WiFiMulti;
extern SocketIoClient webSocket;
extern std::map<int, const char *> condition_port_map;
extern std::map<int, void (*)(void)> input_port_interrupt_handlers;
extern const uint8_t GPIO_PORTS[9];
extern const char *GPIO_PORTS_STR[9];
//extern char* sid;

#endif //SERVERCHIP_ESP8266_MAIN_H
