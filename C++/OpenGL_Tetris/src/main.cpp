
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>

#include <UtilityManager.hpp>
#include <StopWatch.hpp>
#include "ResourceManager.h"

#include <WorldManager.hpp>

#include "LevelInstance.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CheckErrors.h"

#include <assimp/Importer.hpp>
#include <assimp/matrix3x3.h>

#include "DataBuffer.hpp"

class ProgramManager : protected UtilityManager {
public:
	ProgramManager(void) : currentShader(nullptr), isRunning(false) {
		shaderMap.emplace("piece", std::string("TETRIS_PIECE_SHADER"));
		shaderMap.emplace("text", std::string("TEXT_SHADER"));
	}

	bool Initialise(void) {
		check<int> check_int;
		check<float> check_float;

		/*if (check_int.test()) {
			INFO << "Int is true" << END;
		} else {
			INFO << "Int is false" << END;
		}
		if (check_float.test()) {
			INFO << "Float is true" << END;
		} else {
			INFO << "Float is false" << END;
		}*/

		check_int.test1();
		check_float.test2();

		//GLenum ttt = getEnumForType<GLfloat>();
		//INFO << "ttt: " << ttt << END;
		//INFO << "bbb: " << GL_FLOAT << END;


		INFO << "Initialise" << END;

		if (!this->mainWindow.Initialise()) {
			return false;
		}
		CHECKERRORS();

		resourceManager.setupShaderDataFormat();

		for (std::map<std::string, ShaderManager>::iterator mapItr = this->shaderMap.begin();
			 mapItr != this->shaderMap.end();
			 mapItr++) {
			if (!mapItr->second.Initialise()) {
				ERR << "THERE IS AN ERROR WITH A SHADER!!!" << END;
				return false;
			}
		}

		WorldManager::Initialise(&this->mainWindow, &this->shaderMap);



		currentShader = ShaderManager::getActiveShaderManager();

		CHECKERRORS();

		INFO << "Initialised the program!" << END;

		//This stops the other from rendering!!!
		this->level.reset(new LevelInstance());

		INFO << "Finished initialising" << END;

		/*unsigned array[12] = {1, 2, 3, 4, 5, 6, 2, 3, 4, 5, 6, 7};
		//unsigned array2[6] = {5, 3, 4, 5, 6, 7};

		DataBufferTemplate<GL_DOUBLE, 2, 3, 4, 1> temp(3, array);

		temp.enableLayoutPointers(1, 2, 3, 4);

		INFO << "Pre render call" << END;
		temp.render();
		INFO << "Post render call" << END;*/


		return true;
	}

	bool Start(void) {
		INFO << "START" << END;

		isRunning = true;
		this->timer.Start ();
		return isRunning;

		INFO << "START" << END;
	}

	bool Run(void) {
		float deltaTime_sec = timer.Mark();
		this->mainWindow.userInput();
		this->mainWindow.clearWindow();
		isRunning &= level->update((int)(deltaTime_sec * 1000), this->mainWindow.getInput());

		this->shaderMap.at("piece").useProgram();
	    level->render();

	    this->shaderMap.at("text").useProgram();
	    level->renderText();

		this->mainWindow.updateWindow();
		isRunning &= !this->mainWindow.getInput().onDown.quit;
		return isRunning;
	}

	bool Destroy(void) {
		INFO << "At the start of destroy" << END;

		LevelInstance *aaa = level.release();
		delete aaa;

		for (std::map<std::string, ShaderManager>::iterator mapItr = this->shaderMap.begin();
			 mapItr != this->shaderMap.end();
			 mapItr++) {
			mapItr->second.Destroy();
		}

		this->mainWindow.cleanUp();

		INFO << "Finished destroying" << END;

		return true;
	}

	~ProgramManager(void) {
		INFO << "Ending the program" << END;
	}

private:
	OpenGLWindow mainWindow;
	std::map<std::string, ShaderManager> shaderMap;
	ShaderManager *currentShader;

	ResourceManager resourceManager;

	std::unique_ptr<LevelInstance> level;

	StopWatch timer;
	bool isRunning;

	//DataBufferTemplate<GL_BYTE, 1, 2> bla;
};

int main(int argc, char *argv[])
{
	Config config("config/config.txt");
	Logger logger;

	UtilityManager::Initialise(&config, &logger);

	try
	{
		ProgramManager program;
		if (!program.Initialise() ||
			!program.Start()) {
			return EXIT_FAILURE;
		}

		while (program.Run());

		program.Destroy();
	}
	catch (...) {
		std::cout << "Got an exception!!!" << std::cout;
	}

	return EXIT_SUCCESS;
} /* main */
