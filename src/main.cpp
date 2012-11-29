/*
 * Includes go here:
 */
#include <stdlib.h>
#include <iostream>

#include "maze_game.h"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cout << "Unrecognised number of arguments!\n";
		std::cout << "(got " << argc << "%d, expected 1)\n";
		exit(1);
	}

	MazeGame game(argv[1]);

	game.Init();
	//game.Run();

	return 0;
}
