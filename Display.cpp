#include "Display.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

float WIDTH  = 1000;
float HEIGHT = 1000;

// View 
glm::mat4 view = glm::lookAt(
	glm::vec3(1.2f, 1.2f, 1.2f),
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f)
	);

// Projection 
glm::mat4 proj = glm::perspective(45.0f, WIDTH / HEIGHT, 1.0f, 1000.0f);