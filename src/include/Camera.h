#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <GLFWWindow.h>

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera : public glfw::IKeyInputListener, public glfw::IMouseInputListener
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	int MovingForward;
	int MovingBackward;
	int MovingLeft;
	int MovingRight;

	int LastMouseX;
	int LastMouseY;
	bool FirstMouse;

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) 
		: Front(glm::vec3(0.0f, 0.0f, -1.0f))
		, MovementSpeed(SPEED)
		, MouseSensitivity(SENSITIVITY)
		, Zoom(ZOOM)
		, MovingForward(0)
		, MovingBackward(0)
		, MovingLeft(0)
		, MovingRight(0)
		, FirstMouse(true)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();

		glfw::CGLFWWindow::getInstance().AddKeyListener(this);
		glfw::CGLFWWindow::getInstance().AddMouseListener(this);
	}
	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
		: Front(glm::vec3(0.0f, 0.0f, -1.0f))
		, MovementSpeed(SPEED)
		, MouseSensitivity(SENSITIVITY)
		, Zoom(ZOOM)
		, MovingForward(0)
		, MovingBackward(0)
		, MovingLeft(0)
		, MovingRight(0)
		, FirstMouse(true)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();

		glfw::CGLFWWindow::getInstance().AddKeyListener(this);
		glfw::CGLFWWindow::getInstance().AddMouseListener(this);
	}

	// Destructor
	~Camera()
	{
		glfw::CGLFWWindow::getInstance().RemoveKeyListener(this);
		glfw::CGLFWWindow::getInstance().RemoveMouseListener(this);
	}

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	//
	void Update(float dt)
	{
		float v = MovementSpeed * dt;
		if (MovingForward && !MovingBackward)
			Position += Front * v;
		if (MovingBackward && !MovingForward)
			Position -= Front * v;
		if (MovingLeft && !MovingRight)
			Position -= Right * v;
		if (MovingRight && !MovingLeft)
			Position += Right * v;
	}

	// 
	virtual void OnKey(int key, int scancode, int actions, int mods)
	{
		if (key == GLFW_KEY_D)
		{
			MovingRight = actions > 0;
		}
		else if (key == GLFW_KEY_A)
		{
			MovingLeft = actions > 0;
		}
		else if (key == GLFW_KEY_W)
		{
			MovingForward = actions > 0;
		}
		else if (key == GLFW_KEY_S)
		{
			MovingBackward = actions > 0;
		}
	}

	virtual void OnCursorPosition(double xPos, double yPos)
	{
		GLboolean constrainPitch = true;

		if (FirstMouse)
		{
			LastMouseX = xPos;
			LastMouseY = yPos;
			FirstMouse = false;
		}

		float xoffset = xPos - LastMouseX;
		float yoffset = LastMouseY - yPos; // reversed since y-coordinates go from bottom to top

		LastMouseX = xPos;
		LastMouseY = yPos;

		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	virtual void OnScroll(double xoffset, double yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
};

#endif