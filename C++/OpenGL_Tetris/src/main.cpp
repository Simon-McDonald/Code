/*
 * main.cpp
 *
 *  Created on: May 7, 2017
 *      Author: Simon
 */

#include <memory>
#include <map>

#include <UtilityManager.hpp>
#include <StopWatch.hpp>
#include <WorldManager.hpp>

#include "CheckErrors.h"
#include "ResourceManager.h"
#include "LevelInstance.hpp"
#include "LevelInstanceInvisPieces.hpp"

#include "MenuInstance.hpp"

class ProgramManager : protected UtilityManager {
public:
	ProgramManager(void) :
			shaderMap(generateShaderMap()),
			currentShader(nullptr),
			isRunning(false) {

		CHECKERRORS();

		resourceManager.setupShaderDataFormat();
		WorldManager::Initialise(&this->mainWindow, &this->shaderMap);
		currentShader = ShaderManager::getActiveShaderManager();

		CHECKERRORS();

		//this->level.reset(new LevelInstanceInvisPieces());
		//this->level.reset(new LevelInstance());
		this->level.reset(new MenuInstance({"Standard Game", "Invisible Set Blocks", "Rising Level", "Strange Pieces", "No Walls", "Random Start Clutter", "Quit"}));
	}

	std::map<std::string, ShaderManager> generateShaderMap(void) {
		std::map<std::string, ShaderManager> mapOfShaders;

		shaderMap.emplace("piece", std::string("TETRIS_PIECE_SHADER"));
		shaderMap.emplace("text", std::string("TEXT_SHADER"));
		shaderMap.emplace("background", std::string("BACKGROUND_SHADER"));

		return mapOfShaders;
	}

	bool Start(void) {
		isRunning = true;
		this->timer.Start ();
		return isRunning;
	}

	bool Run(void) {
		float deltaTime_sec = timer.Mark();
		this->mainWindow.userInput();
		this->mainWindow.clearWindow();
		isRunning &= level->update((int)(deltaTime_sec * 1000.0), this->mainWindow.getInput());

		this->shaderMap.at("background").useProgram();
		level->renderBackground();

		this->shaderMap.at("piece").useProgram();
	    level->render();

	    this->shaderMap.at("text").useProgram();
	    level->renderText();

		this->mainWindow.updateWindow();
		isRunning &= !this->mainWindow.getInput().onDown.quit;
		return isRunning;
	}

	~ProgramManager(void) {
		INFO << "Ending the program" << END;
	}

private:
	OpenGLWindow mainWindow;
	std::map<std::string, ShaderManager> shaderMap;
	ShaderManager *currentShader;

	ResourceManager resourceManager;

	std::unique_ptr<Instance> level;

	StopWatch timer;
	bool isRunning;
};

int main(int argc, char *argv[])
{
	Config config("config/config.txt");
	Logger logger;

	UtilityManager::Initialise(&config, &logger);

	try
	{
		ProgramManager program;
		if (!program.Start()) {
			return EXIT_FAILURE;
		}

		while (program.Run());
	}
	catch (...) {
		std::cout << "Got an unknown exception!" << std::cout;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
