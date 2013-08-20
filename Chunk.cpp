#include "Chunk.h"

RenderedFaces::RenderedFaces() : nx(std::vector<GLfloat>()), px(std::vector<GLfloat>()),
								 ny(std::vector<GLfloat>()), py(std::vector<GLfloat>()),
								 nz(std::vector<GLfloat>()), pz(std::vector<GLfloat>())
{}

Chunk::Chunk() : data(256, 254), rendered_faces(RenderedFaces()), vertex_count(0) { 
	for (int z = 0; z < 254; z++) { //Make a pyramid, stupid things
		for (int x = 0; x < 254; x++) {
			int y = (x < z)? x : z;
			if (y == 0) {
				data.set(x, y, z, (Uint8)1);
			}

			if (x >= 127 && y >= 127) {
				if (x >= z) {
					y = 127 - (x - 127);
				} else if (x < z) {
					y = 127 - (z - 127);
				}
			}
			if (z < 127 && x >= 127) {
				y = 127 - (x - 127);
				if (y > z) {
					y = z;
				}
			}
			if (x < 127 && z >= 127) {
				y = 127 - (z - 127);
				if (y > x) {
					y = x;
				}
			}

			data.set(x, y, z, (Uint8)1);//+(rand() % 100)
		}
	}

	data(0, 0, 0) = (Uint8)1; 

	glGenBuffers(6, vboFaces);
	
	renderFaces(true);
}

bool Chunk::isAir(Uint8 x, Uint8 y, Uint8 z) {
	if (x < 0  || x > 255 || y < 0 || y > 255 || z < 0 || z > 255) {
		return false;
	}
	
	Uint8 check = data.at(x, y, z);

	return (check == (Uint8)254);
}

void Chunk::addFace(Uint8 x, Uint8 y, Uint8 z, Uint8 face) {
	GLfloat unit_size = 1.0f/50.0;
	GLfloat half_unit = unit_size/2.0f;

	GLfloat _x = x * unit_size;
	GLfloat _y = y * unit_size;
	GLfloat _z = -(z * unit_size) - 2;

	vertex_count += 6;

	switch(face) {
		case Chunk::NZ: {
			_x -= half_unit;
			_y -= half_unit;
			_z += half_unit;

			GLfloat vertices[] = { //Forward Facing
				_x, _y, _z,
				_x + unit_size, _y, _z,
				_x, _y + unit_size, _z,
				_x, _y + unit_size, _z,
				_x + unit_size, _y, _z,
				_x + unit_size, _y + unit_size, _z
			};
			for (int i = 0; i < 18; i++) {
				rendered_faces.nz.push_back(vertices[i]);
			}
			break;
		}
		case Chunk::PZ: { //Back Facing
			_x += half_unit;
			_y -= half_unit;
			_z -= half_unit;
			GLfloat vertices[] = {
				_x, _y, _z,
				_x - unit_size, _y, _z,
				_x - unit_size, _y + unit_size, _z,
				_x, _y, _z,
				_x - unit_size, _y + unit_size, _z,
				_x, _y + unit_size, _z
			};
			for (int i = 0; i < 18; i++) {
				rendered_faces.pz.push_back(vertices[i]);
			}
			break;
		}
		case Chunk::PY: { //Up facing
			_z += half_unit;
			_x += half_unit;
			_y += half_unit;
			GLfloat vertices[] = {
				_x, _y, _z,
				_x - unit_size, _y, _z - unit_size,
				_x - unit_size, _y, _z,
				_x, _y, _z,
				_x, _y, _z - unit_size,
				_x - unit_size, _y, _z - unit_size
			};
			for (int i = 0; i < 18; i++) {
				rendered_faces.py.push_back(vertices[i]);
			}
			break;
		}
		case Chunk::NY: { //Down facing
			_z += half_unit;
			_x += half_unit;
			_y -= half_unit;
			GLfloat vertices[] = {
				_x, _y, _z,
				_x - unit_size, _y, _z,
				_x - unit_size, _y, _z - unit_size,
				_x, _y, _z,
				_x - unit_size, _y, _z - unit_size,
				_x, _y, _z - unit_size
			};
			for (int i = 0; i < 18; i++) {
				rendered_faces.ny.push_back(vertices[i]);
			}
			break;
		}
		case Chunk::PX: { //Right Facing
			_y -= half_unit;
			_z -= half_unit;
			_x += half_unit;
			GLfloat vertices[] = {
				_x, _y, _z, 
				_x, _y + unit_size, _z,
				_x, _y + unit_size, _z + unit_size,
				_x, _y, _z,
				_x, _y + unit_size, _z + unit_size,
				_x, _y, _z + unit_size
			};
			for (int i = 0; i < 18; i++) {
				rendered_faces.px.push_back(vertices[i]);
			}
			break;
		}
		case Chunk::NX: { //Left Facing
			_y -= half_unit;
			_z += half_unit;
			_x -= half_unit;
			GLfloat vertices[] = {
				_x, _y, _z,
				_x, _y + unit_size, _z - unit_size,
				_x, _y, _z - unit_size,
				_x, _y, _z,
				_x, _y + unit_size, _z,
				_x, _y + unit_size, _z - unit_size
			};
			for (int i = 0; i < 18; i++) {
				rendered_faces.nx.push_back(vertices[i]);
			}
			break;
		}
	}
}

