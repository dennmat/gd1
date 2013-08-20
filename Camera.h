#ifndef _CAMERA_H
#define _CAMERA_H

#include "sgtypes.h"
#include "Math3d.h"

#include "Context.h"

#include <allegro5\allegro.h>
#include <allegro5\allegro_opengl.h>

#include <GL/GLU.h>

#include <stdio.h>

class Camera {
protected:
	Vector3f focus_location;
	Vector3f camera_location;

	ALLEGRO_DISPLAY_MODE display_data;

	Quaternion rotation;
	Quaternion pitch;

	CameraContext * context;

public:
	float yaw;
	//float pitch;
	float roll;

	Camera();

	void initialize(CameraContext * cctxt);
	
	void setupProjection(ALLEGRO_DISPLAY_MODE _display_data);
	//void gluPerspective(float fov, float aspect, float near_plane, float far_plane);

	void preRender();
	void postRender();

	void moveTo(Vector3f location);
	void lookAt(Vector3f location);
	void moveY(float inc);

	void changePitch(float a);
	void applyRotation(float a, Vector3f axis);

	~Camera();
};

#endif