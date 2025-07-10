#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "tinyfoo.h"

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;
	if (!tf_threaded_main(0))
		return 1;
	while (tf_get_init_state() != 1) {
		if (tf_get_init_state() < 0)
			return 1;
		tf_time_delay(200);
	}
	while (tf_get_init_state() == 1) {
		tf_time_delay(200);
	}
	return 0;
}