void Chunk::renderFaces(bool firstCall) {
	for (Uint8 z = 0; z < 255; z++) {
		for (Uint8 y = 0; y < 255; y++)  {
			for (Uint8 x = 0; x < 255; x++) {
				if (isAir(x, y, z)) {
					continue;
				}
				
				if (isAir(x, y+1, z)) { //Up
					addFace(x, y, z, Chunk::PY);
				}
				if (isAir(x, y-1, z)) { //Down
					addFace(x, y, z, Chunk::NY);
				}
				if (isAir(x-1, y, z)) { //Left
					addFace(x, y, z, Chunk::NX);
				}
				if (isAir(x+1, y, z)) { //Right
					addFace(x, y, z, Chunk::PX);
				}
				if (isAir(x, y, z+1)) { //Forward
					addFace(x, y, z, Chunk::PZ);
				}
				if (isAir(x, y, z-1)) { //Backwards
					addFace(x, y, z, Chunk::NZ);
				}
			}
		}
	}


	glBindBuffer(GL_ARRAY_BUFFER, vboFaces[Chunk::PY]);
	glBufferData(GL_ARRAY_BUFFER, rendered_faces.py.size() * sizeof(GLfloat), rendered_faces.py.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, vboFaces[Chunk::PX]);
	glBufferData(GL_ARRAY_BUFFER, rendered_faces.px.size() * sizeof(GLfloat), rendered_faces.px.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, vboFaces[Chunk::PZ]);
	glBufferData(GL_ARRAY_BUFFER, rendered_faces.pz.size() * sizeof(GLfloat), rendered_faces.pz.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, vboFaces[Chunk::NY]);
	glBufferData(GL_ARRAY_BUFFER, rendered_faces.ny.size() * sizeof(GLfloat), rendered_faces.ny.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, vboFaces[Chunk::NX]);
	glBufferData(GL_ARRAY_BUFFER, rendered_faces.nx.size() * sizeof(GLfloat), rendered_faces.nx.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, vboFaces[Chunk::NZ]);
	glBufferData(GL_ARRAY_BUFFER, rendered_faces.nz.size() * sizeof(GLfloat), rendered_faces.nz.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Chunk::render() {
	glColor3f(0.0, 0.2, 1.0);
	glDisableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vboFaces[Chunk::PY]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, rendered_faces.py.size()/3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	glColor3f(0.0, 1.0, 0.0);
	glDisableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vboFaces[Chunk::PX]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, rendered_faces.px.size()/3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	glColor3f(0.0, 0.0, 1.0);
	glDisableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vboFaces[Chunk::PZ]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, rendered_faces.pz.size()/3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	glColor3f(1.0, 0.0, 0.0);
	glDisableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vboFaces[Chunk::NY]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, rendered_faces.ny.size()/3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	glColor3f(1.0, 0.0, 1.0);
	glDisableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vboFaces[Chunk::NX]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, rendered_faces.nx.size()/3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	glColor3f(1.0, 1.0, 0.0);
	glDisableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vboFaces[Chunk::NZ]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, rendered_faces.nz.size()/3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Chunk::serialize(std::ostream &out) const {
	data.writeBinary(out);
}

void Chunk::read(std::istream &in) {
	data.readBinary(in);
}

Chunk::~Chunk() {
	glDeleteBuffers(6, vboFaces);
}

ChunkManager::ChunkManager() : vertex_count(0) {}

void ChunkManager::add(int x, int y, Chunk * c) {
	ChunkLocation location(x, y);
	
	vertex_count += c->vertex_count;

	chunks[location] = c;
}

Chunk * ChunkManager::get(int x, int y) {
	ChunkLocation location(x, y);
	ChunkMapIterator it = chunks.find(location);

	if (it == chunks.end()) {
		return NULL;
	}

	return it->second;
}

void ChunkManager::render() {
	float chunk_size = (1.0f/50.0f)*254;
	Matrix4f t;
	GLfloat m2gl[16];
	t.asIdentity();
	for (ChunkMapIterator it = chunks.begin(); it != chunks.end(); ++it) {
		glPushMatrix();
			t.asTranslate(std::get<0>(it->first)*chunk_size, 0, std::get<1>(it->first)*chunk_size);
			t.asColumnMajor(m2gl);
			glMultMatrixf(m2gl);
			it->second->render();
		glPopMatrix();
	}
}

ChunkManager::~ChunkManager() {}