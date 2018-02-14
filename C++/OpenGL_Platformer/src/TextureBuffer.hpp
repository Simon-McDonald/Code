/*
 * TextureBuffer.hpp
 *
 *  Created on: Jan 3, 2018
 *      Author: Simon
 */

#ifndef SRC_TEXTUREBUFFER_HPP_
#define SRC_TEXTUREBUFFER_HPP_

#include <SDL_image.h>

#include <ShaderManager.hpp>
#include <UtilityManager.hpp>

class TextureBuffer : protected UtilityManager {
public:
    static SDL_Surface* loadRawImage(std::string imageFileName);
    static void deleteSDLSurface(SDL_Surface* surface);

    TextureBuffer(std::string fileName,
        GLenum sWrapMethod = GL_REPEAT, GLenum tWrapMethod = GL_REPEAT,
        GLenum magFilter = GL_LINEAR, GLenum minFilter = GL_LINEAR);

    TextureBuffer(const TextureBuffer& orig) = delete;
    TextureBuffer& operator=(const TextureBuffer& orig) = delete;

    TextureBuffer(TextureBuffer&& orig) noexcept;
    TextureBuffer& operator=(TextureBuffer&& orig) noexcept;

    // TODO integrate better with shader manager, send TextureBuffer as argument to ShaderManager function.
    bool bindTexture(ShaderManager &shader, std::string textureLoc);

    ~TextureBuffer(void);

private:
    GLuint bufferId;
};

#endif /* SRC_TEXTUREBUFFER_HPP_ */
