#include "Camera.h"

Camera::Camera() 
{
				position = glm::vec3(0.0f, 0.0f, 0.0f);
				front = glm::vec3(0.0f, 0.0f, -1.0f);
				worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
				right = glm::normalize(glm::cross(front, worldUp));
				up = glm::normalize(glm::cross(right, front));

				yaw = 0.0f;
				pitch = 0.0f;
				moveSpeed = 0.01f;
				turnSpeed = 0.0f;

}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
				position = startPosition;
				worldUp = startUp;
				yaw = startYaw;
				pitch = startPitch;

				front = glm::vec3(0.0f, 0.0f, -1.0f);

				right = glm::normalize(glm::cross(front, worldUp));
				up = glm::normalize(glm::cross(right, front));

				moveSpeed = startMoveSpeed;
				turnSpeed = startTurnSpeed;

				update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
				GLfloat velocity = moveSpeed * deltaTime;

				if (keys[GLFW_KEY_W])
				{
								position += front * velocity;
				}
				if (keys[GLFW_KEY_S])
				{
								position -= front * velocity;
				}
				if (keys[GLFW_KEY_A])
				{
								position -= right * velocity;
				}
				if (keys[GLFW_KEY_D])
				{
								position += right * velocity;
				}
				if (keys[GLFW_KEY_E])
				{
								position += up * velocity;
				}
				if (keys[GLFW_KEY_Q])
				{
								position -= up * velocity;
				}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange, GLfloat deltaTime)
{
				// xChange *= turnSpeed * deltaTime;
				// yChange *= turnSpeed * deltaTime;

				yaw += xChange;
				pitch += yChange;

				if (pitch > 89.0f)
				{
								pitch = 89.0f;
				}

				if (pitch < -89.0f)
				{
								pitch = -89.0f;
				}

				update();
}

glm::mat4 Camera::calculateViewMatrix()
{
				// caluclate the matrix of rotation 
				// arg=origin, arg=look at, arg=where is the up
				return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition()
{
				return position;
}

void Camera::update()
{
				glm::vec3 newFront;
				newFront.x = cos(glm::radians(yaw));
				newFront.y = sin(glm::radians(pitch));
				newFront.z = sin(glm::radians(yaw));

				front = glm::normalize(newFront);

				right = glm::normalize(glm::cross(front, worldUp));
				up = glm::normalize(glm::cross(right, front));
}


Camera::~Camera()
{
}
