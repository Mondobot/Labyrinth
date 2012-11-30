/*
 * Includes go here.
 */
#ifndef MAZE_GAME_H_
#define MAZE_GAME_H_
#include <GL/glut.h>
#include <string>


class MazeGame {
	public:
		explicit MazeGame(std::string input_file);
		~MazeGame();

		void Init();
		void RenderSelf();

		const char* name() const;

	private:
		void ReadData();
		void InitObjects();
		//void InitCamera();

		char *input_file_;
		char **actual_maze_;
		int maze_size_;
		static GLdouble fine_spacing_;
		static GLdouble cube_size_;
		static const char name_[];
};
#endif	//MAZE_GAME_H_
