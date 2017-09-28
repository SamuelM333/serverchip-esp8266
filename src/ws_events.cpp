#include "main.h"
#include "helpers.h"
#include "ws_events.h"


void handle_microchip_connected_ack(const char *payload, size_t length) {

	JsonObject &jsonObject = get_json_object(payload, 0);
	JsonArray &tasks_json = jsonObject["tasks"];
	const char *sid = jsonObject["sid"];

	Serial.printf("sid: %s\n", sid);
	tasks_json.printTo(Serial);
	Serial.printf("\n");

	for (JsonArray::iterator tasks_it = tasks_json.begin(); tasks_it != tasks_json.end(); ++tasks_it) {

		// Set output port pinMode
		uint8_t output_port_number = tasks_it->as<JsonObject &>()["output_port"]["number"];
		Serial.printf("output_port_number: %d\n", output_port_number);
		pinMode(output_port_number, OUTPUT);

		// Set input port pinMode from all tasks
		JsonArray &conditions = tasks_it->as<JsonObject &>()["conditions"];

		for (JsonArray::iterator conditions_it = conditions.begin();
		     conditions_it != conditions.end(); ++conditions_it) {

			conditions_it->as<JsonObject &>()["input_port"].printTo(Serial);
			Serial.printf("\n");

			if (conditions_it->as<JsonObject &>()["input_port"].success()) {
				uint8_t input_port_number = conditions_it->as<JsonObject &>()["input_port"]["number"];
				Serial.printf("input_port_number: %d\n", input_port_number);

				// TODO https://www.arduino.cc/en/Reference/AttachInterrupt
				pinMode(input_port_number, INPUT);
			}
		}
	}

}

void handle_get_ports_status(const char *payload, size_t length) {
	Serial.println("handle_get_ports_status");
	char response[430]; // Length of the response
	const size_t bufferSize = JSON_OBJECT_SIZE(11) + 100;
	StaticJsonBuffer<bufferSize> jsonBuffer;
	JsonObject &data = jsonBuffer.createObject();
	for (int i = 0, LEN_GPIO_PORTS = LEN(GPIO_PORTS); i < LEN_GPIO_PORTS; i++) {
		data[GPIO_PORTS_STR[i]] = digitalRead(GPIO_PORTS[i]) ? false : true;
	}

	data.printTo(response);

	webSocket.emit("get_port_status_response_server", response);
}

void init_websocket_events() {
	webSocket.on("microchip_connected_ack", handle_microchip_connected_ack);
	webSocket.on("run_task_request_microchip", handle_run_task_request_microchip);
	webSocket.on("get_port_status", handle_get_ports_status);
}
