#include "core.h"

void Core::handleEvents() {
	ALLEGRO_EVENT ev;
	int screen_w = al_get_display_width(display)/2.0f;
	int screen_h = al_get_display_height(display)/2.0f;

	while(al_get_next_event(event_queue, &ev)) {
		switch (ev.type) {
			case ALLEGRO_EVENT_KEY_UP: {
				if (ev.keyboard.keycode < 255) {
					keyStates[ev.keyboard.keycode] = false;
				}
				onKeyUp(ev);
				break;
			}
			case ALLEGRO_EVENT_KEY_DOWN: {
				if (ev.keyboard.keycode < 255) {
					keyStates[ev.keyboard.keycode] = true;
				}
				onKeyDown(ev);
				break;
			}
			case ALLEGRO_EVENT_MOUSE_AXES: {
				//printf("(%d, %d) == (%d, %d) == %d, %d\n", ev.mouse.x, ev.mouse.y, screen_w, screen_h, al_get_display_width(display), al_get_display_height(display));
				mouse_dx = ev.mouse.x - screen_w;//-8);
				mouse_dy = ev.mouse.y - screen_h;//-30);
				onMouseMove(mouse_dx, mouse_dy);
				break;
			}
			case ALLEGRO_EVENT_TIMER: {
				update();
				break;
			}
		}
	}
	
	al_set_mouse_xy(display, screen_w, screen_h);
}

void Core::update() {
	if (dev_console->visible) {
		return;
	}

	if (keyStates[ALLEGRO_KEY_W]) {
		camera.moveTo(Vector3f(0, 0, 0.01f));
	}
	
	if (keyStates[ALLEGRO_KEY_S]) {
		camera.moveTo(Vector3f(0, 0, -0.01f));
	}
	
	if (keyStates[ALLEGRO_KEY_A]) {
		camera.moveTo(Vector3f(0.01f, 0, 0));
	}
	
	if (keyStates[ALLEGRO_KEY_D]) {
		camera.moveTo(Vector3f(-0.01f, 0, 0));
	}
	
	if (keyStates[ALLEGRO_KEY_Q]) {
		camera.moveY(0.01f);
	}

	if (keyStates[ALLEGRO_KEY_E]) {
		camera.moveY(-0.01f);
	}
}

void Core::onMouseMove(float dx, float dy) {
	if (dx != 0) {
		camera.applyRotation(dx*(0.0005f), Vector3f(0, 1, 0));
	}
	if (dy != 0) {
		//printf("%f\n", dy);
		camera.changePitch(dy*(0.0005f));
	}
}

void Core::onKeyUp(ALLEGRO_EVENT ev) {
	switch(ev.keyboard.keycode) {
		case ALLEGRO_KEY_ESCAPE: {
			//if (dev_console->visible) {
			//	dev_console->visible = false;
			//} else {
				running = false;
			//}
			return;
			break;
		}
		case ALLEGRO_KEY_TAB: {
			dev_console->visible = !dev_console->visible;
			return;
			break;
		}
		case ALLEGRO_KEY_ENTER: {
			if (dev_console->visible) { //Move to dev_console's input
				dev_console->executeCommand();
			}
			return;
			break;
		}
	}

	if (dev_console->visible) {
		dev_console->input(ev.keyboard.keycode, keyStates[ALLEGRO_KEY_LSHIFT]);
	}
}

void Core::onKeyDown(ALLEGRO_EVENT ev) {
	/*switch(ev.keyboard.keycode) {
		case ALLEGRO_KEY_W: {
			//rot += 1.0f;
			///camera.camera_z -= 0.1f;
			camera.moveTo(Vector3f(0, 0, 0.08f));
			break;
		}
		case ALLEGRO_KEY_S: {
			//camera.camera_z += 0.1f;
			camera.moveTo(Vector3f(0, 0, -0.08f));
			break;
		}
		case ALLEGRO_KEY_A: {
			//roty -= 1.0f;
			//camera.applyRotation(-0.1, Vector3f(0, 1, 0));
			camera.moveTo(Vector3f(0.8f, 0, 0));
			break;
		}
		case ALLEGRO_KEY_D: {
			//roty += 1.0f;
			//camera.applyRotation(0.1, Vector3f(0, 1, 0));
			camera.moveTo(Vector3f(-0.8f, 0, 0));
			break;
		}
		case ALLEGRO_KEY_Q: {
			camera.moveY(0.01f);
			break;
		}
		case ALLEGRO_KEY_E: {
			camera.moveY(-0.01f);
			break;
		}
	}*/
}