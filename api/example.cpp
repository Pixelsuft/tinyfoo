#include <iostream>
#include <cstdlib>
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
	TF_Cmd cmd;
	std::string inp;
	while (tf_get_init_state() == 1) {
		std::cout << "Enter command: ";
		std::cin >> inp;
		if (inp == "stop")
			cmd.idx = 0;
		else if (inp == "play")
			cmd.idx = 1;
		else if (inp == "pause")
			cmd.idx = 2;
		else if (inp == "next")
			cmd.idx = 3;
		else if (inp == "exit") {
			cmd.idx = 8;
			cmd.val = 0.f;
		}
		else {
			std::cout << "Unknown command\n";
			continue;
		}
		tf_thread_cmd(cmd);
	}
	return 0;
}
