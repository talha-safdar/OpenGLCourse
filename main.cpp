#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

int main()
{
				// initialize GLFW
				if (glfwInit() == false) // if it fails
				{
								printf("GLFW initialization failed!"); // message error
								glfwTerminate(); // terminate it
								return 1; // standard 1=error, 2=kernel
				}

				// Setup GLFW window proeprties
				// OpenGL version
				// MAX and MIN verison
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

				// stable and NOT backward compatible profile (depecrated or removed features)
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
				
				// make it forward compatible
				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

				GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "OpenGLCourse", NULL, NULL);
				if (!mainWindow)
				{
								printf("GLFW window creating failed!");
								glfwTerminate();
								return 1;
				}

				// Get buffer size information
				int bufferWidth, bufferHeight; // for the window to get its size
				glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight); // get the window's sizes and store into variable address

				// Set context for GLEW to use
				// tell GLEW: if multiple windows you can switch between them
				glfwMakeContextCurrent(mainWindow);

				// allow modern extension features
				glewExperimental = GL_TRUE; // accessing experimental stuff (enables them)

				if (glewInit() != GLEW_OK) // if GLEW features are false
				{
								printf("GLW initialization failed!");
								glfwDestroyWindow(mainWindow); // GLFW remove window
								glfwTerminate(); // GLFW teriminate process
								return 1; // return error
				}

				// Setup viewport size (like canvas in JavaFX, the drawable part inside the window)
				// use buffer size because it obtaines the exact inside window size
				glViewport(0, 0, bufferWidth, bufferHeight);

				// Loop until window closed
				while (!glfwWindowShouldClose(mainWindow)) // whe nclicking on X button i the window bar (which window should close)
				{
								// Get + handle user input events
								glfwPollEvents(); // check if any event happened (e.g. mosue, keyboard and window moving/resizing)

								// Clear window
								glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // wipe screen of colours, fresh frame, set colour too
								glClear(GL_COLOR_BUFFER_BIT); // each pixel has more information than colour (light, shade)

								glfwSwapBuffers(mainWindow); // 2 buffers the front is shown the back is hidden, it is cycle
				}

				return 0;
}