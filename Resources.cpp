#include "Resources.h"

/*class FontManager {
protected:
	std::map<std::string, ALLEGRO_FONT *> font_map;

public:*/
FontManager::FontManager() {
	loadFont("default", "Debug/mini_pixel-7.ttf", 20);
}

void FontManager::loadFont(std::string id, std::string name, Uint8 size) {
	ALLEGRO_FONT * font = al_load_ttf_font(name.c_str(), size, 0);
	if (!font) {
		printf("FAILED TO LOAD FONT!\0");
	} else {
		printf("Loaded Font %X", font);
		font_map[id] = font;
	}
}

ALLEGRO_FONT * FontManager::getFont(std::string id) {
	printf("ID: '%s'", id.c_str());
	if (font_map.find(id) != font_map.end()) {
		return font_map[id];
	}
	return NULL;
}

FontManager::~FontManager() {
	for (std::map<std::string, ALLEGRO_FONT *>::iterator it = font_map.begin(); it != font_map.end(); ++it) {
		al_destroy_font(it->second);
	}
}

ShaderManager::ShaderManager() {
	current_program = 0;
}

GLuint ShaderManager::loadShader(GLenum type, const std::string filePath, const std::string shaderName) {
	char * source;
	GLuint shader;
	GLint length, result;

	source = loadSourceUM(filePath);

	shader = glCreateShader(type);
	length = strlen(source);

	glShaderSource(shader, 1, (const char **)&source, &length);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		char * log;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		log = new char[length];
		glGetShaderInfoLog(shader, length, &result, log);

		printf("loadShader(): Unable to compile %s: %s\n", filePath.c_str(), log);

		delete[] log;
		delete[] source;
		
		glDeleteShader(shader);
		return 0;
	}

	delete[] source;

	ShaderObject * so = new ShaderObject();
	so->id = shader;

	shaders[shaderName] = so;

	return shader;
}

char * ShaderManager::loadSourceUM(const std::string filePath) {
	std::ifstream file(filePath.c_str(), std::ios::binary | std::ios::in);

	if (!file.is_open()) {
		return "";
	}

	/*file.seekg(0, std::ios::beg);
	std::ifstream::pos_type begin_pos = file.tellg();
	file.seekg(0, std::ios::end);
	int length = static_cast<int>(file.tellg() - begin_pos);
	file.seekg(0, std::ios::beg);

	char * result = new char[length];
	file.read(result, length);
	file.close();*/

	std::stringstream fdata;
	fdata << file.rdbuf();

	std::string sresult = fdata.str();

	char * result = new char[sresult.size() + 1];

	std::strcpy(result, sresult.c_str());

	result[sresult.size()] = '\0';

	return result;
}

GLuint ShaderManager::addProgram(const std::string name) {
	programs[name] = glCreateProgram();
	return programs[name];
}

void ShaderManager::storeUniform(const std::string program, std::string name) {
	uniforms[name] = glGetUniformLocation(programs[program], name.c_str());
}

void ShaderManager::setUniform3v(const std::string name, GLint length, GLfloat * vals) {
	if (uniforms.find(name) == uniforms.end()) {
		printf("Failed to set uniform for %s.\n", name.c_str());
		return;
	}

	glUniform3fv(uniforms[name], length, vals);
}

void ShaderManager::linkProgram(const std::string name) {
	//TODO Check program exists
	GLuint program = programs[name];
	GLint result;

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &result);

	if (result == GL_FALSE) {
		GLint length;
		char * log;

		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		log = new char[length];
		glGetProgramInfoLog(program, length, &result, log);

		printf("linkProgram(): Program linking failed: %s\n", log);

		delete[] log;

		glDeleteProgram(program);
		programs.erase(name);
	}
}

void ShaderManager::useProgram(const std::string name) {
	glUseProgram(programs[name]);
	current_program = programs[name];
}

void ShaderManager::shaderAttachFromFile(GLuint program, GLenum type, const std::string filePath, const std::string shaderName) {
	GLuint shader = loadShader(type, filePath, shaderName);

	if (shader != 0) {
		glAttachShader(program, shader);

		/*Delete the shader - it won't actually be destroyed until the program is*/
		glDeleteShader(shader);
	}
}

void ShaderManager::oneShot(const std::string name, const std::string vertexId, const std::string vertexFile, const std::string fragmentId, const std::string fragmentFile) {
	GLuint program = addProgram(name);
	shaderAttachFromFile(program, GL_VERTEX_SHADER, vertexFile, vertexId);
	shaderAttachFromFile(program, GL_FRAGMENT_SHADER, fragmentFile, fragmentId);
	linkProgram(name);
	//useProgram(name);
}

ShaderManager::~ShaderManager() {
	for (std::map<std::string, ShaderObject *>::iterator it = shaders.begin(); it != shaders.end(); ++it) {
		delete it->second;
	}

	for (std::map<std::string, GLuint>::iterator it = programs.begin(); it != programs.end(); ++it) {
		glDeleteProgram(it->second);
	}
}