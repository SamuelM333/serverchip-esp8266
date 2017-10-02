#include "main.h"
#include "helpers.h"
#include "ws_events.h"


ESP8266WiFiMulti WiFiMulti;
SocketIoClient webSocket;
std::map<int, const char *> condition_port_map;

int buttonState = 0;
boolean setup_done = false;
//const uint8_t BUTTON_PIN = 5;
Port port_states[9];
const int GPIO_PORTS[9] = {2, 3, 4, 5, 10, 12, 13, 14, 15};
const char *GPIO_PORTS_STR[9] = {"2", "3", "4", "5", "10", "12", "13", "14", "15"};


// TODO Comment every function

void check_port_change() {
	for (int i = 0, LEN_GPIO_PORTS = LEN(GPIO_PORTS); i < LEN_GPIO_PORTS; i++) {
		boolean new_state = digitalRead(GPIO_PORTS[i]) ? false : true;
		if (port_states[i].state != new_state) {
			// State change
			const char *task_id = condition_port_map[port_states[i].port_number];
			Serial.printf("%d port changed from %s to %s\n", GPIO_PORTS[i],
			              port_states[i].state ? "true" : "false",
			              new_state ? "true" : "false"
			);
			emit_port_change(task_id, port_states[i].port_number, new_state);
			port_states[i].state = new_state;
//			return;
			break;
		}
	}
}

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

	// Emit microchip_connected
	char payload[50];
	sprintf(payload, "{\"ip\":\"%s\"}", WiFi.localIP().toString().c_str());
	webSocket.emit("microchip_connected", payload);

	Serial.println("Ending setup...");
	delay(500);

	// Init port_states
	for (int i = 0, LEN_GPIO_PORTS = LEN(GPIO_PORTS); i < LEN_GPIO_PORTS; i++) {
		port_states[i].port_number = GPIO_PORTS[i];
		port_states[i].state = digitalRead(GPIO_PORTS[i]) ? false : true;
	}
}

void loop() {
	webSocket.loop();
	runEvery(100) check_port_change(); // call this every 100ms
}
