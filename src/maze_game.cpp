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

GLdouble MazeGame::cube_size_ = 1.5;
GLdouble MazeGame::fine_spacing_ = 0.01;
Float3 * MazeGame::player_ = new Float3(-1, cube_size_ / 2.0f, -1);
Float3 * MazeGame::portal_ = new Float3(-1, cube_size_ / 2.0f, -1);
const char MazeGame::name_[] = "Labyrinth";
int MazeGame::score_ = 0;
const float MazeGame::kMinDist = cube_size_ / 10;

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

// TODO: Works, but needs some cleaning
void MazeGame::RenderSelf(void) {
	for (int i = 0; i < this->maze_size_; ++i)
		for (int j = 0; j < this->maze_size_; ++j) {
			glPushMatrix();

			GLdouble y_coord = cube_size_ / 2;
			glColor3f(ORANGE);


			if (this->actual_maze_[i][j] == '.') {
				y_coord *= -1;
				glColor3f(PURPLE);
			}


			glTranslatef(i * cube_size_, y_coord, j * cube_size_);

			glutSolidCube(cube_size_ - fine_spacing_);

			//glColor3f(WHITE);
			glColor3f(BLACK);
			glutWireCube(cube_size_);

			glPopMatrix();
		}

	// Draw the player
	glPushMatrix();

	glTranslatef(player_->x, player_->y, player_->z);
	glColor3f(BLACK);
	glutSolidSphere(cube_size_ / 5, 20, 20);

	glPopMatrix();

	// Draw the portal
	glPushMatrix();

	glTranslatef(portal_->x, portal_->y, portal_->z);
	glColor3f(0.9f, 0.2f, 0.2f);
	glutSolidTorus(cube_size_ / 10, cube_size_ / 3, 30, 20);

	glPopMatrix();

	//std::cout << "player: " << player_->x << " " << player_->y << " " << player_->z << std::endl;

/*
	glBegin(GL_TRIANGLES);
		glVertex3f(2.0, 0.0, -6.0);
		glVertex3f(-2.0, 0.0, -6.0);
		glVertex3f(0.0, 2.0, -6.0);
	glEnd();
*/
}

bool MazeGame::InRange(float a, float b) {
	return (abs(a - b) < (kMinDist)); //+ cube_size_ / 2));
}

void MazeGame::DetectCollisions(Float3 &offset) {
	// Check wall collisions
	int x = round(player_->x / cube_size_);
	int y = round(player_->z / cube_size_);

	for (int i = -1; i <= 1; ++i)
		for (int j = -1; j <= 1; ++j) {
			if (i == 0 && j == 0)
				continue;
/*
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

	if (x == round(portal_->x / cube_size_) &&
		y == round(portal_->z / cube_size_)) {
			portal_->x = -1;
			++score_;
			PlaceRandObject(portal_);
	}
}

void MazeGame::PlaceRandObject(Float3 *obj) {
	std::cout << "Placing rand obj\n";

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

	std::cout << obj->x / cube_size_ << " " << obj->z / cube_size_ << std::endl;
}

void MazeGame::update_player_pos(Float3 offset) {
	player_->x += offset.x;
	player_->y += offset.y;
	player_->z += offset.z;
}

GLdouble MazeGame::maze_size() const { return this->maze_size_ * cube_size_; }

Float3 MazeGame::player() const { return *player_; }

const char* MazeGame::name() const { return this->name_; }

int MazeGame::score() const { return this->score_; }
