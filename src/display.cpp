#include <iostream>
#include "display.h"

MazeGame * GlutEngine::game_ = NULL;

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
}

void GlutEngine::Run() {
	glutMainLoop();
}

void GlutEngine::RenderScene(void) {
	game_->RenderSelf();
}
