#include "Cube.h"

Cube::Cube() : Cube(glm::vec3(0.0, 0.0, 0.0)) {}

Cube::Cube(glm::vec3 center) {

	// Apply translation
	model = glm::translate(model, center);
}

void Cube::render() {

	// Bind model matrix
	GLint uniModel = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	
	// Draw each face of the cube
	for (int i = 0; i < 24; i += 4) {
		glDrawArrays(GL_QUADS, i, i+4);
	}
}