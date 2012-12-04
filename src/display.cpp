#include <math.h>
#include <iostream>
#include <sstream>

#include "display.h"

#define PI 3.14159265
#define RAD(x) ((x) * PI / 180)
#define WHEEL_DOWN 3
#define WHEEL_UP 4
#define PRECISION 0.00001f

GlutEngine::camera_t GlutEngine::camera_type = OVERVIEW;
MazeGame * GlutEngine::game_ = NULL;

int GlutEngine::fov_angle_ = 45;
int GlutEngine::near_plane_ = 1;
int GlutEngine::far_plane_ = 1000;
float GlutEngine::zoom_factor_ = 3.0f;
float GlutEngine::mouse_sensitivity_ = 0.005f;

Float3 * GlutEngine::camera_ = new Float3();
Float3 * GlutEngine::orig_coords_mouse_ = new Float3();
Float3 * GlutEngine::view_dir_angle_ = new Float3();
Float3 * GlutEngine::delta_angle_ = new Float3();
Float3 * GlutEngine::delta_move_ = new Float3();
Float3 * GlutEngine::camera_dir_ = new Float3();

int GlutEngine::window_height_ = GlutEngine::kWindowSizeY;
int GlutEngine::window_width_ = GlutEngine::kWindowSizeX;

bool GlutEngine::keys_pressed_[4] = {false};

// Constructor
GlutEngine::GlutEngine(MazeGame *game) {
	game_ = game;
}

GlutEngine::~GlutEngine() {
	game_->MazeGame::~MazeGame();

	delete camera_;
	delete orig_coords_mouse_;
	delete view_dir_angle_;
	delete delta_angle_;
	delete delta_move_;
	delete camera_dir_;
}

void GlutEngine::Init(int argc, char *argv[]) {

	// Init Glut and create a window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE |
					GLUT_RGBA | GLUT_MULTISAMPLE);

	glutInitWindowPosition(this->kWindowPosX, this->kWindowPosY);
	glutInitWindowSize(this->kWindowSizeX, this->kWindowSizeY);
	glutCreateWindow(game_->game_name());

	// Register callbacks
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ResizeScene);
	glutKeyboardFunc(KeyPress);
	glutKeyboardUpFunc(KeyRelease);
	glutIdleFunc(IdleFunc);
	glutPassiveMotionFunc(MouseMove);
	glutMouseFunc(MouseClick);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glutSetCursor(GLUT_CURSOR_NONE);
}

// The function is called when we want to start the game
void GlutEngine::Run() {
	glutMainLoop();
}

// 
void GlutEngine::RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	GetMovement();
	UpdatePos();

	// Set the camera and draw the maze
	SetView();
	game_->RenderSelf();
	PrintScore();

	glutSwapBuffers();
}

void GlutEngine::ResizeScene(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Set the viewport to be the entire window
	glViewport(0, 0, width, height);

	// Set the correct perspective.
	float ratio = width * 1.0 / height;
	gluPerspective(fov_angle_, ratio, near_plane_, far_plane_);

	glMatrixMode(GL_MODELVIEW);

	SetView();

	// We need these to be updated each time a resizing occurs
	window_height_ = height;
	window_width_ = width;
}

// Sets the 3 different camera views
void GlutEngine::SetView() {
	glLoadIdentity();

	Float3 player = game_->player();
	Float3 look_at;
	Float3 up;
	int sign;

	switch (camera_type) {
		case (OVERVIEW):
			camera_->x = camera_->z = game_->maze_size() / 2.0;

			// Height is calculated so the maze will fit the screen
			camera_->y = camera_->x / tan(RAD(fov_angle_ / 2.0));

			up.x = up.y = 0;
			up.z = -1;

			look_at.x = look_at.z = camera_->x;
			break;

		case (THIRD_PERSON):
			if (zoom_factor_ - 0 < PRECISION)
				camera_type = FIRST_PERSON;

			camera_->x = player.x - camera_dir_->x * zoom_factor_;
			camera_->y = player.y - camera_dir_->y * (zoom_factor_ + 2);
			camera_->z = player.z - camera_dir_->z * zoom_factor_;

			look_at.x = player.x;
			look_at.y = player.y;
			look_at.z = player.z;

			// Direction is always "up"
			up.x = up.z = 0;
			up.y = 1;
			break;

		case (FIRST_PERSON):
			camera_->x = player.x;
			camera_->y = player.y;
			camera_->z = player.z;

			look_at.x = player.x + camera_dir_->x;
			look_at.y = player.y + camera_dir_->y;
			look_at.z = player.z + camera_dir_->z;

			// Calculate the 90 degrees up vector
			sign = (look_at.y - camera_->y) > 0 ? -1 : 1;
			up.x = sign * look_at.x;
			up.z = sign * look_at.z;

			up.y = up.x * up.x + up.z * up.z * 1.0f / look_at.y;
			up.y *= (up.y < 0) ? -1 : 1;
			
			break;
	}

	// Update the direction (we don't need it in this frame anymore)
	view_dir_angle_->x += delta_angle_->x;
	view_dir_angle_->y += delta_angle_->y;

	delta_angle_->x = delta_angle_->y = delta_angle_->z = 0;

	// A small (and hardcoded) check so the camera 
	// doesn't go below the floor (usually :P)
	if (camera_->y < 0.3f)
		camera_->y = 0.3f;

	gluLookAt(camera_->x, camera_->y, camera_->z,
			look_at.x, look_at.y, look_at.z,
			up.x, up.y, up.z);
}

