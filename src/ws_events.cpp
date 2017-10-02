#include "main.h"
#include "helpers.h"
#include "ws_events.h"


void handle_microchip_connected_ack(const char *payload, size_t length) {
	JsonObject &jsonObject = get_json_object(payload);
	JsonArray &tasks_json = jsonObject["tasks"];
	const char *sid = jsonObject["sid"];

	Serial.printf("sid: %s\n", sid);
	tasks_json.printTo(Serial);
	Serial.printf("\n");

	for (JsonArray::iterator tasks_it = tasks_json.begin(); tasks_it != tasks_json.end(); ++tasks_it) {
		setup_new_task(tasks_it->as<JsonObject &>());
	}

	Serial.println("setup_done");
	setup_done = true;

}

void handle_get_ports_status(const char *payload, size_t length) {
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

void emit_port_change(const char *task_id, uint8_t port_number, boolean new_state) {
	char payload[100];
	const size_t bufferSize = 2 * JSON_OBJECT_SIZE(2) + 80;
	StaticJsonBuffer<bufferSize> jsonBuffer;
	JsonObject &data = jsonBuffer.createObject();
	JsonObject &port = data.createNestedObject("port");
	data["task_id"] = task_id;
	port["port_number"] = port_number;
	port["state"] = new_state;

	data.printTo(payload);
	webSocket.emit("notify_port_change", payload);
}

void handle_run_task_request_microchip(const char *payload, size_t length) {
	char *response;
	JsonObject &output_port = get_json_object(payload)["output_port"];
	Serial.printf("output_port: %d, state: %s", output_port["number"], output_port["state"] ? "true" : "false");
	digitalWrite(output_port["number"], output_port["state"] ? HIGH : LOW);

	// TODO Response message?
	// TODO Emit ack
	webSocket.emit("run_task_request_microchip_ack", response);
}

void init_websocket_events() {
	webSocket.on("microchip_connected_ack", handle_microchip_connected_ack);
	webSocket.on("run_task_request_microchip", handle_run_task_request_microchip);
	webSocket.on("get_port_status", handle_get_ports_status);
}
