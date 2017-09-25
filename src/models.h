#include "LinkedList.h"

struct Port {
	int number;
	boolean state;
};

struct Task {
	const char *task_id;
	Port output_port;
	LinkedList<Port> input_ports;
};
