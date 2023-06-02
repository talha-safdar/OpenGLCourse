#pragma once

#include <GL/glew.h>


class Material
{
public:
				Material();
				Material(GLfloat sIntensity, GLfloat shine);

				void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);


				~Material();

private:
				GLfloat specularIntensity; // brigthness of the lighting effect
				GLfloat shininess; // how much smooth the spread value
				
};

