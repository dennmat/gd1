#include "core.h"

Core::Core() {
	display = NULL;
	event_queue = NULL;
	timer = NULL;
	running = false;

	//Managers
	font_manager = NULL;
	shader_manager = NULL;

	dev_console = NULL;
	dev_info = NULL;
	show_dev_info = 1;

	test_chunk = NULL;
	test_chunk2 = NULL;
	test_chunk3 = NULL;
	test_chunk4 = NULL;
	mouse_dx = 0.0f;
	mouse_dy = 0.0f;
}

Uint8 Core::initialize() {
	for (int i = 0; i < 255; i++) {
		keyStates[i] = false;
	}

	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return 0;
	}

	if (!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize the keyboard!\n");
		return 0;
	}

	if (!al_install_mouse()) {
		fprintf(stderr, "failed to initialize the mouse!\n");
		return 0;
	}

	if (!setupDisplay()) {
		fprintf(stderr, "failed to create display!\n");
		return 0;
	}

	timer = al_create_timer(1.0f / 60.0f);
	if (!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return 0;
	}

	al_init_font_addon();
	al_init_ttf_addon();

	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		return 0;
	}

	//First After Allegro is setup
	initializeEvents();
	initializeManagers();

	//Then console stuff
	dev_console = new DevOverlay(&resource_context);
	dev_console->initialize();
	dev_console->addItem("dev_dash", &show_dev_info, ConsoleCommandEntry::GC_TYPE_UINT8);

	dev_info = new DevInfo(&resource_context);

	console_context.console = dev_console;
	console_context.dev_info = dev_info;

	running = true;

	test_chunk = new Chunk();
	//test_chunk2 = new Chunk();
	//test_chunk3 = new Chunk();
	//test_chunk4 = new Chunk();

	manager_test.add(0, 0, test_chunk);
	manager_test.add(1, 0, test_chunk);
	manager_test.add(0, 1, test_chunk);
	manager_test.add(1, 1, test_chunk);

	dev_info->addWatchVariable(ConsoleCommandEntry::GC_TYPE_INT, &manager_test.vertex_count, "Vertices");

	/*manager_test.add(-1, 1, test_chunk);
	manager_test.add(-1, 0, test_chunk);
	manager_test.add(-1, -1, test_chunk);
	manager_test.add(-1, -2, test_chunk);

	manager_test.add(0, -1, test_chunk);
	manager_test.add(0, -2, test_chunk);
	manager_test.add(1, -1, test_chunk);
	manager_test.add(1, -2, test_chunk);

	manager_test.add(2, 1, test_chunk);
	manager_test.add(2, 0, test_chunk);
	manager_test.add(2, -1, test_chunk);
	manager_test.add(2, -2, test_chunk);*/

	al_start_timer(timer);

	al_hide_mouse_cursor(display);
	coreLoop();
	al_show_mouse_cursor(display);

	return 1;
}

void Core::initializeManagers() {
	font_manager = new FontManager();

	resource_context.fonts = font_manager;
	resource_context.display_data = &display_data;

	shader_manager = new ShaderManager();
	camera_context.shaders = shader_manager;

	camera.initialize(&camera_context);
}

Uint8 Core::setupDisplay() {
	al_get_display_mode(al_get_num_display_modes() - 1, &display_data);

	al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_OPENGL_3_0 | ALLEGRO_FULLSCREEN_WINDOW);
	al_set_new_display_adapter(0);
	al_set_new_display_option(ALLEGRO_VSYNC, 2, ALLEGRO_SUGGEST);

	display = al_create_display(1920, 1080);//display_data.width, display_data.height);
	if (!display) {
		std::cout << display_data.width << ", " << display_data.height << "\n";
		std::cout << "DISPLAY FAIL " << al_get_errno();
		return 0;
	}

	display_data.width = al_get_display_width(display);
	display_data.height = al_get_display_height(display);

	//al_set_current_opengl_context(display);

	camera.setupProjection(display_data);
	
	return 1;
}

void Core::initializeEvents() {
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
}