#include "helpers.h"

JsonObject &get_json_object(const char *payload, size_t bufferSize) {

	// Default value for unknown sizes
	if (bufferSize == 0) {
		bufferSize = 4000;
	}

	char *json = (char *) payload; // TODO Safe?
	Serial.printf("got message: %s\n", json);
	DynamicJsonBuffer jsonBuffer(bufferSize);
	JsonObject &jsonObject = jsonBuffer.parseObject(json);
	return jsonObject;
}
