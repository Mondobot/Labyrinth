/*
 * Includes go here
 */
#include <string.h>
#include <iostream>
#include <fstream>

#include "maze_game.h"

GLdouble MazeGame::cube_size_ = 1.5;
GLdouble MazeGame::fine_spacing_ = 0.01;

MazeGame::MazeGame(std::string input_file) {
	this->input_file_ = new char [input_file.size() + 1];
	strcpy(this->input_file_, input_file.c_str());
}

MazeGame::~MazeGame() {
	//TODO: Add destructor code
}

void MazeGame::Init() {
	ReadData();

	//InitObjects
	//InitCamera();

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
			//glColor3f(0.3f, 0.2f, 0.6f);
			glColor3f(0.6f, 0.4f, 0.3f);


			if (this->actual_maze_[i][j] == '.') {
				y_coord *= -1;
			//	glColor3f(0.6f, 0.4f, 0.3f);
				glColor3f(0.3f, 0.2f, 0.6f);
			}

			glTranslatef(j * cube_size_, y_coord, -i * cube_size_);

			glutSolidCube(cube_size_ - fine_spacing_);

			//glColor3f(1.0, 1.0, 1.0);
			glColor3f(0, 0, 0);
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

const char MazeGame::name_[] = "Labyrinth";

const char* MazeGame::name() const { return this->name_; }
