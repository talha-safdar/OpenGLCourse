#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
				Camera();
				Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

				void keyControl(bool* keys, GLfloat deltaTime);
				void mouseControl(GLfloat xChange, GLfloat yChange, GLfloat deltaTime);

				glm::mat4 calculateViewMatrix();

				~Camera();

private:
				glm::vec3 position;
				glm::vec3 front; // updated by pitch
				glm::vec3 up;
				glm::vec3 right; // updated by pitch
				glm::vec3 worldUp;

				GLfloat yaw; // left and right
				GLfloat pitch; // up and down

				GLfloat moveSpeed;
				GLfloat turnSpeed;

				void update();
};
