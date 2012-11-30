/*
 * Includes go here
 */
#include <time.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include "maze_game.h"

#define BLACK 0.0f, 0.0f, 0.0f
#define PURPLE 0.6f, 0.4f, 0.3f
#define ORANGE 0.3f, 0.2f, 0.6f
#define WHITE 1.0f, 1.0f, 1.0f

GLdouble MazeGame::cube_size_ = 1.5;
GLdouble MazeGame::fine_spacing_ = 0.01;
GLdouble MazeGame::plyr_pos_x_ = -1;
GLdouble MazeGame::plyr_pos_y_ = -1;
const char MazeGame::name_[] = "Labyrinth";

MazeGame::MazeGame(std::string input_file) {
	this->input_file_ = new char [input_file.size() + 1];
	strcpy(this->input_file_, input_file.c_str());
}

MazeGame::~MazeGame() {
	//TODO: Add destructor code
}

void MazeGame::Init() {
	ReadData();
	PlacePlayer();

	for (int i = 0; i < this->maze_size_; ++i) {
		for (int j = 0; j < this->maze_size_; ++j)
			std::cout << this->actual_maze_[i][j] << " ";

		std::cout << std::endl;
	}
}

void MazeGame::ReadData() {
	// Open the file for reading
	std::ifstream fin(this->input_file_);

	fin >> this->maze_size_;
	this->actual_maze_ = new char *[this->maze_size_ + 1];

	// Read the maze
	for (int i = 0; i < this->maze_size_; ++i) {
		this->actual_maze_[i] = new char [this->maze_size_ + 1];

		for (int j = 0; j < this->maze_size_; ++j)
			fin >> this->actual_maze_[i][j];
	}

	fin.close();
}

void MazeGame::RenderSelf(void) {

	for (int i = 0; i < this->maze_size_; ++i)
		for (int j = 0; j < this->maze_size_; ++j) {
			glPushMatrix();

			GLdouble y_coord = cube_size_ / 2;
			glColor3f(PURPLE);


			if (this->actual_maze_[i][j] == '.') {
				y_coord *= -1;
				glColor3f(ORANGE);
			}


			glTranslatef(j * cube_size_, y_coord, i * cube_size_);

			glutSolidCube(cube_size_ - fine_spacing_);

			//glColor3f(WHITE);
			glColor3f(BLACK);
			glutWireCube(cube_size_);

			glPopMatrix();
		}

/*
	glBegin(GL_TRIANGLES);
		glVertex3f(2.0, 0.0, -6.0);
		glVertex3f(-2.0, 0.0, -6.0);
		glVertex3f(0.0, 2.0, -6.0);
	glEnd();
*/
}

void MazeGame::PlacePlayer() {
	srand(time(NULL));

	int i, j;
	while (plyr_pos_x_ == -1) {
		i = rand() % this->maze_size_;
		j = rand() % this->maze_size_;

		if (this->actual_maze_[i][j] != '#') {
			plyr_pos_x_ = i;
			plyr_pos_y_ = j;
		}
	}

	std::cout << plyr_pos_x_ << " " << plyr_pos_y_ << std::endl;
}

GLdouble MazeGame::maze_size() const { return this->maze_size_ * cube_size_; }

const char* MazeGame::name() const { return this->name_; }
