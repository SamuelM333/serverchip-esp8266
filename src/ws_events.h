#ifndef SERVERCHIP_ESP8266_WS_EVENTS_H
#define SERVERCHIP_ESP8266_WS_EVENTS_H

void handle_microchip_connected_ack(const char *payload, size_t length);

void handle_get_ports_status(const char *payload, size_t length);

void emit_port_change(const char *task_id, uint8_t port_number);

void handle_run_task_request_microchip(const char *payload, size_t length);

void init_websocket_events();

#endif //SERVERCHIP_ESP8266_WS_EVENTS_H
