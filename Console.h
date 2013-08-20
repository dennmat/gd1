#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "sgtypes.h"
#include "Context.h"

#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <stdio.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_opengl.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <stdlib.h>

class BaseConsole;

typedef void (*ConsoleFunction)(BaseConsole *, std::vector<std::string>);

struct ConsoleCommandEntry {
	enum {
		GC_TYPE_CHAR=0,
		GC_TYPE_UCHAR,
		GC_TYPE_UINT,
		GC_TYPE_INT,
		GC_TYPE_UINT8,
		GC_TYPE_INT8,
		GC_TYPE_UINT16,
		GC_TYPE_INT16,
		GC_TYPE_STRING,
		GC_TYPE_FLOAT,
		GC_TYPE_DOUBLE,
		GC_TYPE_FUNCTION
	};

	Uint8 type;

	union {
		void * variable_pointer;
		ConsoleFunction function;
	};
};

class BaseConsole {
protected:
	std::string current_buffer;

private:
	std::map<std::string, ConsoleCommandEntry> console_variables;

public:
	std::vector<std::string> history;
	const static int MaxHistory = 100;

	BaseConsole();

	void addItem(const std::string & name, void * pointer, Uint8 type);
	void removeItem(const std::string & name);

	void print(const std::string & text);

	void input(unsigned char key, bool shift=false);
	
	void executeCommand();

	virtual void render() = 0;

	virtual ~BaseConsole();
};

class DevOverlay : public BaseConsole {
protected:
	ALLEGRO_FONT * font;
	std::string test_console_vars;

public:
	ALLEGRO_DISPLAY_MODE * display_data;
	bool visible;

	DevOverlay(ResourceContext * rc);

	void initialize();
	void render();

	~DevOverlay();
};

class DevInfo {
protected:
	ALLEGRO_FONT * font;
	ALLEGRO_DISPLAY_MODE * display_data;

	int frame_count;
	std::string fps;
	std::string mspf;
	double milliseconds;
	double last_tick;

	std::map<std::string, ConsoleCommandEntry> display_variables;
	
public:
	DevInfo(ResourceContext * rc);
	
	void initialize();

	void addWatchVariable(Uint8 type, void * variable, std::string label);

	void tick();
	void render();

	~DevInfo();
};

#endif