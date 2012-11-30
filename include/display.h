/*
 * Graphic jobs (initialisation, mainGlutLoop etc) are here
 */
#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include <GL/glut.h>
#include "maze_game.h"

class Float3 {
	public:
		Float3() { x = y = z = 0; }

		Float3(float x, float y, float z) : x(x), y(y), z(z) {};

		~Float3() {};
		float x, y, z;
};

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

		static camera_t camera_type;
		static const int kWindowPosX = -1;
		static const int kWindowPosY = -1;
		static const int kWindowSizeX = 800;
		static const int kWindowSizeY = 650;
		static int fov_angle_;
		static int near_plane_;
		static int far_plane_;
		static Float3 *camera_;
		static MazeGame *game_;
};

#endif	//_DISPLAY_H_
