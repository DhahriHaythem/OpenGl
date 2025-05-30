#include<iostream>
#include<sstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "ProgramShader.h"
#include "Texture2D.h"
#include "Camera.h"

const char* APP_TITLE = "Introduction to Modern OpenGL - Hello Triangle";

int WINDOW_WIDTH = 1024;
int WINDOW_HEIGHT = 768;
GLFWwindow* pWindow = NULL;
bool gWireframe = false;
const std::string texture1Filename = "Textures/wooden_crate.jpg";
const std::string texture2Filename = "Textures/grid.jpg";

FPSCamera fpsCamera(glm::vec3(0.0f, 0.0f, 5.0f));
const double ZOOM_SENSITIVITY = -3.0f;
const float MOVE_SPEED = 5.0f;
const float MOUSE_SENSITIVITY = 0.1f;

void glfw_Onkey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height);
void glfw_OnMouseMove(GLFWwindow* window, double posX, double posY);
void glfw_OnMouseScroll(GLFWwindow* window, double deltaX, double deltaY);
void update(double elapsedTime);
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
	glm::vec3 floorPos = glm::vec3(0.0f, -1.0f, 0.0f);
	
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

	Texture2D texture1, texture2;
	texture1.loadTexture(texture1Filename, true);
	texture2.loadTexture(texture2Filename, true);

	while (!glfwWindowShouldClose(pWindow))
	{
		showFPS(pWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		glfwPollEvents();
		update(deltaTime);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model, view, projection;
		model = view = projection = glm::mat4(1.0f);

		model = glm::translate(model, cubePos);

		view = fpsCamera.getViewMatrix();

		projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

		programShader.use();

		programShader.setUniform("model", model);
		programShader.setUniform("view", view);
		programShader.setUniform("projection", projection);

		glBindVertexArray(vao);
		texture1.bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		texture2.bind();
		
		model = glm::translate(model, floorPos) * glm::scale(model, glm::vec3(10.0f, 0.01f, 10.0f));
		programShader.setUniform("model", model);
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
	glfwSetCursorPosCallback(pWindow, glfw_OnMouseMove);
	glfwSetScrollCallback(pWindow, glfw_OnMouseScroll);

	glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(pWindow, WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 2.0);

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

void glfw_OnMouseMove(GLFWwindow* window, double posX, double posY)
{
	//static glm::vec2 lastMousePos = glm::vec2(0.0f, 0.0f);

	//float x = (float)posX;
	//float y = (float)posY;

	//float dx = x - lastMousePos.x;
	//float dy = y - lastMousePos.y;

	////update angle based on left mouse button
	//if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_LEFT) == 1)
	//{
	//	gYaw -= dx * MOUSE_SENSITIVITY;
	//	gPitch += dy * MOUSE_SENSITIVITY;
	//}

	//if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_RIGHT) == 1)
	//{
	//	gRadius += 0.01f * (dx - dy);
	//}
	//lastMousePos.x = x;
	//lastMousePos.y = y;
}

void glfw_OnMouseScroll(GLFWwindow* window, double deltaX, double deltaY)
{
	double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSITIVITY;

	fov = glm::clamp<double>(fov, 1.0, 120.0);

	fpsCamera.setFOV((float)fov);
}

void update(double elapseTime)
{
	double mouseX, mouseY;

	glfwGetCursorPos(pWindow, &mouseX, &mouseY);

	fpsCamera.rotate(((float)WINDOW_WIDTH / 2.0f - (float)mouseX) * MOUSE_SENSITIVITY, ((float)WINDOW_HEIGHT / 2.0f - (float)mouseY) * MOUSE_SENSITIVITY);

	glfwSetCursorPos(pWindow, (float)WINDOW_WIDTH / 2.0f, (float)WINDOW_HEIGHT / 2.0f);

	if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED*(float)elapseTime * fpsCamera.getLook());
	else if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapseTime * -fpsCamera.getLook());

	if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapseTime * fpsCamera.getRight());
	else if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapseTime * -fpsCamera.getRight());

	if (glfwGetKey(pWindow, GLFW_KEY_Z) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapseTime * fpsCamera.getUp());
	else if (glfwGetKey(pWindow, GLFW_KEY_X) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapseTime * -fpsCamera.getUp());
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
