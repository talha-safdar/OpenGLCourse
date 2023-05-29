#include "Light.h"

Light::Light()
{
				colour = glm::vec3(1.0f, 1.0f, 1.0f); // the amount of the colour to be shown in terms of lighting
				ambientIntensity = 1.0f;

				direction = glm::vec3(0.0f, -1.0f, 0.0f); // arrow pointing straight down (for basing calcualtions)
				diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity)
{
				colour = glm::vec3(red, green, blue); // culd use clamping to ensure between 0 and 1
				ambientIntensity = aIntensity;

				direction = glm::vec3(xDir, yDir, zDir);
				diffuseIntensity = dIntensity;
}

void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation)
{
				glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
				glUniform1f(ambientIntensityLocation, ambientIntensity);

				glUniform3f(directionLocation, direction.x, direction.y, direction.z);
				glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}



Light::~Light()
{

}