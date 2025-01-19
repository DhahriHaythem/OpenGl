#include<iostream>
#include<sstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "ProgramShader.h"
#include "Texture2D.h"

const char* APP_TITLE = "Introduction to Modern OpenGL - Hello Triangle";
int WINDOW_WIDTH = 1024;
int WINDOW_HEIGHT = 768;
GLFWwindow* pWindow = NULL;
bool gWireframe = false;
const std::string texture1Filename = "Textures/wooden_crate.jpg";

void glfw_Onkey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height);
void showFPS(GLFWwindow* window);
bool InitOpenGL();

int main()
{
	if (!InitOpenGL())
	{
		std::cerr << "GLFW initialization failed " << std::endl;
		return -1;
	}

	GLfloat vertices[] = {
		// position				// tex coords

		// front face
		-1.0f,  1.0f,  1.0f,	0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f,	1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,	1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f,	0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f,	0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f,	1.0f, 0.0f,

		 // back face
		 -1.0f,  1.0f, -1.0f,	0.0f, 1.0f,
		  1.0f, -1.0f, -1.0f,	1.0f, 0.0f,
		  1.0f,  1.0f, -1.0f,	1.0f, 1.0f,
		 -1.0f,  1.0f, -1.0f,	0.0f, 1.0f,
		 -1.0f, -1.0f, -1.0f,	0.0f, 0.0f,
		  1.0f, -1.0f, -1.0f,	1.0f, 0.0f,

		  // left face
		  -1.0f,  1.0f, -1.0f,	0.0f, 1.0f,
		  -1.0f, -1.0f,  1.0f,	1.0f, 0.0f,
		  -1.0f,  1.0f,  1.0f,	1.0f, 1.0f,
		  -1.0f,  1.0f, -1.0f,	0.0f, 1.0f,
		  -1.0f, -1.0f, -1.0f,	0.0f, 0.0f,
		  -1.0f, -1.0f,  1.0f,	1.0f, 0.0f,

		  // right face
		   1.0f,  1.0f,  1.0f,	0.0f, 1.0f,
		   1.0f, -1.0f, -1.0f,	1.0f, 0.0f,
		   1.0f,  1.0f, -1.0f,	1.0f, 1.0f,
		   1.0f,  1.0f,  1.0f,	0.0f, 1.0f,
		   1.0f, -1.0f,  1.0f,	0.0f, 0.0f,
		   1.0f, -1.0f, -1.0f,	1.0f, 0.0f,

		   // top face
		  -1.0f,  1.0f, -1.0f,	0.0f, 1.0f,
		   1.0f,  1.0f,  1.0f,	1.0f, 0.0f,
		   1.0f,  1.0f, -1.0f,	1.0f, 1.0f,
		  -1.0f,  1.0f, -1.0f,	0.0f, 1.0f,
		  -1.0f,  1.0f,  1.0f,	0.0f, 0.0f,
		   1.0f,  1.0f,  1.0f,	1.0f, 0.0f,

		   // bottom face
		  -1.0f, -1.0f,  1.0f,	0.0f, 1.0f,
		   1.0f, -1.0f, -1.0f,	1.0f, 0.0f,
		   1.0f, -1.0f,  1.0f,	1.0f, 1.0f,
		  -1.0f, -1.0f,  1.0f,	0.0f, 1.0f,
		  -1.0f, -1.0f, -1.0f,	0.0f, 0.0f,
		   1.0f, -1.0f, -1.0f,	1.0f, 0.0f,
	};

	glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, -5.0f);

	float cubeAngle = 0.0f;
	double lastTime = glfwGetTime();

	GLuint vbo, vao;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	ProgramShader programShader;
	programShader.loadShaders("ShadersFiles/basic.vert", "ShadersFiles/basic.frag");

	Texture2D texture;
	texture.loadTexture(texture1Filename, true);

	while (!glfwWindowShouldClose(pWindow))
	{
		showFPS(pWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		texture.bind();

		cubeAngle += (float)(deltaTime * 50.0f);
		if (cubeAngle >= 360.0) cubeAngle = 0.0f;

		glm::mat4 model, view, projection;
		model = view = projection = glm::mat4(1.0f);
		glm::vec3 camPos(0.0f, 0.0f, 0.0f);
		glm::vec3 targetPos(0.0f, 0.0f, -1.0f);
		glm::vec3 up(0.0f, 1.0f, 0.0f);

		model = glm::translate(model, cubePos) * glm::rotate(model, glm::radians(cubeAngle), up);

		view = glm::lookAt(camPos,targetPos,up);

		projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

		programShader.use();

		programShader.setUniform("model", model);
		programShader.setUniform("view", view);
		programShader.setUniform("projection", projection);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(pWindow);

		lastTime = currentTime;
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	//glDeleteBuffers(1, &ebo);

	glfwTerminate();
	return 0;
}

bool InitOpenGL()
{
	if (!glfwInit())
	{
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APP_TITLE, NULL, NULL);

	if (pWindow == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(pWindow);
	glfwSetKeyCallback(pWindow, glfw_Onkey);
	glfwSetFramebufferSizeCallback(pWindow, glfw_OnFrameBufferSize);

	GLboolean glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW initialization failed" << std::endl;
		return false;
	}
	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	glEnable(GL_DEPTH_TEST);

	return true;
}

void glfw_Onkey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		gWireframe = !gWireframe;
		if (gWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}

void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height)
{
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void showFPS(GLFWwindow* window)
{
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime();

	elapsedSeconds = currentSeconds - previousSeconds;

	if (elapsedSeconds > 0.25)//update text 4 times per second
	{
		previousSeconds = currentSeconds;
		double fps = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;
		std::ostringstream outs;
		outs.precision(3);
		outs << std::fixed
			<< APP_TITLE << "  "
			<< "FPS: " << fps << " "
			<< "Frame Time; " << msPerFrame << " (ms)";
		glfwSetWindowTitle(window, outs.str().c_str());
		frameCount = 0;
	}
	frameCount++;
}
