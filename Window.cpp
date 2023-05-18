#include "Window.h"

Window::Window()
{
				width = 800;
				height = 600;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
				width = windowWidth;
				height = windowHeight;
}

int Window::initialise()
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

				// create the window
				mainWindow = glfwCreateWindow(width, height, "OpenGLCourse", NULL, NULL);
				if (!mainWindow)
				{
								printf("GLFW window creating failed!");
								glfwTerminate();
								return 1;
				}

				// Get buffer size information
				glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight); // get the window's sizes and store into variable address

				// Set context for GLEW to use
				// tell GLEW: if multiple windows you can switch between them
				glfwMakeContextCurrent(mainWindow);

				// allow modern extension features
				glewExperimental = GL_TRUE; // accessing experimental stuff (enables them)

				if (glewInit() != GLEW_OK) // if GLEW features are false
				{
								// printf("GLW initialization failed!");
								printf("Error: % s", glewGetErrorString(glewInit()));
								glfwDestroyWindow(mainWindow); // GLFW remove window
								glfwTerminate(); // GLFW teriminate process
								return 1; // return error
				}

				// depth test is impoartant
				glEnable(GL_DEPTH_TEST);

				// Setup viewport size (like canvas in JavaFX, the drawable part inside the window)
				// use buffer size because it obtaines the exact inside window size
				glViewport(0, 0, bufferWidth, bufferHeight);
}

Window::~Window()
{
				glfwDestroyWindow(mainWindow); // GLFW remove window
				glfwTerminate(); // GLFW teriminate process
}
