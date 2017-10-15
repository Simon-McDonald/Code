
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Level.h"
#include "Logger.h"
#include "Config.h"
#include "StopWatch.h"

#include "Player.h"

#include "BillboardManager.h"
#include "CheckErrors.h"
#include "OpenGLWindow.h"
#include "UtilityManager.h"
#include "WorldManager.h"
#include "ShaderManager.h"
#include "RenderableObject.h"

#include "Camera.h"
#include "ResourceManager.h"

#include <assimp/Importer.hpp>
#include <assimp/matrix3x3.h>

class ProgramManager : protected UtilityManager {
public:
	ProgramManager(void) :
		shaderMap{{"Billboard", ShaderManager("FOREGROUND_SHADER")},
				  {"Character", ShaderManager("CHARACTER_SHADER")}} {
	}

	bool Initialise(void) {
		if (!this->mainWindow.Initialise()) {
			return false;
		}
		CHECKERRORS();
		for (std::map<std::string, ShaderManager>::iterator mapItr = this->shaderMap.begin();
			 mapItr != this->shaderMap.end();
			 mapItr++) {
			if (!mapItr->second.Initialise()) {
				return false;
			}
		}
		CHECKERRORS();
		WorldManager::Initialise(&this->mainWindow, &this->shaderMap);

		this->shaderMap.at("Billboard").useProgram();

		ResourceManager::InitialiseResourceManager();

		GLuint VertexArrayID;
		glGenVertexArrays (1, &VertexArrayID);
		glBindVertexArray (VertexArrayID);

		return true;
	}

	void Run(void) {
		CHECKERRORS();
		Level level("potato");
		level.LoadResources();
		level.InitialiseLevel(this->camera);


		CHECKERRORS();


		bool isRunning = true;
		StopWatch timer;
		timer.Start ();

		camera.Update({0.5, 0.5});

		while (isRunning) {
			float deltaTime = timer.Mark();
			this->mainWindow.userInput();

			this->mainWindow.clearWindow();

			level.UpdateLevel(deltaTime, this->camera);

			level.RenderLevel(this->camera);
	        CHECKERRORS();

			this->mainWindow.updateWindow();
			isRunning = !this->mainWindow.getInput().quit;
		}

		level.FinaliseLevel();
	}

	bool Destroy(void) {
		for (std::map<std::string, ShaderManager>::iterator mapItr = this->shaderMap.begin();
			 mapItr != this->shaderMap.end();
			 mapItr++) {
			mapItr->second.Destroy();
		}

		//shaderManager.Destroy();
		this->mainWindow.cleanUp();

		return true;
	}

	~ProgramManager(void) {
	}

private:
	OpenGLWindow mainWindow;
	std::map<std::string, ShaderManager> shaderMap;

	ResourceManager resourceManager;

	Camera camera;
};

int main(int argc, char *argv[]) {
	Config config("config/config.txt");
	Logger logger;
	UtilityManager::Initialise(&config, &logger);

	ProgramManager program;
	if (!program.Initialise()) {
		return EXIT_FAILURE;
	}

	program.Run();

	program.Destroy();

	return EXIT_SUCCESS;
}
