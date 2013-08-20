#include "core.h"
#include "DevUtils.h"

void Core::render() {
	camera.preRender();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//glRotatef(rot*5.0, 1, 0, 0);
	glPushMatrix();
	//glRotatef(roty*5.0, 0, 1, 0);
	/*glBegin(GL_QUADS);
	glColor3f(   1.0,  1.0, 1.0 );
	glVertex3f(  0.5, -0.5, 0.5 );
	glVertex3f(  0.5,  0.5, 0.5 );
	glVertex3f( -0.5,  0.5, 0.5 );
	glVertex3f( -0.5, -0.5, 0.5 );
	glEnd();
 
	// Purple side - RIGHT
	glBegin(GL_QUADS);
	glColor3f(  1.0,  0.0,  1.0 );
	glVertex3f( 0.5, -0.5, -0.5 );
	glVertex3f( 0.5,  0.5, -0.5 );
	glVertex3f( 0.5,  0.5,  0.5 );
	glVertex3f( 0.5, -0.5,  0.5 );
	glEnd();
 
	// Green side - LEFT
	glBegin(GL_QUADS);
	glColor3f(   0.0,  1.0,  0.1 );
	glVertex3f( -0.5, -0.5,  0.5 );
	glVertex3f( -0.5,  0.5,  0.5 );
	glVertex3f( -0.5,  0.5, -0.5 );
	glVertex3f( -0.5, -0.5, -0.5 );
	glEnd();
 
	// Blue side - TOP
	glBegin(GL_QUADS);
	glColor3f(   0.0,  0.0,  1.0 );
	glVertex3f(  0.5,  0.5,  0.5 );
	glVertex3f(  0.5,  0.5, -0.5 );
	glVertex3f( -0.5,  0.5, -0.5 );
	glVertex3f( -0.5,  0.5,  0.5 );
	glEnd();
 
	// Red side - BOTTOM
	glBegin(GL_QUADS);
	glColor3f(   1.0,  0.0,  0.1 );
	glVertex3f(  0.5, -0.5, -0.5 );
	glVertex3f(  0.5, -0.5,  0.5 );
	glVertex3f( -0.5, -0.5,  0.5 );
	glVertex3f( -0.5, -0.5, -0.5 );
	glEnd();*/
	//test_chunk->render();
	manager_test.render();

	devutils::drawWireCube(0.5, 0.5, -0.5, 0.1);

	glPopMatrix();
	camera.postRender();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, display_data.width, display_data.height, 0, -1, 1);
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);

	glUseProgram(0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	dev_console->render();
	if (show_dev_info) {
		dev_info->render();
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);

	al_flip_display();
}