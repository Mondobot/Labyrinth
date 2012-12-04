#include <math.h>
#include <iostream>

#include "display.h"

#define PI 3.14159265
#define RAD(x) ((x) * PI / 180)
#define WHEEL_DOWN 3
#define WHEEL_UP 4
#define PRECISION 0.00001f

// TODO: ALL these initialisations look like crap, clean them!
MazeGame * GlutEngine::game_ = NULL;
int GlutEngine::fov_angle_ = 45;
int GlutEngine::near_plane_ = 1;
int GlutEngine::far_plane_ = 1000;
float GlutEngine::zoom_factor_ = 3.0f;

Float3 * GlutEngine::camera_ = new Float3();
GlutEngine::camera_t GlutEngine::camera_type = OVERVIEW;
Float3 * GlutEngine::orig_coords_mouse_ = new Float3();

Float3 * GlutEngine::view_dir_angle_ = new Float3();
Float3 * GlutEngine::delta_angle_ = new Float3();
Float3 * GlutEngine::camera_dir_ = new Float3(0, 0, 0);
Float3 * GlutEngine::delta_move_ = new Float3();
bool GlutEngine::keys_pressed_[4] = {false};

int GlutEngine::kkt = 0;
int GlutEngine::window_height_ = GlutEngine::kWindowSizeY;
int GlutEngine::window_width_ = GlutEngine::kWindowSizeX;

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

	// TODO: Organise the callbacks 
	// (dunno how, figure something out!)
	// Register callbacks
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ResizeScene);
	glutKeyboardFunc(KeyPress);
	glutKeyboardUpFunc(KeyRelease);
	glutIdleFunc(IdleFunc);
	glutPassiveMotionFunc(MouseMove);
	glutMouseFunc(MouseClick);

	glEnable(GL_DEPTH_TEST);
	glutSetCursor(GLUT_CURSOR_NONE);
}

void GlutEngine::Run() {
	glutMainLoop();
}

// TODO: Figure the order of these calls, looks like a mess!
void GlutEngine::RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	GetMovement();
	UpdatePos();

	SetView();
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
	gluPerspective(fov_angle_, ratio, near_plane_, far_plane_);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);

	SetView();

	window_height_ = height;
	window_width_ = width;
}

void GlutEngine::SetView() {
	glLoadIdentity();

	// TODO: Clean up!
	Float3 player = game_->player();
	Float3 look_at;
	Float3 up;

	int sign = 0;

	switch (camera_type) {
		case (OVERVIEW):
			camera_->x = camera_->z = game_->maze_size() / 2.0;
			camera_->y = camera_->x / tan(RAD(fov_angle_ / 2.0));

			up.x = up.y = 0;
			up.z = -1;

			look_at.x = look_at.z = camera_->x;
			break;

		case (THIRD_PERSON):
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

			sign = (look_at.y - camera_->y) > 0 ? -1 : 1;
			up.x = sign * look_at.x;
			up.z = sign * look_at.z;
			up.y = up.x * up.x + up.z * up.z * 1.0f / look_at.y;
			//up.y += 1;
			up.y *= (up.y < 0) ? -1 : 1;
			
			break;
	}

/*
	std::cout << "gluLookAt: " << camera_->x << " " << camera_->y;
	std::cout << " " << camera_->z << " " << look_at.x << " ";
	std::cout << look_at.y << " " << look_at.z << " " << up.x;
	std::cout << " " << up.y << " " << up.z << std::endl;
*/

	view_dir_angle_->x += delta_angle_->x;
	view_dir_angle_->y += delta_angle_->y;

	delta_angle_->x = delta_angle_->y = delta_angle_->z = 0;

	if (camera_->y < 0.3f)
		camera_->y = 0.3f;

	gluLookAt(camera_->x, camera_->y, camera_->z,
			look_at.x, look_at.y, look_at.z,
			up.x, up.y, up.z);

/*
	Float3 plr = game_->player();
	std::cout << "cam: " << camera_->x << " " << camera_->y << " " << camera_->z;
	std::cout << "\nplayer: " << plr.x << " " << plr.y << " ";
	std::cout << plr.z << std::endl;
*/
}

