#ifndef GAME_OF_LIFE_3D_H
#define GAME_OF_LIFE_3D_H

#include "Cube.h"

class GameOfLife3D {

private:
	int generation_count;
	int update_interval;
	int xSize;
	int ySize;
	int zSize;
	Cube *** cubeMatrix;
	int  *** liveMatrix;
	int Neighbours(int i, int k, int j);
	void UpdateCPU();
	void UpdateCUDA();
public:
	GameOfLife3D(int xSize, int ySize, int zSize);
	void Update();
	void Render();
};


#endif