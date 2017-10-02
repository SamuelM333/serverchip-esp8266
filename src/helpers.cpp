#include "main.h"
#include "helpers.h"

JsonObject &get_json_object(const char *payload, size_t bufferSize) {
	DynamicJsonBuffer jsonBuffer(bufferSize);
	JsonObject &jsonObject = jsonBuffer.parseObject(payload);
	return jsonObject;
}

void setup_new_task(JsonObject &task) {
	// Set output port pinMode
	const char *task_id = task["_id"];
	uint8_t output_port_number = task["output_port"]["number"];
	Serial.printf("output_port_number: %d\n", output_port_number);
	pinMode(output_port_number, OUTPUT);

	// Set input port pinMode for conditions
	JsonArray &conditions = task["conditions"];

	for (JsonArray::iterator conditions_iterator = conditions.begin();
	     conditions_iterator != conditions.end(); ++conditions_iterator) {

		conditions_iterator->as<JsonObject &>()["input_port"].printTo(Serial);
		Serial.printf("\n");

		if (conditions_iterator->as<JsonObject &>()["input_port"].success()) {
			uint8_t input_port_number = conditions_iterator->as<JsonObject &>()["input_port"]["number"];
			condition_port_map[input_port_number] = task_id;
			pinMode(input_port_number, INPUT);
		}
	}
}

boolean debounceButton(boolean state, uint8_t pin) {
	boolean stateNow = digitalRead(pin);
	if (state != stateNow) {
		delay(10); // TODO Remove delay
		stateNow = digitalRead(pin);
	}
	return stateNow;
}
