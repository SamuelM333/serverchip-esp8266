#include "main.h"
#include "helpers.h"
#include "ws_events.h"


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
	char* response;
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
