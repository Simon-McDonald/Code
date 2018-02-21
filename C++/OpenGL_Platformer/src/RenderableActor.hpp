/*
 * RenderableActor.hpp
 *
 *  Created on: Jan 3, 2018
 *      Author: Simon
 */

#ifndef SRC_RENDERABLEACTOR_HPP_
#define SRC_RENDERABLEACTOR_HPP_

#include <WorldManager.hpp>
#include <UtilityManager.hpp>

//#include "glBuffer.hpp"
#include "Skeleton.hpp"
#include "Flesh.hpp"
//#include "texBuffer.hpp"

//template <GLuint... T>
//using floatBuffer = buf::dynamicArrayBuffer<GLfloat, T...>;
//using floatBuffer = buf::arrayBuffer<GLfloat, GL_STATIC_DRAW, T...>;
//using floatBuffer = buf::glBuffer<GLfloat, GL_STATIC_DRAW, GL_ARRAY_BUFFER, T...>;

class RenderableActor : protected WorldManager, protected UtilityManager {
public:
    RenderableActor(Config::ConfigHeader);

    void render(ShaderManager &shader);

private:
    Config::ConfigHeader configHeader;
    //TextureBuffer texture;
    //DataBuffer<GLfloat, 2, 2> data;
    //buf::dynamicArrayBuffer<GLfloat, 2, 2> data;

    mod::Flesh flesh;
    mod::Skeleton skeleton;
};

#endif /* SRC_RENDERABLEACTOR_HPP_ */
