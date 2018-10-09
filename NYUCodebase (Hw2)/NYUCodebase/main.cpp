//Robert Yeung
//ry745	N15351645
//Homework 2
//Both paddles are controlled with the mouse

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <stdio.h>
#include <stdlib.h>
#include "ShaderProgram.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

ShaderProgram program;
SDL_Event event;

bool gameStarted = false;
bool gameEnded = false;
float lastFrameTicks = 0.0f;
float elapsed = 0.0f;
float ticks = (float)SDL_GetTicks() / 1000.0f;

glm::mat4 projectionMatrix = glm::mat4(1.0f);
glm::mat4 viewMatrix = glm::mat4(1.0f);
glm::mat4 modelMatrix = glm::mat4(1.0f);

glm::mat4 paddleOneMatrix;
glm::mat4 paddleTwoMatrix;

//Paddle and ball variables
float paddlesY = 0.0f;
float ballLocX = 0.0f;
float ballLocY = 0.0f;
float ballDirX = 0.0f;
float ballDirY = 0.0f;
float ballAngle = 45.0f;

void SetUp() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 360);

	program.Load(RESOURCE_FOLDER "vertex.glsl", RESOURCE_FOLDER "fragment.glsl");

	paddleOneMatrix = glm::mat4(1.0f);
	paddleTwoMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);
}

void ProcessEvents() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	//Listening to mouse motion
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			gameEnded = true;
		}
		else if (event.type == SDL_MOUSEMOTION) {
			float unitY = (((float)(360 - event.motion.y) / 360.0f) * 2.0f) - 1.0f;
			paddlesY = unitY;
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN && !gameStarted) {
			//Click to start game
			gameStarted = true;
			ballDirX = cos(45.0f * 3.14159 / 180);
			ballDirY = sin(45.0f * 3.14159 / 180);
		}
	}
}

//Checks for collision with paddles
bool collision(float num, float pongX, float pongY) {
	float paddle = 0.0f;

	if (num == 1) {
		paddle = -1.6f;
	}
	else if (num == 2) {
		paddle = 1.6f;
	}

	float distanceX = abs(pongX - paddle) - (0.03f + 0.15) / 2;
	float distanceY = abs(pongY - paddlesY) - (0.5f + 0.15) / 2;

	return (distanceX < 0 && distanceY < 0);
}

void Update() {
	glClear(GL_COLOR_BUFFER_BIT);

	program.SetModelMatrix(modelMatrix);
	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);

	ballLocX += ballDirX * elapsed * 5.0f;
	ballLocY += ballDirY * elapsed * 5.0f;

	//Checking for top/bot collissions
	if ((ballLocY >= 0.95f) || (ballLocY <= -0.95f)) {
		ballAngle *= -1;
		ballDirX = cos(ballAngle * 3.14159 / 180);
		ballDirY = sin(ballAngle * 3.14159 / 180);
	}

	//Checking for paddle collissions
	bool collidePaddleOne = collision(1, ballLocX, ballLocY);
	bool collidePaddleTwo = collision(2, ballLocX, ballLocY);

	if (collidePaddleOne || collidePaddleTwo) {
		if (ballAngle < 0) {
			if (!collidePaddleOne)
				ballAngle += -90;
			else
				ballAngle += 90;
		}
		else {
			if (collidePaddleOne) 
				ballAngle += -90;
			else 
				ballAngle += 90;
		}
		ballDirX = cos(ballAngle * 3.14159 / 180);
		ballDirY = sin(ballAngle * 3.14159 / 180);
	}

	//Checking for out of bounds
	else if (ballLocX + 0.02 >= 1.777f || ballLocX - 0.02 <= -1.777f) {

		modelMatrix = glm::mat4(1.0f);
		ballLocX = 0.0f;
		ballLocY = 0.0f;
		ballDirX = 0.0f;
		ballDirY = 0.0f;
		ballAngle = 45.0f;

		gameStarted = false;
	}
}

void Render() {
	//Drawing Paddle One
	paddleOneMatrix = glm::mat4(1.0f);
	paddleOneMatrix = glm::translate(paddleOneMatrix, glm::vec3(0.0f, paddlesY, 0.0f));
	program.SetModelMatrix(paddleOneMatrix);

	float paddle1[] = { -1.7f, -0.3f, -1.6f, 0.3f, -1.7f, 0.3f, -1.7f, -0.3f, -1.6f, -0.3f, -1.6f, 0.3f };
	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, paddle1);
	glEnableVertexAttribArray(program.positionAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program.positionAttribute);

	//Drawing Paddle Two
	paddleTwoMatrix = glm::mat4(1.0f);
	paddleTwoMatrix = glm::translate(paddleTwoMatrix, glm::vec3(0.0f, paddlesY, 0.0f));
	program.SetModelMatrix(paddleTwoMatrix);

	float paddle2[] = { 1.6f, -0.3f, 1.7f, 0.3f, 1.6f, 0.3f, 1.6f, -0.3f, 1.7f, -0.3f, 1.7f, 0.3f };
	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, paddle2);
	glEnableVertexAttribArray(program.positionAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program.positionAttribute);

	//Drawing Pong Ball
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(ballLocX, ballLocY, 0.0f));
	program.SetModelMatrix(modelMatrix);

	float ball[] = { -0.05f, -0.05f, 0.05f, 0.05f, -0.05f, 0.05f, -0.05f, -0.05f, 0.05f, -0.05f, 0.05f, 0.05f };
	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ball);
	glEnableVertexAttribArray(program.positionAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program.positionAttribute);

	SDL_GL_SwapWindow(displayWindow);
}

int main(int argc, char *argv[])
{
	SetUp();
	while (!gameEnded) {
		ProcessEvents();
		Update();
		Render();
	}

	//Cleanup
	SDL_Quit();
	return 0;
}