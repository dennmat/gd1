#ifndef _CONTEXT_H
#define _CONTEXT_H

//#include "core.h"
//#include "Console.h"
#include "Resources.h"

#include <allegro5/allegro.h>

#include <map>

class GameContext {

};

class ResourceContext {
public:
	ALLEGRO_DISPLAY_MODE * display_data;
	FontManager * fonts;
};

class DevInfo; //Declared later
class DevOverlay; //Declared later

class GenericConsoleContext {
public:
	DevInfo * dev_info;
	DevOverlay * console;
};

class CameraContext {
public:
	ShaderManager * shaders;

};
#endif