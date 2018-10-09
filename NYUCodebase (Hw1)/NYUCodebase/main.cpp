//Robert Yeung
//ry745	N15351645
//Homework 1

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

//For images and Matrices
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

GLuint LoadTexture(const char *filePath) {
	int w, h, comp;
	unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);

	if (image == NULL) {
		std::cout << "Unable to load image. Make sure the path is correct\n";
		assert(false);										
	}

	GLuint retTexture;
	glGenTextures(1, &retTexture);
	glBindTexture(GL_TEXTURE_2D, retTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(image);
	return retTexture;
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif
	SDL_Event event;
	bool done = false;

	//Setup and Enabling
	glViewport(0, 0, 640, 360);

	//Programs 1 and 3 will have textures. Program 2 is without
	ShaderProgram program, program2, program3;
	program.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	program2.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	program3.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	GLuint Texture1 = LoadTexture(RESOURCE_FOLDER"Black_Paper.jpg");
	GLuint Texture3 = LoadTexture(RESOURCE_FOLDER"Image_Spiral.jpg");

	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	glm::mat4 projectionMatrix2 = glm::mat4(1.0f);
	glm::mat4 projectionMatrix3 = glm::mat4(1.0f);

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 modelMatrix2 = glm::mat4(1.0f);
	glm::mat4 modelMatrix3 = glm::mat4(1.0f);

	glm::mat4 viewMatrix = glm::mat4(1.0f);
	glm::mat4 viewMatrix2 = glm::mat4(1.0f);
	glm::mat4 viewMatrix3 = glm::mat4(1.0f);

	projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);

	glUseProgram(program.programID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float lastFrameTicks = 0.0f;
	float angle = 75.0f * (3.14159f / 180.0f);
	//End of Setup

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}

		//***Starting First item***
		glUseProgram(program.programID);
		glClearColor(1.0f, 0.2f, 0.7f, 0.8f);
		glClear(GL_COLOR_BUFFER_BIT);

		program.SetModelMatrix(modelMatrix);
		program.SetProjectionMatrix(projectionMatrix);
		program.SetViewMatrix(viewMatrix);

		glBindTexture(GL_TEXTURE_2D, Texture1);

		float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program.positionAttribute);

		float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		//Creates the rotation animation
		modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 0.0f, 1.0f));
		program.SetModelMatrix(modelMatrix);
		//***End of First item***

		//***Starting Second item***
		glUseProgram(program2.programID);

		program2.SetModelMatrix(modelMatrix2);
		program2.SetProjectionMatrix(projectionMatrix2);
		program2.SetViewMatrix(viewMatrix2);

		float vertices2[] = { -1.2, -1, -1.7, 0.7, 0.1, 0.9, -0.1, -0.1, 0.1, 0.1, -0.1, 0.1 };

		glVertexAttribPointer(program2.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
		glEnableVertexAttribArray(program2.positionAttribute);

		float texCoords2[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

		glVertexAttribPointer(program2.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords2);
		glEnableVertexAttribArray(program2.texCoordAttribute);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program2.positionAttribute);
		glDisableVertexAttribArray(program2.texCoordAttribute);
		//***End of Second item***

		//***Starting Third item***
		glUseProgram(program3.programID);

		program3.SetModelMatrix(modelMatrix3);
		program3.SetProjectionMatrix(projectionMatrix3);
		program3.SetViewMatrix(viewMatrix3);

		glBindTexture(GL_TEXTURE_2D, Texture3);

		float vertices3[] = { 1.0, 0.2, 0.5, 0.8, -0.4, 0.5, 0.8, -0.4, 0.3, -0.4, 0.55, -0.7 };

		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices3);
		glEnableVertexAttribArray(program.positionAttribute);

		float texCoords3[] = { 0.7, 0.6, 0.6, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords3);
		glEnableVertexAttribArray(program.texCoordAttribute);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program3.positionAttribute);
		glDisableVertexAttribArray(program3.texCoordAttribute);
		//***End of Third item***

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}