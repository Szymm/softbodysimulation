#include "GLFWWindow.h"

#include <iostream>

// -------------------------------------------------------------------------------------------------
glfw::CGLFWWindow::CGLFWWindow()
	: mWindow(nullptr)
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

	glfwSetWindowUserPointer(mWindow, this);
	glfwSetKeyCallback(mWindow, key_callback);
	glfwSetCursorPosCallback(mWindow, cursor_position_callback);
	glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);

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

// -------------------------------------------------------------------------------------------------
void glfw::CGLFWWindow::AddKeyListener(IKeyInputListener * pListener)
{
	mKeyInputListeners.push_back(pListener);
}

// -------------------------------------------------------------------------------------------------
void glfw::CGLFWWindow::AddMouseListener(IMouseInputListener * pListener)
{
	mMouseInputListeners.push_back(pListener);
}

// -------------------------------------------------------------------------------------------------
void glfw::CGLFWWindow::RemoveKeyListener(IKeyInputListener * pListener)
{
	auto it = std::find(mKeyInputListeners.begin(), mKeyInputListeners.end(), pListener);
	if (it != mKeyInputListeners.end())
	{
		mKeyInputListeners.erase(it);
	}
}

// -------------------------------------------------------------------------------------------------
void glfw::CGLFWWindow::RemoveMouseListener(IMouseInputListener * pListener)
{
	auto it = std::find(mMouseInputListeners.begin(), mMouseInputListeners.end(), pListener);
	if (it != mMouseInputListeners.end())
	{
		mMouseInputListeners.erase(it);
	}
}

// -------------------------------------------------------------------------------------------------
void glfw::CGLFWWindow::OnKey(int key, int scancode, int action, int mods)
{
	std::vector<IKeyInputListener*>::iterator it;
	for (it = mKeyInputListeners.begin(); it != mKeyInputListeners.end(); ++it)
	{
		(*it)->OnKey(key, scancode, action, mods);
	}
}

// -------------------------------------------------------------------------------------------------
void glfw::CGLFWWindow::OnCursorPosition(double xpos, double ypos)
{
	std::vector<IMouseInputListener*>::iterator it;
	for (it = mMouseInputListeners.begin(); it != mMouseInputListeners.end(); ++it)
	{
		(*it)->OnCursorPosition(xpos, ypos);
	}
}

// -------------------------------------------------------------------------------------------------
void glfw::CGLFWWindow::OnScroll(double xoffset, double yoffset)
{
	std::vector<IMouseInputListener*>::iterator it;
	for (it = mMouseInputListeners.begin(); it != mMouseInputListeners.end(); ++it)
	{
		(*it)->OnScroll(xoffset, yoffset);
	}
}

// -------------------------------------------------------------------------------------------------
void glfw::CGLFWWindow::OnFramebufferSize(int width, int height)
{
	glViewport(0, 0, width, height);
}
