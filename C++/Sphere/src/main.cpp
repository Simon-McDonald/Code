#include <GL/glew.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <chrono>
#include <stdio.h>
#include <cmath>

#include "OpenGLController.h"
#include "GeometryShader.h"
#include "Camera.h"
#include "Renderer.h"
#include "RenderableObject.h"
#include "CollidableObject.h"
#include "Sphere.h"
#include "StopWatch.h"
#include "ErrorCheck.h"

#include "WorldPosition.h"
#include "WorldObject.h"

#define NEW

/*
 *
 */
int main(int argc, char *argv[])
{
	OpenGLController openGLController = OpenGLController();
	GeometryShader geometryShader = GeometryShader();
	Camera camera = Camera();

#ifdef OLD
	Sphere sphere = Sphere();
#else
	Renderer renderer = Renderer();
	CollidableObject player = CollidableObject(10, WorldPosition(-90 * M_PI / 180,0,0.5), 0.1, &renderer, 1);
	CollidableObject enemies[] = {
			CollidableObject(10, WorldPosition(-60 * M_PI / 180,0,0.5), 0.1, &renderer, 2),
			CollidableObject(10, WorldPosition(-90 * M_PI / 180,-60 * M_PI / 180,0.5), 0.1, &renderer, 2),
			CollidableObject(10, WorldPosition(-120 * M_PI / 180,0,0.5), 0.1, &renderer, 2),
			CollidableObject(10, WorldPosition(-90 * M_PI / 180,60 * M_PI / 180,0.5), 0.1, &renderer, 2)
	};
	RenderableObject world = RenderableObject(40, WorldPosition(0, 0, 0), 1, &renderer, 0);
#endif
	StopWatch stopWatch = StopWatch();

	openGLController.Initialise();
	CHECKERRORS();
	geometryShader.Initialise();
	CHECKERRORS();
	camera.Initialise(&geometryShader);
	CHECKERRORS();
#ifdef OLD
	sphere.Initialise(&geometryShader);
#else
	renderer.Initialise(&geometryShader);
#endif

	CHECKERRORS();

    stopWatch.Start();
	while (true)
	{
		float time = stopWatch.Mark();

        // Clear the screen to black
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		openGLController.UserInput();

		if (openGLController.userInputStruct.quit) {
			break;
		}

		CHECKERRORS();

		camera.Update(time, &openGLController.userInputStruct);
#ifdef OLD
		sphere.Update(&openGLController.userInputStruct);
#else
		player.moveObject(time, &openGLController.userInputStruct, camera.getHorzRotate(), camera.getVertRotate());

		world.display();
		player.display();

		for (unsigned int index = 0; index < (sizeof(enemies)/sizeof(CollidableObject)); index++) {
			enemies[index].killOnContact(&player);
			enemies[index].display();
		}
#endif

		CHECKERRORS();

        openGLController.Update();
		CHECKERRORS();
	}

	stopWatch.Stop();
	std::cout << "Basic Summary: " << std::endl;
	std::cout << "Frames: " << stopWatch.Counts() << std::endl;
	std::cout << "Time: " << stopWatch.Duration() << std::endl;
	std::cout << "FPS: " << (stopWatch.Counts() / stopWatch.Duration()) << std::endl;

#ifdef OLD
	sphere.Destroy();
#else
	renderer.Destroy();
#endif
	geometryShader.Destroy();
	openGLController.Terminate();

	return 0;
}
