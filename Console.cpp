#include "Console.h"

BaseConsole::BaseConsole() {}

void BaseConsole::addItem(const std::string & name, void * pointer, Uint8 type) {
	ConsoleCommandEntry ce;
	ce.type = type;

	if (ce.type == ConsoleCommandEntry::GC_TYPE_FUNCTION) {
		ce.function = (ConsoleFunction)pointer;
	} else {
		ce.variable_pointer = pointer;
	}

	console_variables[name] = ce;
}

void BaseConsole::removeItem(const std::string & name) {
	if (console_variables.find(name) != console_variables.end()) {
		console_variables.erase(name);
	}
}

void BaseConsole::print(const std::string & text) {
	history.push_back(text);
}

void BaseConsole::executeCommand() {
	std::ostringstream out;
	std::string::size_type index = 0;
	std::vector<std::string> arguments;

	if (current_buffer.size() == 0) {
		return;
	}

	print(current_buffer);

	if (history.size() > BaseConsole::MaxHistory) {
		history.erase(history.begin());
	}
	
	std::stringstream ss(current_buffer);
	std::string item;
	while (std::getline(ss, item, ' ')) {
		if (item.size() > 0) {
			arguments.push_back(item);
		}
	}

	current_buffer = "";

	if (console_variables.find(arguments[0]) == console_variables.end()) {
		print(">> Invalid command.");
		return;
	}

	ConsoleCommandEntry ce = console_variables[arguments[0]];

	if (ce.type != ConsoleCommandEntry::GC_TYPE_FUNCTION && arguments.size() > 2) {
		print(">> Too many arguments.");
		return;
	}

	Uint8 num_args = (Uint8)arguments.size();
	
	out.str(">> ");
	switch (ce.type) {
		case ConsoleCommandEntry::GC_TYPE_CHAR: {
			if (num_args == 2) {
				*((char *)ce.variable_pointer) = (char)arguments[1][0];
			}

			out << arguments[0] << " = " << *((char *)ce.variable_pointer);
			break;
		}
		case ConsoleCommandEntry::GC_TYPE_UCHAR: {
			if (num_args == 2) {
				*((unsigned char *)ce.variable_pointer) = (unsigned char)arguments[1][0];
			}

			out << arguments[0] << " = " << *((unsigned char *)ce.variable_pointer);
			break;
		}
		case ConsoleCommandEntry::GC_TYPE_UINT: {
			if (num_args == 2) {
				*((unsigned int *)ce.variable_pointer) = (unsigned int)atoi(arguments[1].c_str());
			}

			out << arguments[0] << " = " << *((unsigned int *)ce.variable_pointer);
			break;
		}
		case ConsoleCommandEntry::GC_TYPE_INT: {
			if (num_args == 2) {
				*((int *)ce.variable_pointer) = (int) atoi(arguments[1].c_str());
			}

			out << arguments[0] << " = " << *((int *)ce.variable_pointer);
			break;
		}
		case ConsoleCommandEntry::GC_TYPE_UINT8: {
			if (num_args == 2) {
				*((Uint8 *)ce.variable_pointer) = (Uint8) atoi(arguments[1].c_str());
			}

			out << arguments[0] << " = " << (int)*((Uint8 *)ce.variable_pointer);
			break;
		}
		case ConsoleCommandEntry::GC_TYPE_INT8: {
			if (num_args == 2) {
				*((int8 *)ce.variable_pointer) = (int8) atoi(arguments[1].c_str());
			}

			out << arguments[0] << " = " << *((int8 *)ce.variable_pointer);
			break;
		}
		case ConsoleCommandEntry::GC_TYPE_UINT16: {
			if (num_args == 2) {
				*((Uint16 *)ce.variable_pointer) = (Uint16) atoi(arguments[1].c_str());
			}

			out << arguments[0] << " = " << *((Uint16 *)ce.variable_pointer);
			break;
		}
		case ConsoleCommandEntry::GC_TYPE_INT16: {
			if (num_args == 2) {
				*((int16 *)ce.variable_pointer) = (int16) atoi(arguments[1].c_str());
			}

			out << arguments[0] << " = " << *((int16 *)ce.variable_pointer);
			break;
		}
		case ConsoleCommandEntry::GC_TYPE_FLOAT: {
			if (num_args == 2) {
				*((float *)ce.variable_pointer) = (float) atof(arguments[1].c_str());
			}

			out << arguments[0] << " = " << *((float *)ce.variable_pointer);
			break;
		}
		case ConsoleCommandEntry::GC_TYPE_DOUBLE: {
			if (num_args == 2) {
				*((double *)ce.variable_pointer) = (double) atof(arguments[1].c_str());
			}

			out << arguments[0] << " = " << *((double *)ce.variable_pointer);
			break;
		}
		case ConsoleCommandEntry::GC_TYPE_STRING: {
			if (num_args == 2) {
				*((std::string *)ce.variable_pointer) = arguments[1];
			}

			out << arguments[0] << " = " << *((std::string *)ce.variable_pointer);
			break;
		}
		case ConsoleCommandEntry::GC_TYPE_FUNCTION: {
			ce.function(this, arguments);
			return;
			break;
		}
		default: {
			out << "Shouldn't have made it here :/";
			break;
		}
	}

	print(out.str());
}

