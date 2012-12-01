#include <math.h>
#include <iostream>

#include "display.h"

#define PI 3.14159265
#define RAD(x) ((x) * PI / 180)

MazeGame * GlutEngine::game_ = NULL;
int GlutEngine::fov_angle_ = 45;
int GlutEngine::near_plane_ = 1;
int GlutEngine::far_plane_ = 1000;
float GlutEngine::delta_move_ = 0;
Float3 * GlutEngine::camera_ = new Float3();
GlutEngine::camera_t GlutEngine::camera_type = OVERVIEW;
Float3 * GlutEngine::camera_dir_ = new Float3(0, 0, 1);


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
	glutKeyboardFunc(KeyPress);
	glutIdleFunc(IdleFunc);

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

	Float3 player = game_->player();
	Float3 look_at;
	Float3 up;

	int sign = 0;

	switch (camera_type) {
		case (OVERVIEW):
			camera_->x = camera_->z = game_->maze_size() / 2.0;
			camera_->y = camera_->x / tan(RAD(fov_angle_ / 2.0));

			up.x = up.y = 0;
			up.z = 1;

			look_at.x = look_at.z = camera_->x;
			break;

		case (FIRST_PERSON):
			camera_->x = player.x;
			camera_->y = player.y;
			camera_->z = player.z;

			look_at.x = player.x + camera_dir_->x;
			look_at.y = player.y + camera_dir_->y;
			look_at.z = player.z + camera_dir_->z;

			sign = (look_at.y - camera_->y) > 0 ? 1 : -1;
			up.x = sign * look_at.x;
			up.z = sign * look_at.z;
			up.y = up.x * up.x + up.z * up.z * 1.0f / look_at.y;
			//up.y = floor(up.y);
			
			break;
	}


	std::cout << "gluLookAt: " << camera_->x << " " << camera_->y;
	std::cout << " " << camera_->z << " " << look_at.x << " ";
	std::cout << look_at.y << " " << look_at.z << " " << up.x;
	std::cout << " " << up.y << " " << up.z << std::endl;

	gluLookAt(camera_->x, camera_->y, camera_->z,
			look_at.x, look_at.y, look_at.z,
			up.x, up.y, up.z);
}

void GlutEngine::KeyPress(unsigned char key, int x, int y) {
	switch (key) {
		case KEY_ESCAPE:
			// TODO: Exit properly!!!
			exit(0);
			break;

		case KEY_1:
			camera_type = OVERVIEW;
			break;

		case KEY_2:
			camera_type = THIRD_PERSON;
			break;

		case KEY_3:
			camera_type = FIRST_PERSON;
			break;

		case KEY_W:
			delta_move_ += kMoveSpeed;
			break;

		case KEY_S:
			delta_move_ -= kMoveSpeed;
			break;
	}
}

void GlutEngine::IdleFunc() {
	glutPostRedisplay();
}
