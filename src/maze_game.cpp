/*
 * Includes go here
 */
#include <math.h>
#include <time.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include "maze_game.h"

#define BLACK 0.0f, 0.0f, 0.0f
#define PURPLE 0.3f, 0.2f, 0.6f
#define ORANGE 0.6f, 0.4f, 0.3f
#define WHITE 1.0f, 1.0f, 1.0f
#define PORTAL_RED 0.9f, 0.2f, 0.2f

const char MazeGame::game_name_[] = "Labyrinth";

GLdouble MazeGame::fine_spacing_ = 0.01;
GLdouble MazeGame::cube_size_ = 1.5;

const float MazeGame::kMinDist = cube_size_ / 10;
int MazeGame::score_ = 0;

// Any object that have to be randomly placed on the map must
// start with x = -1!!
Float3 * MazeGame::player_ = new Float3(-1, cube_size_ / 2.0f, -1);
Float3 * MazeGame::portal_ = new Float3(-1, cube_size_ / 2.0f, -1);

// Constructor
MazeGame::MazeGame(std::string input_file) {
	this->input_file_ = new char [input_file.size() + 1];
	strcpy(this->input_file_, input_file.c_str());
}

MazeGame::~MazeGame() {
	//TODO: Add destructor code
}

void MazeGame::Init() {
	srand(time(NULL));

	ReadData();
	PlaceRandObject(player_);
	PlaceRandObject(portal_);

	// Print the maze (at first for debugging purposes,
	// but now because it looks cool >:)
	for (int i = 0; i < this->maze_size_; ++i) {
		for (int j = 0; j < this->maze_size_; ++j)
			std::cout << this->actual_maze_[i][j] << " ";

		std::cout << std::endl;
	}
}

// Reads the input from the input file
// The format is the following:
//	n :	-> dimension of the maze
//		-> currently only square mazes are supported
//
//	matrix :	-> given line by line
//			-> '.' for passable terrain
//			-> '#' for walls
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

// Gets called by the engine.
// Renders the maze, player and1 "portal"
void MazeGame::RenderSelf(void) {
	for (int i = 0; i < this->maze_size_; ++i)
		for (int j = 0; j < this->maze_size_; ++j) {
			glPushMatrix();

			// Height of all objects is set to a default
			// (except from floors which have a negative height)
			GLdouble y_coord = cube_size_ / 2;
			glColor3f(ORANGE);

			// Floors have a different height and color
			if (this->actual_maze_[i][j] == '.') {
				y_coord *= -1;
				glColor3f(PURPLE);
			}

			glTranslatef(i * cube_size_, y_coord, j * cube_size_);
			glutSolidCube(cube_size_ - fine_spacing_);

			// Draw the black "lines"
			glColor3f(BLACK);
			glutWireCube(cube_size_);

			glPopMatrix();
		}

	// Draw the player
	// Dimensions are kept as magic numbers
	// (too lazy to keep them in a proper way)
	glPushMatrix();

	glTranslatef(player_->x, player_->y, player_->z);
	glColor3f(BLACK);
	glutSolidSphere(cube_size_ / 5, 20, 20);

	glPopMatrix();

	// Draw the portal
	// Dimensions are also magic numbers
	glPushMatrix();

	glTranslatef(portal_->x, portal_->y, portal_->z);
	glColor3f(PORTAL_RED);
	glutSolidTorus(cube_size_ / 10, cube_size_ / 3, 30, 20);

	glPopMatrix();
}

// Simple function for detecting a one dimensional distance
bool MazeGame::InRange(float a, float b) {
	return (abs(a - b) < (kMinDist));
}

// When we want to check for collisions we check the 8 surrounding
// squares; I chose this because the bounding spheres method looked
// ugly
void MazeGame::DetectCollisions(Float3 &offset) {
	// Get the position of the player in the maze matrix
	int x = round(player_->x / cube_size_);
	int y = round(player_->z / cube_size_);

	// For each of the 8 surrounding squares
	for (int i = -1; i <= 1; ++i)
		for (int j = -1; j <= 1; ++j) {
			if (i == 0 && j == 0)
				continue;

/* I chose to leave this commented because it did not behave as I wanted;
 * It prevents my sphere to pass through cube corners (checks the diagonal
 * cubes for collisions) but the movement is not natural at all
 * (the sphere tends to glitch when moving while hugging a wall)

		if (i != 0 && j != 0 && actual_maze_[x + i][y + j] == '#')
			if (InRange((player_->x + offset.x), ((x + i) * cube_size_)) &&
				InRange((player_->z + offset.z), ((y + j) * cube_size_))) {
					offset.x = 0;
					offset.z = 0;
			}
*/

			if (i != 0 && actual_maze_[x + i][y] == '#')
				if (InRange((player_->x + offset.x),
							((x + i) * cube_size_)))
					offset.x = 0;

			if (j != 0 && actual_maze_[x][y + j] == '#')
				if (InRange((player_->z + offset.z),
							((y + j) * cube_size_)))
					offset.z = 0;
		}

	// If the "portal" is reached we just spawn it
	// somwhere else (we're devilishly intelligent!);
	// Doing so will prevent our little ball from ever
	// leaving the labyrinth! Mwahahahaha!
	if (x == round(portal_->x / cube_size_) &&
		y == round(portal_->z / cube_size_)) {
			portal_->x = -1;
			++score_;
			PlaceRandObject(portal_);
	}
}

// Places an object on a random free square on the map
void MazeGame::PlaceRandObject(Float3 *obj) {
	// Since I use this only for the player and "portal"
	// I have hardcoded the height
	obj->y = 3.0 / 8 * cube_size_;

	int i, j;
	while (obj->x == -1) {
		i = rand() % this->maze_size_;
		j = rand() % this->maze_size_;

		if (this->actual_maze_[i][j] != '#') {
			obj->x = i * cube_size_;
			obj->z = j * cube_size_;
		}
	}
}

// Does what it says, pretty straightforward 
void MazeGame::update_player_pos(Float3 offset) {
	player_->x += offset.x;
	player_->y += offset.y;
	player_->z += offset.z;
}

// All the getters
GLdouble MazeGame::maze_size() const { return this->maze_size_ * cube_size_; }

Float3 MazeGame::player() const { return *player_; }

const char* MazeGame::game_name() const { return this->game_name_; }

int MazeGame::score() const { return this->score_; }
