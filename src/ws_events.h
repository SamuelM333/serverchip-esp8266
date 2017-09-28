#ifndef SERVERCHIP_ESP8266_WS_EVENTS_H
#define SERVERCHIP_ESP8266_WS_EVENTS_H

void handle_get_ports_status(const char *payload, size_t length);
void handle_run_task_request_microchip(const char *payload, size_t length);
void handle_microchip_connected_ack(const char *payload, size_t length);

#endif //SERVERCHIP_ESP8266_WS_EVENTS_H
