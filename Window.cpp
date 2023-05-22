#include "Window.h"

Window::Window()
{
				width = 800;
				height = 600;
				mouseFirstMoved = false;

				for (size_t i = 0; i < 1024; i++)
				{
								keys[i] = 0;
				}

				xChange = 0.0f;
				yChange = 0.0f;

				mouseFirstMoved = true;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
				width = windowWidth;
				height = windowHeight;

				mouseFirstMoved = false;

				for (size_t i = 0; i < 1024; i++)
				{
								keys[i] = 0;
				}

				xChange = 0.0f;
				yChange = 0.0f;

				mouseFirstMoved = true;
}

int Window::initialise()
{
				xChange = 0.0f;
				yChange = 0.0f;

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
								printf("Error creating GLFW window!\n");
								glfwTerminate();
								return 1;
				}

				// Get buffer size information
				glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight); // get the window's sizes and store into variable address

				// Set context for GLEW to use
				// tell GLEW: if multiple windows you can switch between them
				glfwMakeContextCurrent(mainWindow);
				
				// Handle key + mouse input
				createCallbacks();
				glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // to hide the mourse cursor

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

				glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
				glfwSetKeyCallback(mainWindow, handleKeys);
				glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat Window::getXChange()
{
				GLfloat theChange = xChange;
				xChange = 0.0f;
				return theChange;
}

GLfloat Window::getYChange()
{
				GLfloat theChange = yChange;
				yChange = 0.0f;
				return theChange;
}

// key handler
void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
				Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window)); // so now we can acces the key through the static function

				if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				{
								glfwSetWindowShouldClose(window, GL_TRUE);
				}

				if (key >= 0 && key < 1024) // only valid keywords
				{
								if (action == GLFW_PRESS)
								{
												theWindow->keys[key] = true;
												// printf("Pressed: %d\n", key);
								}
								else if (action == GLFW_RELEASE)
								{
												theWindow->keys[key] = false;
												// printf("Released: %d\n", key);
								}
				}
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
				// access to our window
				Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window)); // so now we can acces the key through the static function
				
				if (theWindow->mouseFirstMoved)
				{
								theWindow->lastX = xPos;
								theWindow->lastY = yPos;
								theWindow->mouseFirstMoved = false;
				}

				theWindow->xChange = xPos - theWindow->lastX;
				theWindow->yChange =  xPos - theWindow->lastY; // you can switch the positions if inverteed preferred

				theWindow->lastX = xPos;
				theWindow->lastY = yPos;

				printf("x:%.6f, y:%.6f\n", theWindow->xChange, theWindow->yChange);
}

Window::~Window()
{
				glfwDestroyWindow(mainWindow); // GLFW remove window
				glfwTerminate(); // GLFW teriminate process
}
