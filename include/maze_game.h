/*
 * Includes go here.
 */
#ifndef MAZE_GAME_H_
#define MAZE_GAME_H_

public class MazeGame {

	public explicit MazeGame(std::string input_file) {

		this->input_file = new char [input_file.size() + 1];
		strcpy(this->input_file, input_file.c_str());
	}

	public void Init();
	public void Run();

	private void LoadDataOrDie();
	private void InitDisplay();
	private void InitCamera();


	private char *input_file_;
}
#endif	//MAZE_GAME_H_
