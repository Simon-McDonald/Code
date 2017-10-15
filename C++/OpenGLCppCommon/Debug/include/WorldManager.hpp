/*
 * WorldManager.h
 *
 *  Created on: Mar 28, 2016
 *      Author: Simon
 */

#ifndef WORLDMANAGER_H_
#define WORLDMANAGER_H_

#include "OpenGLWindow.hpp"
#include "ShaderManager.hpp"

class WorldManager {
public:
	static void Initialise(OpenGLWindow *window, std::map<std::string, ShaderManager> *shaders);

	WorldManager(void);
	~WorldManager(void);

private:
	static std::map<std::string, ShaderManager> *shaders;
	static OpenGLWindow *window;

protected:
	static OpenGLWindow& getWindow(void);
	static ShaderManager& getShaderManager(std::string);
	static ShaderManager& getShaderManager(void);
};

#endif /* WORLDMANAGER_H_ */
