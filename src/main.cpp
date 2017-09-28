#include "main.h"
#include "ws_events.h"

using namespace std;

ESP8266WiFiMulti WiFiMulti;
SocketIoClient webSocket;


int buttonState = 0;
const int BUTTON_PIN = 5;
const int GPIO_PORTS[10] = {0, 2, 3, 4, 5, 10, 12, 13, 14, 15};
const char* GPIO_PORTS_STR[10] = {"0", "2", "3", "4", "5", "10", "12", "13", "14", "15"};


// TODO Comment every function

boolean debounceButton(boolean state) {
	boolean stateNow = digitalRead(BUTTON_PIN);
	if (state != stateNow) {
		delay(10); // TODO Remove delay
		stateNow = digitalRead(BUTTON_PIN);
	}
	return stateNow;
}

void setup() {
	Serial.begin(115200);
	Serial.setDebugOutput(true);

	// TODO Create "setup button task" function
	// initialize the pushbutton pin as an input:
	pinMode(BUTTON_PIN, INPUT);

	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);

	Serial.println();
	Serial.println();
	Serial.println();

	for (uint8_t t = 4; t > 0; t--) {
		Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
		Serial.flush();
		delay(1000);
	}
	// Connect to WiFi
	WiFiMulti.addAP(SSID);
//	WiFiMulti.addAP(SSID, PASS);
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
}

void loop() {
	// TODO Emit when a port changes state
//	if (sid) {
	if (debounceButton(buttonState) == LOW && buttonState == HIGH) {
		// Button is pressed
		buttonState = LOW;
		Serial.printf("LOW\n");
		webSocket.emit("test_ws", "{\"button\": \"kek\"}");
		digitalWrite(LED_BUILTIN, LOW);
	} else if (debounceButton(buttonState) == HIGH && buttonState == LOW) {
		// Button is unpressed / released
		buttonState = HIGH;
		Serial.printf("HIGH\n");

		digitalWrite(LED_BUILTIN, HIGH);
	}
	webSocket.loop();
//	}
}
