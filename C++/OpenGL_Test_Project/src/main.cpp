
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Logger.hpp"
#include "Config.hpp"
#include "StopWatch.hpp"

#include "Player.h"

#include "BillboardManager.h"
#include "CheckErrors.h"
#include "DirectionalLight.h"
#include "OpenGLWindow.h"
#include "UtilityManager.h"
#include "WorldManager.h"
#include "ShaderManager.h"
#include "RenderableObject.h"
#include "SkyMapManager.h"
#include "TessellatedObject.h"

#include "ShadowMapManager.h"

#include "ParticleSystem.h"

#include "Camera.h"
#include "ResourceManager.h"

#include <assimp/Importer.hpp>
#include <assimp/matrix3x3.h>

class ProgramManager : protected UtilityManager {
public:
	ProgramManager(void) : currentShader(nullptr), player(glm::vec3(0, -5, 1), 1),
		light("gDirectionalLight", glm::vec3(1.0f, 1.0f, 1.0f), 0.1, 0.9, glm::vec3(0.0f, -1.0f, -1.0f)) {
		std::cout << "CCCCCCCCCCCCCCC" << std::endl;
		INFO << "AAAAAAAAAAAAAAAA" << END;
		shaderMap.emplace("SkyBox", ShaderManager("SKYBOX_SHADER"));
		shaderMap.emplace("Standard", ShaderManager("SHADOW_SHADER"));
		shaderMap.emplace("Billboard", ShaderManager("BILLBOARD_SHADER"));
		shaderMap.emplace("Transform", ShaderManager("TRANSFORM_FEEDBACK"));
		shaderMap.emplace("Tessellation", ShaderManager("TESSELLATION"));

		DirectionalLight::initGlobalShaderParams(0.1, 0.1);
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

		WorldManager::Initialise(&this->mainWindow, &this->shaderMap);
		CHECKERRORS();
		this->shaderMap.at("Standard").useProgram();
		CHECKERRORS();
		DirectionalLight::setGlobalShaderParams();
		light.setLightInShader();

		/////////////////////////////////////////////
		this->shaderMap.at("SkyBox").useProgram();

		DirectionalLight::setGlobalShaderParams();
		////////////////////////////////////////

		this->shaderMap.at("Standard").useProgram();

		resourceManager.setupShaderDataFormat();
		shadowMap.InitialiseShadowMap();

		currentShader = ShaderManager::getActiveShaderManager();



		CHECKERRORS();

		return true;
	}

