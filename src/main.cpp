#include "main.h"
#include "helpers.h"
#include "ws_events.h"


ESP8266WiFiMulti WiFiMulti;
SocketIoClient webSocket;
std::map<int, const char *> condition_port_map;
std::map<int, void (*)(void)> input_port_interrupt_handlers;

// TODO Optimize this
const uint8_t GPIO_PORTS[9] = {2, 3, 4, 5, 10, 12, 13, 14, 15};
const char *GPIO_PORTS_STR[9] = {"2", "3", "4", "5", "10", "12", "13", "14", "15"};


// TODO Comment every function

void setup() {
	Serial.begin(115200);
	Serial.setDebugOutput(true);

	// TODO Create "setup button task" function

	Serial.println();
	Serial.println();
	Serial.println();

	for (uint8_t t = 4; t > 0; t--) {
		Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
		Serial.flush();
		delay(1000);
	}
	// Connect to WiFi
	WiFiMulti.addAP(SSID, PASS);
	Serial.printf("Connecting to %s\n", SSID);
	while (WiFiMulti.run() != WL_CONNECTED) {
		delay(100);
		Serial.printf(".");
	}
	Serial.printf("Connected to %s! Local IP: %s\n", SSID, WiFi.localIP().toString().c_str());

	// Add events
	init_websocket_events();

	// Connect to WS server
	webSocket.begin(WS_SERVER_IP, WS_SERVER_PORT);

	// Setup interrupt handlers for input ports
	input_port_interrupt_handlers[0] = handler_pin0;
	input_port_interrupt_handlers[2] = handler_pin2;
	input_port_interrupt_handlers[3] = handler_pin3;
	input_port_interrupt_handlers[4] = handler_pin4;
	input_port_interrupt_handlers[5] = handler_pin5;
	input_port_interrupt_handlers[10] = handler_pin10;
	input_port_interrupt_handlers[12] = handler_pin12;
	input_port_interrupt_handlers[13] = handler_pin13;
	input_port_interrupt_handlers[14] = handler_pin14;
	input_port_interrupt_handlers[15] = handler_pin15;

	// Emit microchip_connected
	char payload[50];
	sprintf(payload, R"({"ip":"%s"})", WiFi.localIP().toString().c_str());
	webSocket.emit("microchip_connected", payload);

	Serial.println("Ending setup...");
	delay(500);
}

void loop() { webSocket.loop(); }