void BaseConsole::input(unsigned char key, bool shift) {
	//Upper Case ASCII starts at: 65
	//Lower Case ASCII starts at: 97
	//0-9 ASCII starts at: 48 Allegro starts at: 27
	//Allegro A-Z = 1 - 26
	//Shift = L 215 R 216
	//Backspace = 63, Space = 75
	//Allegro dot (.) = 73 ascii is 46

	char parsed_key = 0;

	if (key <= 26) {
		parsed_key = key + ((shift)? 64 : 96);
	} else if (key > 26 && key <= 36) {
		parsed_key = key + 21;
	} else if (key == 75) {
		parsed_key = 32; //' '
	} else if (key == 61) {
		parsed_key = (shift)? 95 : 45;// _ -
	} else if (key == 62 && shift) {
		parsed_key = 43; //+
	} else if (key == 73) {
		parsed_key = 46;
	} else if (key == 74) { // /
		parsed_key = 47;
	}

	if (key == 63) { //backspace
		if (current_buffer.size() > 0) {
			current_buffer.pop_back();
		}
		return;
	}

	if (parsed_key > 0) {
		current_buffer.push_back((char)parsed_key);
	}
}

BaseConsole::~BaseConsole() {
}

DevOverlay::DevOverlay(ResourceContext * rc) {
	visible = false;
	font = rc->fonts->getFont("default");

	display_data = rc->display_data;
}

//CONSOLE UTILITIES
void console_util_window_info(BaseConsole * c, std::vector<std::string> args) {
	DevOverlay * console = (DevOverlay *)c;
	
	char buffer[40];
	sprintf(buffer, ">> Resolution: %iX%i\0", console->display_data->width, console->display_data->height);
	
	console->print(buffer);
}

void DevOverlay::initialize() {
	addItem("test_var", &test_console_vars, ConsoleCommandEntry::GC_TYPE_STRING);
	addItem("window", &console_util_window_info, ConsoleCommandEntry::GC_TYPE_FUNCTION);
}

void DevOverlay::render() {
	if (!visible) {
		return;
	}
	int y = 10;

	glColor4f(0.0, 0.0, 0.0, 0.4);
	glBegin(GL_QUADS);
		glVertex2f(800, 170);
		glVertex2f(5, 170);
		glVertex2f(5, 5);
		glVertex2f(800, 5);
	glEnd();

	glBegin(GL_QUADS);
		glVertex2f(800, 195);
		glVertex2f(5, 195);
		glVertex2f(5, 175);
		glVertex2f(800, 175);
	glEnd();

	std::vector<std::string>::iterator it;

	if (history.size() > 11) {
		it = history.end() - 11;
	} else {
		it = history.begin();
	}

	for (; it != history.end(); ++it) {
		al_draw_text(font, al_map_rgb(255, 255, 255), 10, y, ALLEGRO_ALIGN_LEFT, (*it).c_str());
		y += 14;
	}
	
	al_draw_text(font, al_map_rgb(255, 255, 255), 10, 180, ALLEGRO_ALIGN_LEFT, current_buffer.c_str());
}

DevOverlay::~DevOverlay() {
}

/*class DevInfo {
protected:
	int frameCount;
	int milliseconds;
	
public:*/
DevInfo::DevInfo(ResourceContext * rc) {
	font = rc->fonts->getFont("default");

	frame_count = 0;
	milliseconds = 0.0;
	last_tick = al_get_time();

	display_data = rc->display_data;
}

void DevInfo::initialize() {

}

void DevInfo::addWatchVariable(Uint8 type, void * variable, std::string label) {
	ConsoleCommandEntry ce;

	ce.type = type;
	ce.variable_pointer = variable;

	display_variables[label] = ce;
}

void DevInfo::tick() {
	double tick_delta = al_get_time() - last_tick;
	milliseconds += tick_delta;

	if (milliseconds >= 3.0) {
		std::stringstream out;
		out << (milliseconds/(float)frame_count) << " MS/F";
		mspf = out.str();
		out.str("");
		out << (int)(frame_count/milliseconds) << " FPS";
		fps = out.str();

		frame_count = 0;
		milliseconds = 0.0;
	}

	frame_count++;

	last_tick = al_get_time();
}

void DevInfo::render() {
	int top = (display_variables.size() * 20) + 130;

	glColor4f(0.0, 0.0, 0.0, 0.5);
	glBegin(GL_QUADS);
		glVertex2f(140, display_data->height - 70);
		glVertex2f(5, display_data->height - 70);
		glVertex2f(5, display_data->height - top);
		glVertex2f(140, display_data->height - top);
	glEnd();
	al_draw_text(font, al_map_rgb(255, 255, 255), 10, display_data->height - 100, ALLEGRO_ALIGN_LEFT, mspf.c_str());
	al_draw_text(font, al_map_rgb(255, 255, 255), 10, display_data->height - 120, ALLEGRO_ALIGN_LEFT, fps.c_str());

	char buffer[100];
	int y = 140;
	for (std::map<std::string, ConsoleCommandEntry>::iterator it = display_variables.begin(); it != display_variables.end(); ++it) {
		sprintf(buffer, "%d %s\0", *((int *)it->second.variable_pointer), it->first.c_str());
		al_draw_text(font, al_map_rgb(255,255,255), 10, display_data->height - y, ALLEGRO_ALIGN_LEFT, buffer);
		y -= 20;
	}
}

DevInfo::~DevInfo() {}