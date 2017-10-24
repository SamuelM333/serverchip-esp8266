#include "main.h"
#include "helpers.h"
#include "ws_events.h"

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
			pinMode(input_port_number, INPUT_PULLUP);
			detachInterrupt(input_port_number);
			attachInterrupt(input_port_number, input_port_interrupt_handlers[input_port_number], CHANGE);
		}
	}
}

void pin_state_change(uint8_t pin) {
	const char *task_id = condition_port_map[pin];
	if (task_id != nullptr) emit_port_change(task_id, pin);
}

void handler_pin0() { pin_state_change(0); }

void handler_pin2() { pin_state_change(2); }

void handler_pin3() { pin_state_change(3); }

void handler_pin4() { pin_state_change(4); }

void handler_pin5() { pin_state_change(5); }

void handler_pin10() { pin_state_change(10); }

void handler_pin12() { pin_state_change(12); }

void handler_pin13() { pin_state_change(13); }

void handler_pin14() { pin_state_change(14); }

void handler_pin15() { pin_state_change(15); }

boolean debounceButton(boolean state, uint8_t pin) {
	boolean stateNow = digitalRead(pin);
	if (state != stateNow) {
		delay(10); // TODO Remove delay
		stateNow = digitalRead(pin);
	}
	return stateNow;
}
