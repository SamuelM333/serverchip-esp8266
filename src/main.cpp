#include <stdlib.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <Hash.h>
//#include <string.h>

#include "ArduinoJson.h"
#include "SocketIoClient.h"
#include "LinkedList.h"
//#include "gpio.h"

using namespace std;

ESP8266WiFiMulti WiFiMulti;
SocketIoClient webSocket;

// TODO OTA change
#define SSID "Red Deshabilitada"
//#define SSID "Murillos"
//#define PASS "d11748670"
#define WS_SERVER_IP "192.168.1.123"
#define WS_SERVER_PORT 5000

struct Port {
	int number;
	boolean state;
};

struct Task {
	const char *task_id;
	Port output_port;
	LinkedList<Port> input_ports;
};

const int buttonPin = 5;
int buttonState = 0;
//char *sid = "";
LinkedList<Task> tasks;

// TODO Create function prototypes

// TODO Comment every function
JsonObject &get_json_object(const char *payload, const size_t bufferSize) {
	char *json = (char *) payload; // TODO Safe?
	Serial.printf("got message: %s\n", json);
	DynamicJsonBuffer jsonBuffer(bufferSize);
	JsonObject &jsonObject = jsonBuffer.parseObject(json);
	return jsonObject;
}

void handle_get_port_status(const char *payload, size_t length) {
	const size_t bufferSize = JSON_OBJECT_SIZE(2) + 60;
	JsonObject &jsonObject = get_json_object(payload, bufferSize);
	int port_number = jsonObject["port_number"];
	boolean value = digitalRead((uint8_t) port_number);
	Serial.printf("Port number: %d, value: %d", port_number, value);
	char* response;
	sprintf(response, "{\"port\": %d, \"value\": %s}", port_number, value ? "true" : "false");
	webSocket.emit("port_status_response", response);
}

void handle_run_task_request_microchip(const char *payload, size_t length) {
	const size_t bufferSize = JSON_OBJECT_SIZE(1) + 40;
	JsonObject &jsonObject = get_json_object(payload, bufferSize);
	const char *task_id = jsonObject["task_id"];
	// Look for task inside the task array
	boolean found = false;
	for (int i = 0, task_list_size = tasks.size(); i < task_list_size; i++) {
		Task task;
		task = tasks.get(i);
		if (task.task_id == task_id) {
			// TODO Run task
			found = true;
			break;
		}
	}

	char* response;
	sprintf(response, "{\"task_id\": \"%s\", \"task_found\": %s}", task_id, found ? "true" : "false");
	// Emit ack
	webSocket.emit("run_task_request_microchip_ack", response);
	// TODO Log here
}

void handle_microchip_connected_ack(const char *payload, size_t length) {

	const size_t bufferSize =
		3 * JSON_ARRAY_SIZE(1) + 4 * JSON_OBJECT_SIZE(1) + 4 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(8) + 360;

	JsonObject &jsonObject = get_json_object(payload, bufferSize);
	JsonArray &tasks_json = jsonObject["tasks"];

	// Store sid from server
	const char *sid = jsonObject["sid"];

	Serial.printf("sid: %s\n", sid);

	// Store tasks on memory
	tasks = LinkedList<Task>();
	for (JsonArray::iterator tasks_it = tasks_json.begin(); tasks_it != tasks_json.end(); ++tasks_it) {

		// Parse task
		Task task;
		task.task_id = tasks_it->as<JsonObject &>()["_id"]["$oid"];
		task.output_port.number = tasks_it->as<JsonObject &>()["output_port"]["number"];
		task.output_port.state = tasks_it->as<JsonObject &>()["output_port"]["state"];
		// Set pinMode to the task's output port
//		pinMode((uint8_t) task.output_port.number, OUTPUT);

		// Parse task's input ports
		task.input_ports = LinkedList<Port>();

		JsonArray &conditions = tasks_it->as<JsonObject &>()["conditions"];

		for (JsonArray::iterator conditions_it = conditions.begin();
			 conditions_it != conditions.end(); ++conditions_it) {

			conditions_it->as<JsonObject &>()["input_port"].printTo(Serial);
			if (conditions_it->as<JsonObject &>()["input_port"].success()) {
				Port input_port;
				input_port.number = conditions_it->as<JsonObject &>()["input_port"]["number"];
				input_port.state = conditions_it->as<JsonObject &>()["input_port"]["state"];
				task.input_ports.add(input_port);
				// Set pinMode to the condition's input port
				// https://www.arduino.cc/en/Reference/AttachInterrupt
//				pinMode((uint8_t) input_port.number, INPUT);
			}
		}

		tasks.add(task);
	}
}

boolean debounceButton(boolean state) {
	boolean stateNow = digitalRead(buttonPin);
	if (state != stateNow) {
		delay(10); // TODO Remove delay
		stateNow = digitalRead(buttonPin);
	}
	return stateNow;
}

void setup() {
	Serial.begin(115200);
	Serial.setDebugOutput(true);
	pinMode(LED_BUILTIN, OUTPUT);

	// TODO Create "setup button task" function
	// initialize the pushbutton pin as an input:
	pinMode(buttonPin, INPUT);

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
	webSocket.on("microchip_connected_ack", handle_microchip_connected_ack);
	webSocket.on("run_task_request_microchip", handle_run_task_request_microchip);
	webSocket.on("get_port_status_request", handle_get_port_status);

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
