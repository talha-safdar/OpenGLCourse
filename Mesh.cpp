#include "Mesh.h"

Mesh::Mesh()
{
				VAO = 0;
				VBO = 0;
				IBO = 0;
				indexCount = 0;
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
				indexCount = numOfIndices;

				glGenVertexArrays(1, &VAO); // create VEO: (amount of array, ID of array)
				glBindVertexArray(VAO); // binding

				glGenBuffers(1, &IBO);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

				glGenBuffers(1, &VBO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW); // static values won't change

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0); // last arguement from which element to start in the array
				glEnableVertexAttribArray(0); // location with 0
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)sizeof(vertices[0] * 3)); // 0=position 1=texture coordinate
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
				glEnableVertexAttribArray(2);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				glBindVertexArray(0);
}

void Mesh::RenderMesh()
{
				glBindVertexArray(VAO);
				// glDrawArrays(GL_TRIANGLES, 0, 3);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
				// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
				if (IBO == 0)
				{
								// delete the bufer of the GPU memory
								// for more space;
								glDeleteBuffers(1, &IBO); // improves performance
								IBO = 0;
				}
				if (VBO == 0)
				{
								// delete the bufer of the GPU memory
								// for more space;
								glDeleteBuffers(1, &VBO); // improves performance
								VBO = 0;
				}
				if (VAO == 0)
				{
								// delete the bufer of the GPU memory
								// for more space;
								glDeleteVertexArrays(1, &VAO); // improves performance
								VAO = 0;
				}

				// clear all the buffers

				indexCount = 0; 
}

// destructor
Mesh::~Mesh()
{
				// in case something goes wrong it clears the mesh
				ClearMesh();
}