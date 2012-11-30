/*
 * Graphic jobs (initialisation, mainGlutLoop etc) are here
 */
#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include <GL/glut.h>
#include "maze_game.h"

class GlutEngine {
	public:
		explicit GlutEngine(MazeGame *game);
		~GlutEngine();

		void Init(int argc, char *argv[]);
		void Run();

	private:
		static void RenderScene();
		static void ResizeScene(int width, int height);

		static const int kWindowPosX = -1;
		static const int kWindowPosY = -1;
		static const int kWindowSizeX = 800;
		static const int kWindowSizeY = 650;
		static int fov_angle;
		static int near_plane;
		static int far_plane;
		static MazeGame *game_;
};

#endif	//_DISPLAY_H_
