/*
 * Graphic jobs (initialisation, mainGlutLoop etc) are here
 */
#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include <GL/glut.h>
#include "maze_game.h"
#include "float3.h"

class GlutEngine {
	public:
		enum camera_t {OVERVIEW, THIRD_PERSON, FIRST_PERSON};

		explicit GlutEngine(MazeGame *game);
		~GlutEngine();

		void Init(int argc, char *argv[]);
		void Run();

	private:
		static void RenderScene();
		static void ResizeScene(int width, int height);
		static void SetView();
		static void IdleFunc();
		static void KeyPress(unsigned char key, int x, int y);

		static camera_t camera_type;
		static const int kWindowPosX = -1;
		static const int kWindowPosY = -1;
		static const int kWindowSizeX = 800;
		static const int kWindowSizeY = 650;
		static const float kMoveSpeed = 0.5f;

		static int fov_angle_;
		static int near_plane_;
		static int far_plane_;

		static Float3 *camera_;
		static float view_dir_angle_;
		static Float3 *delta_angle_;
		static float delta_move_;
		static Float3 *camera_dir_;

		static MazeGame *game_;

		static const int KEY_ESCAPE = 27;
		static const int KEY_1 = '1';
		static const int KEY_2 = '2';
		static const int KEY_3 = '3';

		static const int KEY_W = 'w';
		static const int KEY_S = 's';
		static const int KEY_A = 'a';
		static const int KEY_D = 'd';


};

#endif	//_DISPLAY_H_
