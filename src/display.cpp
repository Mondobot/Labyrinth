#include <math.h>
#include <iostream>

#include "display.h"

#define PI 3.14159265
#define RAD(x) ((x) * PI / 180)

MazeGame * GlutEngine::game_ = NULL;
int GlutEngine::fov_angle_ = 45;
int GlutEngine::near_plane_ = 1;
int GlutEngine::far_plane_ = 1000;
Float3 * GlutEngine::camera_ = new Float3();
GlutEngine::camera_t GlutEngine::camera_type = OVERVIEW;

GlutEngine::GlutEngine(MazeGame *game) {
	game_ = game;
}

GlutEngine::~GlutEngine() {
	//TODO: add destructor code
}

void GlutEngine::Init(int argc, char *argv[]) {

	// Init Glut and create a window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowPosition(this->kWindowPosX, this->kWindowPosY);
	glutInitWindowSize(this->kWindowSizeX, this->kWindowSizeY);
	glutCreateWindow(game_->name());

	// Register callbacks
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ResizeScene);

	glEnable(GL_DEPTH_TEST);
}

void GlutEngine::Run() {
	glutMainLoop();
}

void GlutEngine::RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	game_->RenderSelf();

	SetView();
	glutSwapBuffers();
}

void GlutEngine::ResizeScene(int width, int height) {
	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	//Reset Matrix
	glLoadIdentity();

	//Set the viewport to be the entire window
	glViewport(0, 0, width, height);

	// Set the correct perspective.
	float ratio = width * 1.0 / height;
	gluPerspective(fov_angle_, ratio, near_plane_, far_plane_);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);

	SetView();
}

void GlutEngine::SetView() {
	glLoadIdentity();

	Float3 look_at;
	Float3 up;
	
	switch (camera_type) {
		case (OVERVIEW):
			camera_->x = camera_->z = game_->maze_size() / 2.0;
			camera_->y = camera_->x / tan(RAD(fov_angle_ / 2.0));

			up.x = up.y = 0;
			up.z = 1;

			look_at.x = look_at.z = camera_->x;
			break;
	}

/*
	std::cout << "gluLookAt: " << camera_->x << " " << camera_->y;
	std::cout << " " << camera_->z << " " << look_at.x << " ";
	std::cout << look_at.y << " " << look_at.z << " " << up.x;
	std::cout << up.y << " " << up.z << std::endl;
*/
	gluLookAt(camera_->x, camera_->y, camera_->z,
			look_at.x, look_at.y, look_at.z,
			up.x, up.y, up.z);
}