void GlutEngine::KeyPress(unsigned char key, int x, int y) {
	switch (key) {
		case EXIT:
			// TODO: Exit properly!!!
			// (by calling the destructor)
			exit(0);
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
			//keys_pressed_[1] = false;
			break;

		case BACK:
			keys_pressed_[1] = true;
			//keys_pressed_[0] = false;
			//delta_move_->x -= kMoveSpeed;
			break;

		case RIGHT:
			keys_pressed_[2] = true;
			//keys_pressed_[3] = false;
			//delta_move_->z -= kMoveSpeed;
			break;

		case LEFT:
			keys_pressed_[3] = true;
			//keys_pressed_[2] = false;
			//delta_move_->z += kMoveSpeed;
			break;
	}
}

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

void GlutEngine::GetMovement() {
	delta_move_->x = delta_move_->z = 0;
	if (keys_pressed_[0])
		delta_move_->x += kMoveSpeed;

	if (keys_pressed_[1])
		delta_move_->x -= kMoveSpeed;

	if (keys_pressed_[2])
		delta_move_->z -= kMoveSpeed;

	if (keys_pressed_[3])
		delta_move_->z += kMoveSpeed;
}

void GlutEngine::UpdatePos() {
	int sign = 1;
	Float3 offset;

	switch (camera_type) {
		case (OVERVIEW):
			offset.z -= delta_move_->x;
			offset.x -= delta_move_->z;
			break;

		// TODO: Looks like crap, clean it up!
		case (THIRD_PERSON):
			//player.x += delta_move->x * camera_dir_->x;
			//player.z += delta_move->z * camera_dir_->z;
			

		case (FIRST_PERSON):
			offset.x += delta_move_->x * camera_dir_->x;
			offset.z += delta_move_->x * camera_dir_->z;

			//if (camera_dir_->z

			offset.x += delta_move_->z *  
					sin(view_dir_angle_->x + delta_angle_->x - RAD(90)) *
						sin(view_dir_angle_->y + delta_angle_->y);

			offset.z += delta_move_->z * 
					-cos(view_dir_angle_->x + delta_angle_->x - RAD(90)) *
					sin(view_dir_angle_->y + delta_angle_->y);
/*			std::cout << "Offset_x -> " << view_dir_angle_->x << " + " << delta_angle_->x << "\n";
				std::cout << "Offset_y -> " << view_dir_angle_->y << " + " << delta_angle_->y << "\n";
*/			
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
			delta_angle_->x = (x - orig_coords_mouse_->x) * 0.005f;
			delta_angle_->y = (y - orig_coords_mouse_->y) * 0.005f;

			//	std::cout << x - orig_coords_mouse_->x << " <----------> ";
			//	std::cout << y - orig_coords_mouse_->y << std::endl;

			orig_coords_mouse_->x = x;
			orig_coords_mouse_->y = y;


		// TODO: FIX THIS CRAP
		/*	if (kkt != 1) {
				kkt++;
				orig_coords_mouse_->x = x;
				orig_coords_mouse_->y = y;

				} else  {*/
			if (x < 5 || window_width_ - x < 5 ||
				y < 5 || window_height_ - y < 5) {

				orig_coords_mouse_->x = window_width_ / 2.0f;
				orig_coords_mouse_->y = window_height_ / 2.0f;
				glutWarpPointer(orig_coords_mouse_->x, orig_coords_mouse_->y);


			//		kkt = 0;
			}
			camera_dir_->x = sin(view_dir_angle_->x + delta_angle_->x) *
							sin(view_dir_angle_->y + delta_angle_->y);
			camera_dir_->z = -cos(view_dir_angle_->x + delta_angle_->x) *
							sin(view_dir_angle_->y + delta_angle_->y);
			camera_dir_->y = cos(view_dir_angle_->y + delta_angle_->y);
/*
			// TODO: FIX the magic numbers
			if (camera_dir_->x > 10)
				camera_dir_->x -= 10;

			if (camera_dir_->y > 10)
				camera_dir_->y -= 10;

			if (camera_dir_->z > 10)
				camera_dir_->z -= 10;
*/

			break;
	}
}

void GlutEngine::MouseClick(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
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
