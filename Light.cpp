#include "Light.h"

Light::Light()
{
				colour = glm::vec3(1.0f, 1.0f, 1.0f); // the amount of the colour to be shown in terms of lighting
				ambientIntensity = 1.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity)
{
				colour = glm::vec3(red, green, blue); // culd use clamping to ensure between 0 and 1
				ambientIntensity = aIntensity;
}

void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation)
{
				glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
				glUniform1f(ambientIntensityLocation, ambientIntensity);
}

Light::~Light()
{

}