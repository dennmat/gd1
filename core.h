#ifndef __CORE_H
#define __CORE_H

//#include <GL/glew.h>

#include "sgtypes.h"
#include "Math3d.h"
#include "Camera.h"
#include "Chunk.h"
#include "Console.h"
#include "Resources.h"
#include "Context.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_opengl.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <stdio.h>

class Core {
protected:
	 ALLEGRO_DISPLAY * display;
	 ALLEGRO_DISPLAY_MODE display_data;
	 ALLEGRO_EVENT_QUEUE * event_queue;
	 ALLEGRO_TIMER * timer;
	 Camera camera;

	 Chunk * test_chunk;
	 Chunk * test_chunk2;
	 Chunk * test_chunk3;
	 Chunk * test_chunk4;
	 ChunkManager manager_test;

	 FontManager * font_manager;
	 ResourceContext resource_context;
	 
	 ShaderManager * shader_manager;
	 CameraContext camera_context;

	 DevOverlay * dev_console;
	 Uint8 show_dev_info;
	 DevInfo * dev_info;

	 GenericConsoleContext console_context;
	 //float rot;
	 //float roty;

	 bool keyStates[255];

	 float mouse_dx;
	 float mouse_dy;

	 bool running;

public:
	Core();

	Uint8 initialize();
	void initializeEvents();
	void initializeManagers();

	Uint8 setupDisplay();

	void render();
	
	void handleEvents();
	void onKeyUp(ALLEGRO_EVENT ev);
	void onKeyDown(ALLEGRO_EVENT ev);
	void onMouseMove(float dx, float dy);
	void update();

	void coreLoop();

	~Core();
};

#endif