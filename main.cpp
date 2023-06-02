#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h> // for reproting errors nothing to do with opengl
#include <cmath> // for mathematical operators
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // for raw values

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"
#include "Material.h"

const float roRadians = 3.14159265f / 180.0f; // scale between ranges to convert degree to number

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;

Material shinyMaterial; // high intensity
Material dullMaterial; // low shininess

Light mainLight;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// buffers
// VAO=vertex array object holds mutiple VBOs to defie how to draw
// VBO=vertex buffer object
// IBO=Index buffer object

// Vertex Shader
// clamp is used if the number is beyond zero put it between 0 and 1
// model = model itself in space
// view = camera relative to everyehting
// proejction = how the camera trasnelts into what it sees
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
				// loop through every single triangle
				for (size_t i = 0; i < indiceCount; i += 3)
				{
								unsigned int in0 = indices[i] * vLength;
								unsigned int in1 = indices[i + 1] * vLength;
								unsigned int in2 = indices[i + 2] * vLength;
								glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
								glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
								glm::vec3 normal = glm::cross(v1, v2);
								normal = glm::normalize(normal);

								in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
								vertices[in0] += normal.x;	vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
								vertices[in1] += normal.x;	vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
								vertices[in2] += normal.x;	vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
				}

				for (size_t i = 0; i < verticeCount / vLength; i++)
				{
								unsigned int nOffset = i * vLength + normalOffset;
								glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
								vec = glm::normalize(vec);
								vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
				}
}

void CreateObjects() 
{

				// for drawing vertices by using vertices[] by indexing them
				unsigned int indices[] = {
								0, 3, 1,
								1, 3, 2, 
								2, 3, 0,
								0, 1, 2
				};

				GLfloat vertices[] = {
				//  x						y		 			z									u=bot-left			v=top-right				nx				ny				nz						(n=normal)
								-1.0f, -1.0f, -0.6f,					0.0f,							 0.0f,										0.0f, 0.0f, 0.0f,
								 0.0f,	-1.0f, 1.0f,					0.5f,							 0.0f,										0.0f, 0.0f, 0.0f,
								 1.0f, -1.0f, 0.6f,					1.0f,								0.0f,										0.0f, 0.0f, 0.0f,
								 0.0f,  1.0f, 0.0f,					0.5f,							 1.0f,										0.0f, 0.0f, 0.0f
				};

				calcAverageNormals(indices, 12, vertices, 32, 8, 5);

				Mesh* obj1 = new Mesh();
				obj1->CreateMesh(vertices, indices, 32, 12);
				meshList.push_back(obj1);

				Mesh* obj2 = new Mesh();
				obj2->CreateMesh(vertices, indices, 32, 12);
				meshList.push_back(obj2);
}

void CreateShaders()
{
				Shader* shader1 = new Shader();
				shader1->CreateFromFiles(vShader, fShader);
				shaderList.push_back(*shader1);
}

int main()
{
				mainWindow = Window(1366, 768);
				mainWindow.initialise();

				CreateObjects();
				CreateShaders();

				/*
				* You can write flat before "out" and "in" in the shaders
				* to calculate the lighting from one angle so no interploation
				* which means the whole flat triangle surface is lightes not efficient for spheres
				*/

				camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 18.0f);

				brickTexture = Texture((char*) "Textures/brick.png"); // pull texture
				brickTexture.LoadTexture(); // load to GPU
				dirtTexture = Texture((char*)"Textures/dirt.png"); // pull texture
				dirtTexture.LoadTexture(); // load to GPU

				shinyMaterial = Material(1.0f, 32); // 32 is comming for average shinienss
				dullMaterial = Material(0.3f, 4);

				mainLight = Light(1.0f, 1.0f, 1.0f, 0.1f, 
																						2.0f, -1.0f, -2.0f, 0.3f); // R, G, B, aIntesity

				GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
								   uniformAmbientIntensity = 0, uniformAmbientColour = 0, uniformDirection = 0, uniformDiffuseIntensity = 0,
											uniformSpecularIntensity = 0, uniformShininess = 0;
				glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

				// Loop until window closed
				while (!mainWindow.getShouldClose()) // whe nclicking on X button i the window bar (which window should close)
				{
								GLfloat now = glfwGetTime();
								deltaTime = now - lastTime;
								lastTime = now;

								// Get + handle user input events
								glfwPollEvents(); // check if any event happened (e.g. mosue, keyboard and window moving/resizing)

								// check for key presses for camera
								camera.keyControl(mainWindow.getKeys(), deltaTime);
								camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange(), deltaTime);

								// Clear colors in window and depth buffer bit
								glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // wipe screen of colours, fresh frame, set colour too
								glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // each pixel has more information than colour (light, shade)

								shaderList[0].UseShader();
								uniformModel = shaderList[0].GetModelLocation();
								uniformProjection = shaderList[0].GetProjectionLocation();
								uniformView = shaderList[0].GetViewLocation();
								uniformAmbientColour = shaderList[0].GetAmbientColourLocation();
								uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();
								uniformDirection = shaderList[0].GetDirectionLocation();
								uniformDiffuseIntensity = shaderList[0].GetDiffuseIntensityLocation();
								uniformEyePosition = shaderList[0].GetEyePositionLocation();
								uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
								uniformShininess = shaderList[0].GetShininessLocation();

								mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColour, uniformDiffuseIntensity, uniformDirection);

								glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
								glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
								glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

								glm::mat4 model(1.0f);
								// if do translete first rotate then the model is rotate from the original pivot point hence it creates distortion
								// because it rotated after being moved away from the pivot point
								model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f)); // here can manipulate the matrix of the model
								// model = glm::rotate(model, curAngle * roRadians, glm::vec3(0.0f, 1.0f, 0.0f));
								// model = glm::scale(model, glm::vec3(0.4, 0.4f, 1.0f)); // twice as big

								glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
								brickTexture.UseTexture(); // anything drawn will use this texture
								shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
								meshList[0]->RenderMesh();

								// for second object
								model = glm::mat4(1.0f); // 1.0 is very important otherwise the model is not visible
								model = glm::translate(model, glm::vec3(0.0f, 3.0f, -2.5f));
								// model = glm::scale(model, glm::vec3(0.4, 0.4f, 1.0f)); // twice as big
								glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
								dirtTexture.UseTexture(); // anything drawn will use this texture
								dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
								meshList[1]->RenderMesh();

								glUseProgram(0);

								mainWindow.swapBuffers(); // 2 buffers the front is shown the back is hidden, it is cycle
				}
				return 0;
}