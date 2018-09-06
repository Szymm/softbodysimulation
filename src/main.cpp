#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <GLFWWindow.h>
#include <Shader.h>
#include <Physics.h>
#include <Camera.h>

#include <iostream>

#define window glfw::CGLFWWindow::getInstance()

CPhysicsSystem g_physics;

double PrevTime;
double CurrentTime;

const int SCR_WIDTH = 1440;
const int SCR_HEIGHT = 900;

int main()
{
	if (!window.Init(SCR_WIDTH, SCR_HEIGHT, "SoftBodySimulation"))
	{
		return -1;
	}

	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
	CurrentTime = glfwGetTime();
	PrevTime = CurrentTime;

	// Rope
	//g_physics.CreateRope(0.02f, 10);
	//std::vector<glm::vec3>& mPositions = g_physics.GetMassPointPositions();

	float vertices[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// Render data
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mPositions.size(), &mPositions[0].x, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	Shader shader("../shaders/shader.vs", "../shaders/shader.fs");

	while (!window.ShouldClose())
	{
		CurrentTime = glfwGetTime();
		double dt = CurrentTime - PrevTime;
		PrevTime = CurrentTime;

		g_physics.Update(dt);
		camera.Update(dt);

		shader.use();

		// projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("view", view);

		// model matrix
		glm::mat4 model = glm::mat4(1.0);
		shader.setMat4("model", model);

		for (int i = 0; i < 4; i++)
		{
			glm::vec4 pos = glm::vec4();
			pos.x = vertices[0 + i * 3];
			pos.y = vertices[1 + i * 3];
			pos.z = vertices[2 + i * 3];
			pos.w = 1.0f;

			glm::vec4 screenPos = projection * view * model * pos;
			std::cout << "\nv: " << (i+1) << ",x:" << pos.x << "y:" << pos.y << "z:" << pos.z;
		}

		window.Draw();
		
		//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * mPositions.size(), &mPositions[0].x);

		glBindVertexArray(VAO);
		//glDrawArrays(GL_LINE_STRIP, 0, mPositions.size());
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		window.SwapBuffers();

	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);

	window.Destroy();
	
	return 0;
}