#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
				Window();
				Window(GLint windowWidth, GLint windowHeight);

				int initialise();

				GLfloat getBufferWidth() { return bufferWidth; }
				GLfloat getBufferHeight() { return bufferHeight; }

				bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

				bool* getKeys() { return keys; }
				GLfloat getXChange();
				GLfloat getYChange();

				void swapBuffers() { glfwSwapBuffers(mainWindow); }

				~Window();

private:
				GLFWwindow* mainWindow;

				GLint width, height;
				GLint bufferWidth, bufferHeight;

				bool keys[1024]; // 1024 max number of ASCII keys

				GLfloat lastX;
				GLfloat lastY; // last coordinate
				GLfloat xChange;
				GLfloat yChange; // calculate the chnange
				bool mouseFirstMoved; // initialiastion movement

				void createCallbacks();
				static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode); // bcz need callback and only static are allowed
				static void handleMouse(GLFWwindow* window, double xPos, double yPos); // bcz need callback and only static are allowed
};

