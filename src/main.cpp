/*
 * Includes go here:
 */
#include <stdlib.h>
#include <iostream>



int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cout << "Unrecognised number of arguments!\n";
		std::cout << "(got " << argc << "%d, expected 1)\n";
		exit(1);
	}

	MazeGame game = new MazeGame();

	game.init();
	game.run();

	return 0;
}
