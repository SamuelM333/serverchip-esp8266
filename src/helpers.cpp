#include "helpers.h"

JsonObject &get_json_object(const char *payload, const size_t bufferSize) {
	char *json = (char *) payload; // TODO Safe?
	Serial.printf("got message: %s\n", json);
	DynamicJsonBuffer jsonBuffer(bufferSize);
	JsonObject &jsonObject = jsonBuffer.parseObject(json);
	return jsonObject;
}
