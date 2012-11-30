#include <iostream>

#include "display.h"

MazeGame * GlutEngine::game_ = NULL;
int GlutEngine::fov_angle = 45;
int GlutEngine::near_plane = 1;
int GlutEngine::far_plane = 1000;

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
}

void GlutEngine::Run() {
	glutMainLoop();
}

void GlutEngine::RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	game_->RenderSelf();

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
	gluPerspective(fov_angle, ratio, near_plane, far_plane);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}
