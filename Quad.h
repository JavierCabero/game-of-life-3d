#ifndef QUAD_H
#define QUAD_H

// OpenGL Math library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include "glm/ext.hpp" // For printing matrices

#include <SFML/Graphics.hpp>
#include <GL/glew.h>


class Quad {

private:
	// Transform (later)
	glm::mat4 model;

	// Geometry
	float points[12];

	// Colors
	float colours[12];

	// VAO
	GLuint vao;

	// Shader
	GLuint shaderProgram;
	
public:

	Quad();
	void translate(glm::vec3 translation);
	void render();
};

#endif