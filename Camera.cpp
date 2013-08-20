#include "Camera.h"

/*class Camera {
protected:
	Point focus_location;
	Point camera_location;
#endif*/

Camera::Camera() {
	yaw = 0.0f;
	//pitch = 0.0f;
	roll = 0.0f;
}

/*void Camera::gluPerspective(float fov, float aspect, float near_plane, float far_plane) {
	double left, right;
	double bottom, top;
	top = tan(fov*SG_PI/360.0)*near_plane;
	bottom = -top;
	left = aspect * bottom;
	right = aspect * top;
	
	glFrustum(left, right, bottom, top, near_plane, far_plane);
}*/

void Camera::initialize(CameraContext * cctxt) {
	context = cctxt;

	context->shaders->oneShot("default", "vertex", "shader.vp", "fragment", "sharder.fp");
	context->shaders->storeUniform("default", "cameraPosition");
	context->shaders->storeUniform("default", "lightPosition");
	context->shaders->storeUniform("default", "lightColor");
}

void Camera::setupProjection(ALLEGRO_DISPLAY_MODE _display_data) {
	display_data = _display_data;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluPerspective(45.0f, (GLdouble)display_data.width/(GLdouble)display_data.height, 0.01f, 500.0f);

	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	GLfloat light_position[] = {0, 0.75, -1.0, 0.0};

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);
}

void Camera::applyRotation(float a, Vector3f axis) {
	rotation.rotate(a, axis.x, axis.y, axis.z);
}

void Camera::changePitch(float a) {
	pitch.rotate(a, 1.0, 0.0, 0.0);
}

void Camera::preRender() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluPerspective(45.0f, (GLdouble)display_data.width/(GLdouble)display_data.height, 0.01f, 500.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	Matrix4f t;
	t.asTranslate(camera_location.x, camera_location.y, camera_location.z);

	Matrix4f projection_matrix;
	projection_matrix.asIdentity();

	projection_matrix = pitch.getConjugate().toMatrix() * rotation.toMatrix() * t * projection_matrix;
	
	Vector3f light_position(0, 0.75, -1.0, 1.0);
	//Vector3f new_light_position = projection_matrix * light_position;
	GLfloat gl_light_pos[] = {light_position.x, light_position.y, light_position.z, 0};
	glLightfv(GL_LIGHT0, GL_POSITION, gl_light_pos);

	float projection_result[16];// = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	projection_matrix.asColumnMajor(projection_result);
	glMultMatrixf(projection_result);
	
	context->shaders->useProgram("default");
	GLfloat camera_pos[] = {camera_location.x, camera_location.y, camera_location.z};
	GLfloat light_pos[] = {gl_light_pos[0], gl_light_pos[1], gl_light_pos[2]};
	GLfloat light_color[] = {0.2, 0.5, 0.9};
	context->shaders->setUniform3v("cameraPosition", 1, camera_pos);
	context->shaders->setUniform3v("lightPosition", 1, light_pos);
	context->shaders->setUniform3v("lightColor", 1, light_color);

	glPushMatrix();
}

void Camera::postRender() {
	glPopMatrix();
}

void Camera::moveTo(Vector3f location) {
	//cam_pos += rotation * Vector3f(0.4f, 0.0f, 0.0f);
	camera_location += rotation.getConjugate().toMatrix() * location;
}

void Camera::moveY(float inc) {
	camera_location.y += inc;
}

void Camera::lookAt(Vector3f location) {
}

Camera::~Camera() {
}