/*
 * Graphic jobs (initialisation, mainGlutLoop etc) are here
 */
#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "maze_game.h"
#include "float3.h"

class GlutEngine {
	public:
		// Camera types
		enum camera_t {OVERVIEW, THIRD_PERSON, FIRST_PERSON};

		explicit GlutEngine(MazeGame *game);
		~GlutEngine();

		void Init(int argc, char *argv[]);
		void Run();

	private:
		static void RenderScene();
		static void ResizeScene(int width, int height);
		static void SetView();
		static void PrintScore();
		static void IdleFunc();
		static void KeyPress(unsigned char key, int x, int y);
		static void KeyRelease(unsigned char key, int x, int y);
		static void UpdatePos();
		static void GetMovement();
		static void MouseMove(int x, int y);
		static void MouseClick(int button, int state, int x, int y);

		static camera_t camera_type;
		static const int kWindowPosX = -1;
		static const int kWindowPosY = -1;
		static const int kWindowSizeX = 1200;
		static const int kWindowSizeY = 600;
		static const float kMoveSpeed = 0.1f;

		static MazeGame *game_;

		static int fov_angle_;
		static int near_plane_;
		static int far_plane_;
		static float zoom_factor_;
		static float mouse_sensitivity_;

		static Float3 *camera_;
		static Float3 *orig_coords_mouse_;
		static Float3 *view_dir_angle_;
		static Float3 *delta_angle_;
		static Float3 *delta_move_;
		static Float3 *camera_dir_;

		static int window_height_;
		static int window_width_;

		static const int EXIT = 27;
		static const int CAM_1 = '1';
		static const int CAM_2 = '2';
		static const int CAM_3 = '3';

		static const int FWD = 'w';
		static const int BACK = 's';
		static const int LEFT= 'a';
		static const int RIGHT = 'd';
		static bool keys_pressed_[4];
};
#endif	//_DISPLAY_H_
