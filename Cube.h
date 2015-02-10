#ifndef CUBE_H
#define CUBE_H

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include "glm/ext.hpp" 

#include "Display.h"
#include "colors_shader.h"

class Cube {

private:
	// Transform
	glm::mat4 model;

public: 
	Cube();
	Cube(glm::vec3 center);
	void render();
};

#endif