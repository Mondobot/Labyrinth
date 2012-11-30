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
		GLdouble maze_size() const;

	private:
		void ReadData();
		void PlacePlayer();
		//void InitCamera();
		
		char *input_file_;
		char **actual_maze_;
		int maze_size_;
		static GLdouble fine_spacing_;
		static GLdouble cube_size_;
		static const char name_[];
		static GLdouble plyr_pos_x_;
		static GLdouble plyr_pos_y_;
};
#endif	//MAZE_GAME_H_
