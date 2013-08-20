#include "core.h"

Core::~Core() {
	delete test_chunk;
	//delete test_chunk2;
	//delete test_chunk3;
	//delete test_chunk4;
	
	//Destroy Utils
	delete dev_console;
	delete dev_info;

	//Destroy Managers
	delete font_manager;
	delete shader_manager;
	
	if (event_queue) al_destroy_event_queue(event_queue);
	if (display) al_destroy_display(display);
	al_shutdown_font_addon();
	al_shutdown_ttf_addon();
	al_uninstall_keyboard();
	al_uninstall_mouse();
	if (timer) al_destroy_timer(timer);
}