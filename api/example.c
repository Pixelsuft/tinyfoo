#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "tinyfoo.h"

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;
	if (!tf_threaded_main(1))
		return 1;
	return 0;
}
