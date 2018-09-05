#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace glfw
{
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

	private:
		CGLFWWindow();

		CGLFWWindow(const CGLFWWindow&) = delete;
		CGLFWWindow(CGLFWWindow&&) = delete;

		CGLFWWindow& operator =(const CGLFWWindow&) = delete;
		CGLFWWindow& operator =(CGLFWWindow&&) = delete;


	private:
		bool mInitialized;
		GLFWwindow * mWindow;
	};
}