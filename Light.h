#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
				Light();
				Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);

				void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation);
				
				~Light(); // dont need to call destroyer since there are no pointers used

private:
				glm::vec3 colour;
				GLfloat ambientIntensity;

				glm::vec3 direction;  // for diffuse light
				GLfloat diffuseIntensity;  // for diffuse light
};

