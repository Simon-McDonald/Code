
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


// Shader sources
const GLchar* vertexSource =
    "#version 330\n"
    "layout (location = 0) in vec2 position;"
    "void main()"
    "{"
    "    gl_Position = vec4(position, 0.0, 1.0);"
    "}";
const GLchar* fragmentSource =
    "#version 330\n"
    "out vec4 outColor;"
    "void main()"
    "{"
    "    outColor = vec4(1.0, 1.0, 1.0, 1.0);"
    "}";


class ProgramManager : protected UtilityManager {
public:
	ProgramManager(void) :
		shaderMap{{"Billboard", ShaderManager("FOREGROUND_SHADER")}} {
	}

	bool Initialise(void) {
		if (!this->mainWindow.Initialise()) {
			return false;
		}

		/*for (std::map<std::string, ShaderManager>::iterator mapItr = this->shaderMap.begin();
			 mapItr != this->shaderMap.end();
			 mapItr++) {
			if (!mapItr->second.Initialise()) {
				return false;
			}
		}

		WorldManager::Initialise(&this->mainWindow, &this->shaderMap);

		this->shaderMap.at("Billboard").useProgram();

		//ResourceManager::setupShaderDataFormat();
		GLuint VertexArrayID;
		glGenVertexArrays (1, &VertexArrayID);
		glBindVertexArray (VertexArrayID);*/

		return true;
	}

	void Run(void) {

		/*Level level("potato");
		level.LoadResources();
		level.InitialiseLevel();*/


		CHECKERRORS();






	    // Create Vertex Array Object
	    GLuint vao;
	    glGenVertexArrays(1, &vao);
	    glBindVertexArray(vao);

	    // Create a Vertex Buffer Object and copy the vertex data to it
	    GLuint vbo;
	    glGenBuffers(1, &vbo);

	    GLfloat vertices[] = {
	         0.0f,  0.5f,
	        -0.5f, -0.5f,
	         0.5f, -0.5f
	    };

	    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	    // Create and compile the vertex shader
	    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	    glShaderSource(vertexShader, 1, &vertexSource, NULL);
	    glCompileShader(vertexShader);

	    // Create and compile the fragment shader
	    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	    glCompileShader(fragmentShader);

	    // Link the vertex and fragment shader into a shader program
	    GLuint shaderProgram = glCreateProgram();
	    glAttachShader(shaderProgram, vertexShader);
	    glAttachShader(shaderProgram, fragmentShader);
	    glBindFragDataLocation(shaderProgram, 0, "outColor");
	    glLinkProgram(shaderProgram);
	    glUseProgram(shaderProgram);

	    // Specify the layout of the vertex data
	    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	    glEnableVertexAttribArray(posAttrib);
	    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);













		bool isRunning = true;
		StopWatch timer;
		timer.Start ();

		while (isRunning) {
			float deltaTime = timer.Mark();
			this->mainWindow.userInput();

			this->mainWindow.clearWindow();

			//level.UpdateLevel(deltaTime, this->camera);

	        // Clear the screen to black
	        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	        glClear(GL_COLOR_BUFFER_BIT);

	        // Draw a triangle from the 3 vertices
	        glDrawArrays(GL_TRIANGLES, 0, 3);

	        CHECKERRORS();

			this->mainWindow.updateWindow();
			isRunning = !this->mainWindow.getInput().quit;
		}


	    glDeleteProgram(shaderProgram);
	    glDeleteShader(fragmentShader);
	    glDeleteShader(vertexShader);

	    glDeleteBuffers(1, &vbo);

	    glDeleteVertexArrays(1, &vao);

		//level.FinaliseLevel();
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

	//program.Destroy();

	return EXIT_SUCCESS;
}
