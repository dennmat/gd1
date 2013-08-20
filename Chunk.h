#ifndef _CHUNK_H
#define _CHUNK_H

#include "Math3d.h"
#include "sgtypes.h"

#include <octree.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_opengl.h>

#include <vector> 
#include <tuple>
#include <map>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

class Chunk;

typedef std::tuple<int, int> ChunkLocation;
typedef std::map<ChunkLocation, Chunk *> ChunkMap;
typedef std::map<ChunkLocation, Chunk *>::iterator ChunkMapIterator;

struct RenderedFaces {
	std::vector<GLfloat> px;
	std::vector<GLfloat> nx;
	std::vector<GLfloat> py;
	std::vector<GLfloat> ny;
	std::vector<GLfloat> pz;
	std::vector<GLfloat> nz;

	RenderedFaces();
};

class Chunk {
protected:
	 Octree<Uint8, 4> data;
	 RenderedFaces rendered_faces;

public:
	Chunk();

	enum {
		PX=0,
		NX,
		PY,
		NY,
		PZ,
		NZ
	};

	GLuint vboFaces[6];

	int vertex_count;

	bool isAir(Uint8 x, Uint8 y, Uint8 z);
	
	void renderFaces(bool firstCall=false);
	void addFace(Uint8 x, Uint8 y, Uint8 z, Uint8 face);

	void render();

	void serialize(std::ostream &out) const;
	void read(std::istream &in);

	~Chunk();
};

class ChunkManager {
protected:
	ChunkMap chunks;

public:
	ChunkManager();

	int vertex_count;

	void add(int x, int y, Chunk * c);
	Chunk * get(int x, int y);

	void render();

	~ChunkManager();
};

#endif