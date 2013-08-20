#include "core.h"

void Core::coreLoop() {
	while(running) {
		handleEvents();
		render();
		dev_info->tick();
	}
}