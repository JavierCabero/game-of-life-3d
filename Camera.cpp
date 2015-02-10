/*
* From: https://classes.soe.ucsc.edu/cmps160/Spring13/projects/mesajaco/CMPS160_Final/code/Voxelish/Voxelish/Camera.cpp
*/
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <SFML/Window.hpp>

#include "Camera.h"

#include <iostream>

Camera::Camera(sf::Window* _window) {
	angle    = glm::vec2(M_PI_2, M_PI_2);
	position = glm::vec3(1.2, 1.2, 1.2);
	velocity = glm::vec3(0, 0, 0);
	accel    = glm::vec3(0, 0, 0);

	// Window
	window = _window;
	sf::Vector2u wSize = window->getSize();
	width = wSize.x;
	height = wSize.y;
}

Camera::~Camera(void) {
}

glm::vec3 Camera::Position(void) {
	return position;
}

void Camera::handleInput(void) {
	//Keyboard movement
	accel = glm::vec3(0, 0, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		accel.y += cos(angle.x);
		accel.x += sin(angle.x);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		accel.y -= cos(angle.x);
		accel.x -= sin(angle.x);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		accel.y += cos(angle.x - M_PI_2);
		accel.x += sin(angle.x - M_PI_2);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		accel.y -= cos(angle.x - M_PI_2);
		accel.x -= sin(angle.x - M_PI_2);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		accel.z += 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		accel.z -= 1;
	}
	accel *= 48;

	//Mouse camera control (move first, and handle bounds?)

	sf::Vector2i center = sf::Vector2i(width / 2, height / 2);
	sf::Vector2i offset = sf::Mouse::getPosition(*window) - center;
	//printf("%d %d %d %d\n", center.x, center.y, offset.x, offset.y);

	float mouse_speed = 0.001;
	angle.y += offset.x * mouse_speed;
	angle.x -= offset.y * mouse_speed;

	sf::Mouse::setPosition(center, *window);
}


void Camera::Update(sf::Time elapsed) {
	handleInput();
	float time = elapsed.asSeconds();

	// Do not confuse with velocity.length() <- Number of vector components
	float speed = length(velocity);

	// Calculate "air resistance"
	glm::vec3 friction(0.0, 0.0, 0.0);

	if (speed != 0) 
		friction = -glm::normalize(velocity) * (speed * speed * 4.0f);
	
	// Update velocity
	velocity += (friction + accel) * time;

	// Cap velocity
	if (velocity.length() > 40.0f) {
		velocity = glm::normalize(velocity);
		velocity *= 40.0f;
	}

	// Update position
	position += velocity * time;

	// Update view matrix so objects can use it
	view = viewMatrix();
}

void Camera::SetViewportSize(int w, int h) {
	width = w;
	height = h;
}

glm::vec3 Camera::lookAt(void) {
	float maxAngle = M_PI_2 - M_PI_2 / 4.0f;
	float minAngle = -maxAngle;

	if (angle.y > maxAngle) {
		angle.y = maxAngle;
	}
	else if (angle.y < minAngle) {
		angle.y = minAngle;
	}
	glm::vec3 vLookAt;
	vLookAt.x = sin(angle.x) * cos(angle.y);
	vLookAt.y = sin(angle.y);
	vLookAt.z = cos(angle.x) * cos(angle.y);
	return vLookAt;
}

glm::mat4 Camera::viewMatrix(void) {
	return glm::lookAt(position, position + lookAt(), glm::vec3(0.0f, 0.0f, 1.0f));
}

glm::mat4 Camera::projectionMatrix(void) {
	//TODO: Might want to adjust field of view depending on velocity
	return glm::perspective(
		45.0f,
		(float)width / (float)height,
		0.1f,
		512.0f
		);
}