// Prints the current score in the bottem left corner of the screen
void GlutEngine::PrintScore() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f, window_width_, 0.0f, window_height_, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);

	// We have the matrix in place, now print the Score string
	glRasterPos2d(10.0f, 10.0f);

	std::stringstream out;
	out << "Score: " << game_->score();
	std::string ceva = out.str();
	
	for (int i = 0; i < (int)ceva.length(); ++i)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ceva[i]);

	// Revert to the normal view
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

// Callback function for handling keys
void GlutEngine::KeyPress(unsigned char key, int x, int y) {
	switch (key) {
		case EXIT:
			glutLeaveMainLoop();
			break;

		case CAM_1:
			camera_type = OVERVIEW;
			break;

		case CAM_2:
			camera_type = THIRD_PERSON;
			break;

		case CAM_3:
			camera_type = FIRST_PERSON;
			
			break;

		case FWD:
			keys_pressed_[0] = true;
			break;

		case BACK:
			keys_pressed_[1] = true;
			break;

		case RIGHT:
			keys_pressed_[2] = true;
			break;

		case LEFT:
			keys_pressed_[3] = true;
			break;
	}
}

// Function for handling key releases;
// needed for multiple keyboard input
void GlutEngine::KeyRelease(unsigned char key, int x, int y) {
	switch (key) {
		case (FWD):
			keys_pressed_[0] = false;
			break;

		case (BACK):
			keys_pressed_[1] = false;
			break;

		case (RIGHT):
			keys_pressed_[2] = false;
			break;

		case (LEFT):
			keys_pressed_[3] = false;
			break;
	}
}

void GlutEngine::IdleFunc() {
	glutPostRedisplay();
}

// This function checks if any of the movement
// keys are pressed. Directions are maped 
void GlutEngine::GetMovement() {
	delta_move_->x = delta_move_->z = 0;

	// FWD is pressed
	if (keys_pressed_[0])
		delta_move_->x += kMoveSpeed;

	// BACK is pressed
	if (keys_pressed_[1])
		delta_move_->x -= kMoveSpeed;

	// RIGHT is pressed
	if (keys_pressed_[2])
		delta_move_->z -= kMoveSpeed;

	// LEFT is pressed
	if (keys_pressed_[3])
		delta_move_->z += kMoveSpeed;
}

// Computes the new player position depending on the camera type
void GlutEngine::UpdatePos() {
	Float3 offset;

	switch (camera_type) {
		case (OVERVIEW):
			offset.z -= delta_move_->x;
			offset.x -= delta_move_->z;
			break;

		// TODO: Looks like crap, clean it up!
		case (THIRD_PERSON):

		case (FIRST_PERSON):
			offset.x += delta_move_->x * camera_dir_->x;
			offset.z += delta_move_->x * camera_dir_->z;

			// In order to allow strafing we need to shift  the current
			// view direction with 90 degrees
			offset.x += delta_move_->z *
					sin(view_dir_angle_->x + delta_angle_->x - RAD(90)) *
					sin(view_dir_angle_->y + delta_angle_->y);

			offset.z += delta_move_->z * 
					-cos(view_dir_angle_->x + delta_angle_->x - RAD(90)) *
					sin(view_dir_angle_->y + delta_angle_->y);
			break;
	}

	game_->DetectCollisions(offset);
	game_->update_player_pos(offset);
}

void GlutEngine::MouseMove(int x, int y) {
	switch (camera_type) {
		case (OVERVIEW):
			break;

		case (THIRD_PERSON):

		case (FIRST_PERSON):
			delta_angle_->x = (x - orig_coords_mouse_->x) * mouse_sensitivity_;
			delta_angle_->y = (y - orig_coords_mouse_->y) * mouse_sensitivity_;

			// We need the original coords in order
			// too see how much the mouse hase moved between frames
			orig_coords_mouse_->x = x;
			orig_coords_mouse_->y = y;

			// Small hack to prevent the mouse from leaving the screen
			// and to ensure that we can always move it
			if (x < 10 || window_width_ - x < 10 ||
				y < 10 || window_height_ - y < 10) {

				orig_coords_mouse_->x = window_width_ / 2.0f;
				orig_coords_mouse_->y = window_height_ / 2.0f;
				glutWarpPointer(orig_coords_mouse_->x, orig_coords_mouse_->y);
			}

			// Compute the camera directions using spherical coords
			camera_dir_->x = sin(view_dir_angle_->x + delta_angle_->x) *
							sin(view_dir_angle_->y + delta_angle_->y);
			camera_dir_->z = -cos(view_dir_angle_->x + delta_angle_->x) *
							sin(view_dir_angle_->y + delta_angle_->y);
			camera_dir_->y = cos(view_dir_angle_->y + delta_angle_->y);
			break;
	}
}

// Enable zoom in / zoom out by scrolling
// The function also enable a player to switch between
// FIRST_PERSON and THIRD_PERSON by scrolling
void GlutEngine::MouseClick(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		// Prevent the player from scrolling while in first person
		if (button == WHEEL_DOWN && camera_type == THIRD_PERSON) {
			zoom_factor_ -= 1.0f;

			if (zoom_factor_ - 0 < PRECISION)
				camera_type = FIRST_PERSON;

		} else if (button == WHEEL_UP) {
			zoom_factor_ += 1.0f;
			
			if (zoom_factor_ - 0 > PRECISION)
				camera_type = THIRD_PERSON;
		}
	}
}
