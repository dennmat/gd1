#ifndef _RESOURCES_H
#define _RESOURCES_H

#include "sgtypes.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_opengl.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <map>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>

#include <stdio.h>


class FontManager {
protected:
	std::map<std::string, ALLEGRO_FONT *> font_map;

public:
	FontManager();

	void loadFont(std::string id, std::string name, Uint8 size);
	ALLEGRO_FONT * getFont(std::string id);

	~FontManager();
};

struct ShaderObject {
	GLuint id;
};

class ShaderManager {
protected:
	std::map<std::string, ShaderObject *> shaders;
	std::map<std::string, GLuint> programs;
	std::map<std::string, GLuint> uniforms;

	GLuint current_program;

public:
	ShaderManager();

	GLuint addProgram(const std::string name);
	void linkProgram(const std::string name);
	void useProgram(const std::string name);
	
	void storeUniform(const std::string program, std::string name);
	void setUniform3v(const std::string name, GLint length, GLfloat * vals);

	char * loadSourceUM(const std::string filePath);
	GLuint loadShader(GLenum type, const std::string filePath, const std::string shaderName);

	void shaderAttachFromFile(GLuint program, GLenum type, const std::string filePath, const std::string shaderName);

	void oneShot(const std::string name, const std::string vertexId, const std::string vertexFile, const std::string fragmentId, const std::string fragmentFile);

	~ShaderManager();
};

#endif