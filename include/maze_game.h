/*
 * Main game class is here
 */
#ifndef MAZE_GAME_H_
#define MAZE_GAME_H_
#include <GL/glut.h>
#include <string>

#include "float3.h"

class MazeGame {
	public:
		explicit MazeGame(std::string input_file);
		~MazeGame();

		void Init();
		void RenderSelf();
		void DetectCollisions(Float3 &delta_move);

		// Getters and setters
		const char* game_name() const;
		GLdouble maze_size() const;
		Float3 player() const;
		int score() const;
		void update_player_pos(Float3 offset);

	private:
		void ReadData();
		void PlaceRandObject(Float3 *obj);
		inline bool InRange(float a, float b);

		char *input_file_;
		static const char game_name_[];

		char **actual_maze_;
		int maze_size_;
		static GLdouble fine_spacing_;
		static GLdouble cube_size_;

		static int score_;
		static const float kMinDist;

		static Float3 *player_;
		static Float3 *portal_;
};
#endif	//MAZE_GAME_H_
