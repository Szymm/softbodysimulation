#include "CGLFWWindow.h"

#include <iostream>

// -------------------------------------------------------------------------------------------------
glfw::CGLFWWindow::CGLFWWindow()
	: mInitialized(false)
	, mWindow(nullptr)
{
}

// -------------------------------------------------------------------------------------------------
bool glfw::CGLFWWindow::Init(const int w, const int h, const char * title)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mWindow = glfwCreateWindow(w, h, title, NULL, NULL);
	if (mWindow == nullptr)
	{
		Destroy();
		return false;
	}

	glfwMakeContextCurrent(mWindow);
	//glfwSetFramebufferSizeCallback(mWindow, );

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		Destroy();
		return false;
	}

	return true;
}

// -------------------------------------------------------------------------------------------------
void glfw::CGLFWWindow::Destroy()
{
	glfwTerminate();
}

// -------------------------------------------------------------------------------------------------
bool glfw::CGLFWWindow::ShouldClose()
{
	if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(mWindow, true);
		return true;
	}

	return false;
}

// -------------------------------------------------------------------------------------------------
void glfw::CGLFWWindow::Draw()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

// -------------------------------------------------------------------------------------------------
void glfw::CGLFWWindow::SwapBuffers()
{
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
}
