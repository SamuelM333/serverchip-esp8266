#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <Hash.h>
#include "SocketIoClient.h"

#ifndef SERVERCHIP_ESP8266_MAIN_H
#define SERVERCHIP_ESP8266_MAIN_H

// TODO OTA change
#define SSID "Red Deshabilitada"
//#define SSID "Murillos"
//#define PASS "d11748670"
#define WS_SERVER_IP "192.168.1.123"
#define WS_SERVER_PORT 5000

extern ESP8266WiFiMulti WiFiMulti;
extern SocketIoClient webSocket;

extern const int buttonPin;
extern int buttonState;
//extern char* sid;
extern LinkedList<Task> tasks;

#endif //SERVERCHIP_ESP8266_MAIN_H
