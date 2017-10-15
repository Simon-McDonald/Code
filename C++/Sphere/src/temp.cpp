//#define GLEW_STATIC
//#include <GL/glew.h>
//
//#include <SDL.h>
//#include <SDL_opengl.h>
//
//#include <SOIL.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include <iostream>
//#include <chrono>
//#include <stdio.h>
//#include <cmath>
//
/////////////////////////////////
//// Vertex shader source code //
/////////////////////////////////
//const GLchar* vertexSourceFinal =
//		"#version 150 core\n"
//
//		"in vec3 position;"
//		"in vec4 color;"
//		"in vec2 texcoord;"
//
//		"out vec4 Color;"
//		"out vec2 Texcoord;"
//
//		"uniform mat4 model;"
//		"uniform mat4 view;"
//		"uniform mat4 proj;"
//		"uniform vec3 overrideColor;"
//
//	    "void main() {"
//	    "   Color = color * vec4(overrideColor, 1.0f);"
//	    "   Texcoord = texcoord;"
//	    "   gl_Position = proj * view * model * vec4(position, 1.0);"
//	    "}";
//
//const GLchar* vertexSource = vertexSourceFinal;
//
///////////////////////////////////
//// Fragment Shader source code //
///////////////////////////////////
//const GLchar* fragmentSourceFinal =
//	"#version 150 core\n"
//
//	"in vec4 Color;"
//	"in vec2 Texcoord;"
//
//	"out vec4 outColor;"
//
//	"uniform sampler2D texKitten;"
//	"uniform sampler2D texPuppy;"
//	"uniform float textureTransition;"
//
//	"void main() {"
//	"   outColor = mix(texture(texKitten, Texcoord), texture(texPuppy, Texcoord), 0.5);"
//	"   outColor = Color * outColor;"
//	"}";
//
//const GLchar* fragmentSource = fragmentSourceFinal;
//
///*
// *
// */
//int blaMain(int argc, char *argv[])
//{
//	// Tell SDL which modules are needed
//	SDL_Init(SDL_INIT_VIDEO);
//
//	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
//
//	// Tell SDL that a forward compatible OpenGL 3.2 context is required
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
//
//	// Create the window
//	SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
//	// Final argument:
//	//  - SDL_WINDOW_OPENGL - Create a window ready for OpenGL
//	//  - SDL_WINDOW_RESIZABLE - Create a resizable window
//	//  - SDL_WINDOW_FULLSCREEN - Create a fullscreen window
//
//	// Create the OpenGL context
//	SDL_GLContext context = SDL_GL_CreateContext(window);
//
//	// Initialise GLEW
//	glewExperimental = GL_TRUE;
//	glewInit();
//
//
//
//
//
//
//
//
//
//
//	glEnable(GL_DEPTH_TEST);
//
//
//	// Create Vertex Array Object
//    GLuint vao;
//    glGenVertexArrays(1, &vao);
//    glBindVertexArray(vao);
//
//	// Create a Vertex Buffer Object and copy the vertex data to it
//    GLuint vbo;
//    glGenBuffers(1, &vbo);
//
//    GLfloat vertices[] = {
//    //      x,     y,    z,    r,    g,    b,    s,   tx,   ty,
//         /*0.5f,  0.5f, 0.0f, 1.0f, 0.5f, 0.5f, 1.0f, 1.0,  0.0,
//		 0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 0.5f, 1.0f, 1.0,  1.0,
//		-0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0, 1.0,
//		-0.5f,  0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0,  0.0,*/
//	//      x      y      z     r     g     b     s     tx    ty
//	    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
//	     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
//	     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
//	     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
//	    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
//	    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
//
//	    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
//	     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
//	     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
//	     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
//	    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
//	    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
//
//	    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
//	    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
//	    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
//	    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
//	    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
//	    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
//
//	     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
//	     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
//	     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
//	     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
//	     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
//	     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
//
//	    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
//	     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
//	     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
//	     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
//	    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
//	    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
//
//	    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
//	     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
//	     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
//	     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
//	    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
//	    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
//
//		-1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//		 1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
//		 1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
//		 1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
//		-1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
//		-1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f
//    };
//
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    /*GLuint ebo;
//    glGenBuffers(1, &ebo);
//
//    GLuint elements[] = {
//    	0, 1, 3,
//		1, 2, 3,
//		0, 1, 4,
//		1, 2, 5,
//		2, 3, 6,
//		3, 0, 7
//    };
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);*/
//
//
//
//
//
//
//
//
//
//	//////////////////////////////////////////
//	// Create and compile the vertex shader //
//	//////////////////////////////////////////
//    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexSource, NULL);
//    glCompileShader(vertexShader);
//
//	GLint status;
//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
//
//	if (status != GL_TRUE) {
//		printf("Vertex Shader failed to compile, log:");
//		char buffer[512];
//		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
//
//		for (int index = 0; index < 512; index++) {
//			std::cout << buffer[index];
//		}
//		std::cout << std::endl;
//
//		return 1;
//	} else {
//		std::cout << "Vertex Shader compiled successfully!" << std::endl;
//	}
//
//	////////////////////////////////////////////
//	// Create and compile the fragment shader //
//	////////////////////////////////////////////
//    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
//    glCompileShader(fragmentShader);
//
//	//GLint status;
//	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
//
//	if (status != GL_TRUE) {
//		printf("Fragment Shader failed to compile, log:");
//		char buffer[512];
//		glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
//
//		for (int index = 0; index < 512; index++) {
//			std::cout << buffer[index];
//		}
//		std::cout << std::endl;
//
//		return 1;
//	} else {
//		std::cout << "Fragment Shader compiled successfully!" << std::endl;
//	}
//
//	///////////////////////////////////////////////////////////////
//	// Link the vertex and fragment shader into a shader program //
//	///////////////////////////////////////////////////////////////
//	// compiling the program shaders and connecting them together
//    GLuint shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glBindFragDataLocation(shaderProgram, 0, "outColor");
//    glLinkProgram(shaderProgram);
//    glUseProgram(shaderProgram);
//
//	///////////////////////////////////////////
//	// Specify the layout of the vertex data //
//	///////////////////////////////////////////
//	// specifies the format of the position data
//    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
//    glEnableVertexAttribArray(posAttrib);
//    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);
//	// Specify how data from the input is retrieved from the array, note: 5 floats are used to specify each point
//
//	// specifies the format of the color data
//    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
//    glEnableVertexAttribArray(colAttrib);
//    glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
//
//    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
//    glEnableVertexAttribArray(texAttrib);
//    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(7 * sizeof(GLfloat)));
//
//    // Load texture
//    GLuint textures[2];
//    glGenTextures(2, textures);
//
//    int width, height;
//    unsigned char* image;
//
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, textures[0]);
//    image = SOIL_load_image("C:/MinGW/external/kitten.png", &width, &height, 0, SOIL_LOAD_RGB);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
//    SOIL_free_image_data(image);
//    glUniform1i(glGetUniformLocation(shaderProgram, "texKitten"), 0);
//
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_2D, textures[1]);
//    image = SOIL_load_image("C:/MinGW/external/puppy.png", &width, &height, 0, SOIL_LOAD_RGB);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
//    SOIL_free_image_data(image);
//    glUniform1i(glGetUniformLocation(shaderProgram, "texPuppy"), 1);
//
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    /*GLint textureTransition = glGetUniformLocation(shaderProgram, "textureTransition");
//    glUniform1f(textureTransition, 0.0f);*/
//
//    GLint uniModel = glGetUniformLocation(shaderProgram, "model");
//    GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
//    GLint uniView = glGetUniformLocation(shaderProgram, "view");
//
//    // Set up projection
//    glm::mat4 view = glm::lookAt(
//        glm::vec3(2.5f, 2.5f, 2.0f),
//        glm::vec3(0.0f, 0.0f, 0.0f),
//        glm::vec3(0.0f, 0.0f, 1.0f)
//    );
//    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
//
//    glm::mat4 proj = glm::perspective(glm::radians(50.0f), 800.0f / 600.0f, 1.0f, 10.0f);
//    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
//
//    /*glm::mat4 modelTrans;
//    modelTrans = glm::rotate(
//        modelTrans,
//        glm::radians(180.0f),
//        glm::vec3(0.0f, 0.0f, 1.0f)
//    );*/
//
//    //bool xRotate = false;
//    //auto xRotate_start = std::chrono::high_resolution_clock::now();
//
//    GLint uniColor = glGetUniformLocation(shaderProgram, "overrideColor");
//
//    GLuint counter = 0;
//    auto t_start = std::chrono::high_resolution_clock::now();
//	// Start the event loop
//	SDL_Event windowEvent;
//	while (true)
//	{
//		counter++;
//
//	    if (SDL_PollEvent(&windowEvent))
//	    {
//	        if (windowEvent.type == SDL_QUIT) break;
//
//	        if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
//
//	        /*if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_SPACE) {
//	        	xRotate_start = std::chrono::high_resolution_clock::now();
//	        	xRotate = true;
//	        	std::cout << "AAAA" << std::endl;
//	        }*/
//	    }
//
//        // Clear the screen to black
//        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//        //glClear(GL_COLOR_BUFFER_BIT);
//    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        // Calculate transformation
//        auto t_now = std::chrono::high_resolution_clock::now();
//        float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
//
//        //float rotatetime = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - xRotate_start).count();
//
//        glm::mat4 model;
//        model = glm::rotate(
//            model,
//            time * glm::radians(180.0f),
//            glm::vec3(0.0f, 0.0f, 1.0f)
//        );
//        glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
//
//        // Draw cube
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//
//        glEnable(GL_STENCIL_TEST);
//
//            // Draw floor
//            glStencilFunc(GL_ALWAYS, 1, 0xFF);
//            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//            glStencilMask(0xFF);
//            glDepthMask(GL_FALSE);
//            glClear(GL_STENCIL_BUFFER_BIT);
//
//            glDrawArrays(GL_TRIANGLES, 36, 6);
//
//            // Draw cube reflection
//            glStencilFunc(GL_EQUAL, 1, 0xFF);
//            glStencilMask(0x00);
//            glDepthMask(GL_TRUE);
//
//            model = glm::scale(glm::translate(model, glm::vec3(0, 0, -1)), glm::vec3(1, 1, -1));
//            glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
//
//            glUniform3f(uniColor, 0.3f, 0.3f, 0.3f);
//                glDrawArrays(GL_TRIANGLES, 0, 36);
//            glUniform3f(uniColor, 1.0f, 1.0f, 1.0f);
//
//        glDisable(GL_STENCIL_TEST);
//
//        // swap buffers
//	    SDL_GL_SwapWindow(window);
//	}
//
//	printf("Processed iteration %i\n", counter);
//
//	glDeleteTextures(2, textures);
//
//    glDeleteProgram(shaderProgram);
//    glDeleteShader(fragmentShader);
//    glDeleteShader(vertexShader);
//
//    //glDeleteBuffers(1, &ebo);
//    glDeleteBuffers(1, &vbo);
//
//    glDeleteVertexArrays(1, &vao);
//
//	// Destroy the OpenGL context
//	SDL_GL_DeleteContext(context);
//
//	// Tell SDL when to unload modules
//	SDL_Quit();
//	return 0;
//}
//
