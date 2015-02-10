#ifdef SFML_STATIC
#pragma comment(lib, "glew.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")  
#endif // SFML_STATIC

#include <SFML/Graphics.hpp>
#include <GL/glew.h>

#include <iostream>

#include "Quad.h"
#include "Cube.h"
#include "Display.h"
#include "Camera.h"

#include "GameOfLife3D.h"

// Prototypes
void bindCubeVAO();
void bindCubeShader();

int main(int argc, char* argv[])
{
	// SFML Window Init
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");
	window.setFramerateLimit(60); // Limit frame rate

	// Start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();
	
	printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));

	// OpenGL
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CW); // GL_CCW for counter clock-wise
	glEnable(GL_DEPTH_TEST);

	float k    = 0;
	float incr = 0.1f;
	
	// CPU Timing structures
	clock_t t1;
	clock_t t2;
	double frame_time;

	// Create quad
	//Quad quad;

	//Cube cube(glm::vec3(-.5, -.5, -.5));

	// Matrix
	int xSize = 80;
	int ySize = 80;
	int zSize = 80;

	if (argc == 3) {
		xSize = atoi(argv[0]);
		ySize = atoi(argv[1]);
		zSize = atoi(argv[2]);
	}

	GameOfLife3D gol3D(xSize, ySize, zSize);

	// Cursor not visisble
	window.setMouseCursorVisible(false);

	// Camera
	Camera camera(&window);

	// Timing
	sf::Clock clock;

	// This is the same for every cube
	// so we only have to do it once
	bindCubeVAO();
	bindCubeShader();

	// Main loop
	while (window.isOpen())
	{
		// Timing
		sf::Time elapsed = clock.restart();

		// Camera update
		camera.Update(elapsed);

		// Events (Display & Input)
		sf::Event event;
		while (window.pollEvent(event)) 
		{
			switch (event.type) {
			case sf::Event::Closed: // Close app
				window.close();
				exit(0);
				break;
			case sf::Event::KeyPressed: // Keyboard key

				// Close when ESCAPE is pressed
				if (event.text.unicode == 36) {
					window.close();
					exit(0);
				}

				// Print characters
				//if (event.text.unicode < 128) 
				//{
				//	char c = static_cast<char>(event.text.unicode);
				//	fprintf(stdout, "Char: %c (%i)\n", c+65, event.text.unicode);
				//}
				break;
			}
		}

		gol3D.Update();

		// Bind view matrix
		GLint uniView = glGetUniformLocation(shader, "view");
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

		// Bind projection matrix
		GLint uniProj = glGetUniformLocation(shader, "proj");
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render
		gol3D.Render();

		// Increment translation
		//quad.translate(glm::vec3(sin(k)/100, cos(k)/100, 0.0));

		// Paint it
		window.display();
	}

	return 0;
}

void bindCubeVAO() {
	float points[] = {
		// Base
		0.0, 0.0, 1.0,
		1.0, 0.0, 1.0,
		1.0, 0.0, 0.0,
		0.0, 0.0, 0.0,

		// Front
		0.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 0.0, 1.0,
		0.0, 0.0, 1.0,

		// Left
		0.0, 1.0, 0.0,
		0.0, 1.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 0.0,

		// Back
		0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 1.0, 0.0,
		0.0, 1.0, 0.0,

		// Right
		1.0, 0.0, 0.0,
		1.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 0.0,

		// Top
		1.0, 1.0, 0.0,
		1.0, 1.0, 1.0,
		0.0, 1.0, 1.0,
		0.0, 1.0, 0.0
	};

	float colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.7f, 0.0f, 0.5f,

		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.7f, 0.0f, 0.5f,

		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.7f, 0.0f, 0.5f,

		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.7f, 0.0f, 0.5f,

		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.7f, 0.0f, 0.5f,

		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.7f, 0.0f, 0.5f,

		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.7f, 0.0f, 0.5f
	};

	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 24 * sizeof(float), points, GL_STATIC_DRAW);

	GLuint colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 24 * sizeof(float), points, GL_STATIC_DRAW);

	// Vertex Array Object
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Bind geometry to VAO
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// Bind colors to VAO
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// Enable atribbutes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Bind geometry
	glBindVertexArray(vao);
}

void bindCubeShader() {
	const char* vertex_shader =
		"#version 400\n"
		"layout(location = 0) in vec3 vertex_position;"
		"layout(location = 1) in vec3 vertex_color;"
		"out vec3 color;"
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 proj;"
		"void main () {"
		"  color = vertex_color;"
		"  gl_Position = proj * view * model * vec4(vertex_position, 1.0);"
		"}";
	const char* fragment_shader =
		"#version 400\n"
		"out vec4 frag_colour;"
		"in vec3 color;"
		"void main () {"
		"  frag_colour = vec4 (color, 1.0);"
		"}";

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	shader = glCreateProgram();
	glAttachShader(shader, fs);
	glAttachShader(shader, vs);
	glLinkProgram(shader);

	// Bind shader
	glUseProgram(shader);
}