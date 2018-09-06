#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace glfw
{
	// -----------------------------------------------------------------------------------------
	class IKeyInputListener
	{
	public:
		virtual void OnKey(int key, int scancode, int actions, int mods) = 0;
	};

	// -----------------------------------------------------------------------------------------
	class IMouseInputListener
	{
	public:
		virtual void OnCursorPosition(double xPos, double yPos) = 0;
		virtual void OnScroll(double xoffset, double yoffset) = 0;
	};

	// -----------------------------------------------------------------------------------------
	class CGLFWWindow
	{
	public:
		static CGLFWWindow& getInstance()
		{
			static CGLFWWindow instance;
			return instance;
		}

		bool Init(const int w, const int h, const char* title);
		void Destroy();

		bool ShouldClose();
		void Draw();
		void SwapBuffers();

		void AddKeyListener(IKeyInputListener* pListener);
		void AddMouseListener(IMouseInputListener* pListener);
		void RemoveKeyListener(IKeyInputListener* pListener);
		void RemoveMouseListener(IMouseInputListener* pListener);

	protected:
		virtual void OnKey(int key, int scancode, int actions, int mods);
		virtual void OnCursorPosition(double xPos, double yPos);
		virtual void OnScroll(double xoffset, double yoffset);
		virtual void OnFramebufferSize(int w, int h);

		std::vector<IKeyInputListener*>		mKeyInputListeners;
		std::vector<IMouseInputListener*>	mMouseInputListeners;

	private:
		CGLFWWindow();

		CGLFWWindow(const CGLFWWindow&) = delete;
		CGLFWWindow(CGLFWWindow&&) = delete;

		CGLFWWindow& operator =(const CGLFWWindow&) = delete;
		CGLFWWindow& operator =(CGLFWWindow&&) = delete;


	private:
		GLFWwindow * mWindow;

		// glfw callbacks
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			CGLFWWindow* pWindow = (CGLFWWindow*)glfwGetWindowUserPointer(window);
			pWindow->OnKey(key, scancode, action, mods);
		}

		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
		{
			CGLFWWindow* pWindow = (CGLFWWindow*)glfwGetWindowUserPointer(window);
			pWindow->OnCursorPosition(xpos, ypos);
		}

		static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
		{
			CGLFWWindow* pWindow = (CGLFWWindow*)glfwGetWindowUserPointer(window);
			pWindow->OnFramebufferSize(width, height);
		}
	
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
		{
			CGLFWWindow* pWindow = (CGLFWWindow*)glfwGetWindowUserPointer(window);
			pWindow->OnScroll(xoffset, yoffset);
		}
	};
}