	void Run(void) {

		BillboardManager testBoard = BillboardManager("random-pic-14.bmp");
		testBoard.InitialiseBillboard(std::vector<GLfloat>({5.0, 2.0, 1.0,
															3.0, 3.0, 2}));

		BillboardManager blaBoard = BillboardManager("bla.bmp");
		blaBoard.InitialiseBillboard(std::vector<GLfloat>({-5.0, -2.0, 1.0,
															-3.0, -3.0, 2,
															3.0, -3.0, 2}));

		renderedObjects.push_back(RenderableObject("jeep.obj", "jeep_army.jpg", glm::vec3(4.0, 4.0, 1.0), 0.01));
		renderedObjects.push_back(RenderableObject("surfaceUp.obj", "smallRed2.bmp", glm::vec3(0.0, 0.0, 0.0), 5));

		SkyMapManager skyMap = SkyMapManager("sp3right.bmp", "sp3left.bmp", "sp3top.bmp", "sp3bot.bmp", "sp3front.bmp", "sp3back.bmp");
		skyMap.initialiseSkyMap();

		RenderableObject backFace = RenderableObject("BackSurface.obj", "", glm::vec3(0.0, 0.0, 0.0), 1.0);


		RenderableObject fullScreen = RenderableObject("fullScreenObj.obj", "", glm::vec3(0.6, -0.6, 0.0), 1.2);
		resourceManager.loadTexture("green.bmp");




		ParticleSystem fireworkLauncher1(
			glm::vec3(5.0, 5.0, 0.0),
			glm::vec3(1.0, -1.0, 8.0),
			0.08,
			1.0,
			0.5,
			1.0,
			20,
			1.5);
		fireworkLauncher1.InitParticleSystem();

		ParticleSystem fireworkLauncher2(
			glm::vec3(-5.0, -5.0, 0.0),
			glm::vec3(1.0, 1.0, 10.0),
			0.1,
			1.0,
			0.5,
			1.0,
			20,
			1.5);
		fireworkLauncher2.InitParticleSystem();

		TessellatedObject tessObj("suzanne.obj", "bla.bmp", glm::vec3(0.0, 0.0, 0.2));

		bool isRunning = true;
		this->timer.Start ();

		while (isRunning) {
			float deltaTime_sec = timer.Mark();
			this->mainWindow.userInput ();
			this->mainWindow.clearWindow();
			player.Update(deltaTime_sec);

			glm::vec3 eyeLoc = player.getWorldLoc();
			glm::mat4 ProjectionMatrix = camera.getProjectionMatrix();
			glm::mat4 ViewMatrix = player.getViewMatrix();
			glm::mat4 ViewProj = ProjectionMatrix * ViewMatrix;

			// Render the SkyBox

			this->currentShader = &shaderMap.at("SkyBox");
			this->currentShader->useProgram();

			skyMap.bindSkyMap("gSamplerCube");

			glm::mat4 newView = player.getRotationMatrix();
			shaderMap.at("SkyBox").bindArray("viewMat", 1, GL_FALSE, &newView[0][0]);

			currentShader->enableAttributeLocations();
			backFace.Render();
			currentShader->disableAttributeLocations();

			// Lighting based rendering

			currentShader = &shaderMap.at("Standard");
			currentShader->useProgram();

			glm::mat4 lightView = glm::lookAt(
					glm::vec3(0.0f, 5.0, 5.0),
					glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec3(0.0f, -1.0f, 1.0f)
			);
			glm::mat4 scaleMat = glm::scale(glm::mat4(1.0), glm::vec3(1.0, 1.0, 0.1));
			glm::mat4 lightMatrix = ProjectionMatrix * scaleMat * lightView;

			this->currentShader->bindVector3("gEyeWorldPos", &eyeLoc[0]);
			this->currentShader->bindArray("uWorld", 1, GL_FALSE, &ViewMatrix[0][0]);

			shadowMap.WriteTexture();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			this->currentShader->enableAttributeLocations();
			RenderScene(lightMatrix, lightMatrix);
			this->currentShader->disableAttributeLocations();

			shadowMap.defaultFramebuffer();
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			this->currentShader->bindTexture("gShadowMap", shadowMap.getTextureID());

			currentShader->enableAttributeLocations();
			RenderScene(lightMatrix, ViewProj);

			glm::mat4 id = glm::mat4(1.0);
			glm::mat4 mod = fullScreen.getViewMatrix();

			this->currentShader->bindArray("uWVP", 1, GL_FALSE, &mod[0][0]);
			this->currentShader->bindArray("uLightWMV", 1, GL_FALSE, &id[0][0]);
			this->currentShader->bindArray("uWorld", 1, GL_FALSE, &mod[0][0]);

			this->resourceManager.setTexture("green.bmp");
			fullScreen.Render();

			currentShader->disableAttributeLocations();

			// Render the Billboards
			this->currentShader = &shaderMap.at("Billboard");
			this->currentShader->useProgram();

			this->currentShader->bindArray("gVP", 1, GL_FALSE, &ViewProj[0][0]);
			this->currentShader->bindVector3("gCameraPos", &eyeLoc[0]);
			CHECKERRORS();
			currentShader->enableAttributeLocations();
			testBoard.Render();
			blaBoard.Render();
			currentShader->disableAttributeLocations();
			CHECKERRORS();
			// Render the particle system
			fireworkLauncher1.Render(deltaTime_sec, ViewProj, eyeLoc);
			CHECKERRORS();
			fireworkLauncher2.Render(deltaTime_sec, ViewProj, eyeLoc);
			CHECKERRORS();
			this->currentShader = &shaderMap.at("Tessellation");
			this->currentShader->useProgram();

			currentShader->enableAttributeLocations();
			tessObj.render(ViewProj);
			currentShader->disableAttributeLocations();

			// Update the window
			this->mainWindow.updateWindow();
			isRunning = !this->mainWindow.getInput().quit;
			CHECKERRORS();
		}
	}

	void RenderScene(glm::mat4 LightView, glm::mat4 ViewProjMatrix) {
		for (std::vector<RenderableObject>::iterator objItr = renderedObjects.begin();
			 objItr != renderedObjects.end();
			 objItr++) {

			glm::mat4 ModelMatrix = objItr->getViewMatrix();
			glm::mat4 MVP = ViewProjMatrix * ModelMatrix;
			glm::mat4 LightMatrix = LightView * ModelMatrix;

			this->currentShader->bindArray("uWVP", 1, GL_FALSE, &MVP[0][0]);
			this->currentShader->bindArray("uLightWMV", 1, GL_FALSE, &LightMatrix[0][0]);
			this->currentShader->bindArray("uWorld", 1, GL_FALSE, &ModelMatrix[0][0]);

			objItr->Render();
		}
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
		// close logger, mainWindow, shaderMap etc
	}

private:
	OpenGLWindow mainWindow;
	std::map<std::string, ShaderManager> shaderMap;

	ShaderManager *currentShader;
	std::vector<RenderableObject> renderedObjects;

	ResourceManager resourceManager;

	StopWatch timer;
	Player player;
	Camera camera;
	DirectionalLight light;
	ShadowMapManager shadowMap;
};

int main(int argc, char *argv[])
{
	Config config("config/config.txt");
	Logger logger;

	//config.displayConfig();
	UtilityManager::Initialise(&config, &logger);

	std::cout << "BBBBBBBBBBBB" << std::endl;
	ProgramManager program;
	if (!program.Initialise()) {
		std::cout << "QQQQQQQQQQQQQQQQQQQQQ" << std::endl;
		//ERR << "Failed to initialise stuff" << std::endl;
		return EXIT_FAILURE;
	}

	program.Run();

	program.Destroy();

	return EXIT_SUCCESS;
} /* main */
