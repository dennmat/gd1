#ifndef _DEV_UTILS_H
#define _DEV_UTILS_H

#include <stdio.h>

#include <allegro5\allegro.h>
#include <allegro5\allegro_opengl.h>

namespace devutils {
	void drawWireCube(float x, float y, float z, float size) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		float half_size = size/2.0f;
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_QUADS);
			//Top
			glVertex3f(x-half_size,y+half_size,z+half_size);
			glVertex3f(x+half_size,y+half_size,z+half_size);
			glVertex3f(x+half_size,y+half_size,z-half_size);
			glVertex3f(x-half_size,y+half_size,z-half_size);
			//Bottom
			glVertex3f(x-half_size,y-half_size,z+half_size);
			glVertex3f(x+half_size,y-half_size,z+half_size);
			glVertex3f(x+half_size,y-half_size,z-half_size);
			glVertex3f(x-half_size,y-half_size,z-half_size);
			//Left
			glVertex3f(x-half_size,y-half_size,z-half_size);
			glVertex3f(x-half_size,y-half_size,z+half_size);
			glVertex3f(x-half_size,y+half_size,z+half_size);
			glVertex3f(x-half_size,y+half_size,z-half_size);
			//Right
			glVertex3f(x+half_size,y-half_size,z-half_size);
			glVertex3f(x+half_size,y-half_size,z+half_size);
			glVertex3f(x+half_size,y+half_size,z+half_size);
			glVertex3f(x+half_size,y+half_size,z-half_size);
			//Front
			glVertex3f(x-half_size,y-half_size,z+half_size);
			glVertex3f(x+half_size,y-half_size,z+half_size);
			glVertex3f(x+half_size,y+half_size,z+half_size);
			glVertex3f(x-half_size,y+half_size,z+half_size);
			//Back
			glVertex3f(x-half_size,y-half_size,z-half_size);
			glVertex3f(x+half_size,y-half_size,z-half_size);
			glVertex3f(x+half_size,y+half_size,z-half_size);
			glVertex3f(x-half_size,y+half_size,z-half_size);
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
};

#endif