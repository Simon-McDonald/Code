/*
 * TextureBuffer.hpp
 *
 *  Created on: Jan 3, 2018
 *      Author: Simon
 */

#ifndef SRC_TEXBUFFER_HPP_
#define SRC_TEXBUFFER_HPP_

#include <SDL_image.h>

#include <ShaderManager.hpp>
#include <UtilityManager.hpp>

namespace buf {
    class texBuffer : protected UtilityManager {
    public:
        static SDL_Surface* loadRawImage(std::string imageFileName);
        static void deleteSDLSurface(SDL_Surface* surface);

        texBuffer(std::string fileName,
            GLenum sWrapMethod = GL_REPEAT, GLenum tWrapMethod = GL_REPEAT,
            GLenum magFilter = GL_LINEAR, GLenum minFilter = GL_LINEAR);

        texBuffer(const texBuffer& orig) = delete;
        texBuffer& operator=(const texBuffer& orig) = delete;

        texBuffer(texBuffer&& orig) noexcept;
        texBuffer& operator=(texBuffer&& orig) noexcept;

        // TODO integrate better with shader manager, send TextureBuffer as argument to ShaderManager function.
        bool bindTexture(ShaderManager &shader, std::string textureLoc);

        ~texBuffer(void);

    private:
        GLuint bufferId;
    };
};

#endif /* SRC_TEXBUFFER_HPP_ */
