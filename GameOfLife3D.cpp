#include "GameOfLife3D.h"

GameOfLife3D::GameOfLife3D(int xSize, int ySize, int zSize) {

	this->generation_count = 0;
	this->update_interval  = 10;
	this->xSize = xSize;
	this->ySize = ySize;
	this->zSize = zSize;

	// Memory allocation
	this->cubeMatrix = new Cube**[xSize];
	this->liveMatrix = new int**[xSize];
	for (int i = 0; i < xSize; i++) {
		this->cubeMatrix[i] = new Cube*[ySize];
		this->liveMatrix[i] = new int*[ySize];
		for (int j = 0; j < ySize; j++) {
			this->cubeMatrix[i][j] = new Cube[zSize];
			this->liveMatrix[i][j] = new int[zSize];
		}
	}	

	// Set everything to 0
	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			for (int k = 0; k < zSize; k++) {
				this->liveMatrix[i][j][k] = 0;
			}
		}
	}

	// Put cubes on the matrix
	for (int i = 1; i < xSize-1; i++) {
		for (int j = 1; j < ySize-1; j++) {
			for (int k = 1; k < zSize-1; k++){
				cubeMatrix[i][j][k] = Cube(glm::vec3(i, j, k)); 
				liveMatrix[i][j][k] = (rand() % 2) * (rand() % 2);
			}
		}
	}
}

void GameOfLife3D::Render() {
	for (int i = 1; i < xSize-1; i++) {
		for (int j = 1; j < ySize-1; j++) {
			for (int k = 1; k < zSize-1; k++){

				// Render alive cubes
				if (liveMatrix[i][j][k] == 1) 
					cubeMatrix[i][j][k].render();
			}
		}
	}
}

int GameOfLife3D::Neighbours(int i, int j, int k) {
	int neighbours = 0;

	neighbours += this->liveMatrix[i - 1][j - 1][k - 1];
	neighbours += this->liveMatrix[i - 1][j - 1][k - 0];
	neighbours += this->liveMatrix[i - 1][j - 1][k + 1];
	neighbours += this->liveMatrix[i - 1][j - 0][k - 1];
	neighbours += this->liveMatrix[i - 1][j - 0][k - 0];
	neighbours += this->liveMatrix[i - 1][j - 0][k + 1];
	neighbours += this->liveMatrix[i - 1][j + 1][k - 1];
	neighbours += this->liveMatrix[i - 1][j + 1][k - 0];
	neighbours += this->liveMatrix[i - 1][j + 1][k + 1];

	neighbours += this->liveMatrix[i - 0][j - 1][k - 1];
	neighbours += this->liveMatrix[i - 0][j - 1][k - 0];
	neighbours += this->liveMatrix[i - 0][j - 1][k + 1];
	neighbours += this->liveMatrix[i - 0][j - 0][k - 1];
	//neighbours += this->liveMatrix[i - 0][j - 0][k - 0]; // Do not count itself
	neighbours += this->liveMatrix[i - 0][j - 0][k + 1];
	neighbours += this->liveMatrix[i - 0][j + 1][k - 1];
	neighbours += this->liveMatrix[i - 0][j + 1][k - 0];
	neighbours += this->liveMatrix[i - 0][j + 1][k + 1];

	neighbours += this->liveMatrix[i + 1][j - 1][k - 1];
	neighbours += this->liveMatrix[i + 1][j - 1][k - 0];
	neighbours += this->liveMatrix[i + 1][j - 1][k + 1];
	neighbours += this->liveMatrix[i + 1][j - 0][k - 1];
	neighbours += this->liveMatrix[i + 1][j - 0][k - 0];
	neighbours += this->liveMatrix[i + 1][j - 0][k + 1];
	neighbours += this->liveMatrix[i + 1][j + 1][k - 1];
	neighbours += this->liveMatrix[i + 1][j + 1][k - 0];
	neighbours += this->liveMatrix[i + 1][j + 1][k + 1];

	return neighbours;
}
void GameOfLife3D::Update() {

	if (generation_count++ % update_interval == 0) {
		UpdateCPU();
		//UpdateCUDA();
	}
}

void GameOfLife3D::UpdateCPU() {
	int *** nextLiveMatrix = new int**[xSize];

	for (int i = 0; i < xSize; i++) {
		nextLiveMatrix[i] = new int*[ySize];
		for (int j = 0; j < ySize; j++) {
			nextLiveMatrix[i][j] = new int[zSize];
			for (int k = 0; k < zSize; k++) {
				nextLiveMatrix[i][j][k] = 0;
			}
		}
	}

	for (int i = 1; i < xSize - 1; i++) {
		for (int j = 1; j < ySize - 1; j++) {
			for (int k = 1; k < zSize - 1; k++){

				// Apply rules
				int neighbours = Neighbours(i, j, k);

				if (4 <= neighbours && neighbours <= 5)
					nextLiveMatrix[i][j][k] = this->liveMatrix[i][j][k];

				if (5 <= neighbours && neighbours <= 5) {
					nextLiveMatrix[i][j][k] = 1;
				}
			}
		}
	}

	this->liveMatrix = nextLiveMatrix;
}


void GameOfLife3D::UpdateCUDA() {
	// TODO Should I?
}