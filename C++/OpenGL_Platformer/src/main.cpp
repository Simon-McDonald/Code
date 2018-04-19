/*
 * main.cpp
 *
 *  Created on: May 7, 2017
 *      Author: Simon
 */

#include <CheckErrors.h>
#include <memory>
#include <map>

#include <UtilityManager.hpp>
#include <StopWatch.hpp>
#include <WorldManager.hpp>

#include "ResourceManager.hpp"
#include "RenderableActor.hpp"

class ProgramManager : protected UtilityManager {
public:
	ProgramManager(void) :
			shaderMap(generateShaderMap()),
			currentShader(nullptr),
			player(RenderableActor::generateInstance("models/modelVersion2.txt")),
			isRunning(false) {

		CHECKERRORS();

		WorldManager::Initialise(&this->mainWindow, &this->shaderMap);
		currentShader = ShaderManager::getActiveShaderManager();

		CHECKERRORS();
	}

	std::map<std::string, ShaderManager> generateShaderMap(void) {
		std::map<std::string, ShaderManager> mapOfShaders;

		shaderMap.emplace("actor", std::string("ACTOR_SHADER"));

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

		const auto& input = this->mainWindow.getInput();

		this->player.update(deltaTime_sec, input);

		this->shaderMap.at("actor").useProgram();
		this->player.render(this->shaderMap.at("actor"), duration_s);

		this->mainWindow.updateWindow();
		isRunning &= !this->mainWindow.getInput().onDown.quit;
		return isRunning;
	}

	~ProgramManager(void) {}

private:
	OpenGLWindow mainWindow;
	std::map<std::string, ShaderManager> shaderMap;
	ShaderManager *currentShader;

	ResourceManager resourceManager;

	RenderableActor player;

	StopWatch timer;
	float duration_s;
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
	} catch (std::exception &e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
