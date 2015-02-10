#include "quad.h"

#include <iostream>

Quad::Quad() {
	
	float my_points[] = {
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	float my_colours[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.7f, 0.0f, 0.5f
	};

	for (int i = 0; i < 12; i++) {
		points[i] = my_points[i];
		colours[i] = my_colours[i];
	}

	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo); 
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), points, GL_STATIC_DRAW);

	GLuint colours_vbo = 0;
	glGenBuffers(1, &colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), colours, GL_STATIC_DRAW);

	// VAO (will contain points and colors)
	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	const char* vertex_shader =
		"#version 400\n"
		"layout(location = 0) in vec3 vertex_position;"
		"layout(location = 1) in vec3 vertex_colour;"
		"out vec3 colour;"
		"uniform mat4 model;"
		"void main () {"
		"colour = vertex_colour;"
		"gl_Position =  model * vec4(vertex_position, 1.0);" 
		"}";
	const char* fragment_shader =
		"#version 400\n"
		"in vec3 colour;"
		"out vec4 frag_colour;"
		"void main () {"
		"frag_colour = vec4(colour, 1.0);"
		"}";

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, fs);
	glAttachShader(shaderProgram, vs);

	// insert location binding code here
	glBindAttribLocation(shaderProgram, 0, "vertex_position");
	glBindAttribLocation(shaderProgram, 1, "vertex_colour");

	glLinkProgram(shaderProgram);
}

void Quad::translate(glm::vec3 translation) {
	model = glm::translate(model, translation);
}

void Quad::render() {
	// Transform
	GLint uniModel = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	// Bind
	glUseProgram(shaderProgram);
	glBindVertexArray(vao);

	// Currently bound VAO with current shader
	glDrawArrays(GL_QUADS, 0, 4);

	// Print transform
	//const float *pSource = (const float*)glm::value_ptr(trans);
	//for (int i = 0; i < 16; ++i) {
	//	if (i % 4 == 0)
	//		std::cout << std::endl;
	//	std::cout << pSource[i] << " ";
	//}

}
