#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <Hash.h>
#include "SocketIoClient.h"

#ifndef SERVERCHIP_ESP8266_MAIN_H
#define SERVERCHIP_ESP8266_MAIN_H

// TODO OTA change
#define SSID "Red Deshabilitada"
//#define PASS ""
#define WS_SERVER_IP "192.168.1.123"
#define WS_SERVER_PORT 5000

extern ESP8266WiFiMulti WiFiMulti;
extern SocketIoClient webSocket;

extern const int GPIO_PORTS[10];
extern const char* GPIO_PORTS_STR[10];
extern const int BUTTON_PIN;
extern int buttonState;
//extern char* sid;

#endif //SERVERCHIP_ESP8266_MAIN_H
