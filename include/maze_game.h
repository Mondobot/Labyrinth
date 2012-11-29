/*
 * Includes go here.
 */
#ifndef MAZE_GAME_H_
#define MAZE_GAME_H_

class MazeGame {
	public:
		explicit MazeGame(std::string input_file);
		~MazeGame();

		void Init();
		void Run();

	private:
		void ReadData();
		void InitDisplay();
		void InitObjects();
		void InitCamera();

		char *input_file_;
		char **actual_maze_;
		int maze_size_;
};
#endif	//MAZE_GAME_H_
