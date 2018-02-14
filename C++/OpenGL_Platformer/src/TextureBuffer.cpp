/*
 * TextureBuffer.cpp
 *
 *  Created on: Jan 3, 2018
 *      Author: Simon
 */

#include <stdexcept>

#include <GL/glew.h>

#include "TextureBuffer.hpp"

SDL_Surface* TextureBuffer::loadRawImage(std::string imageFileName) {
    std::string texturePath = TextureBuffer::getConfig().getValue<std::string>("RESOURCES", "images_dir") + "\\" + imageFileName;

    SDL_Surface *textureData = IMG_Load(texturePath.c_str());
    if (!textureData) {
        ERR << "Failed to load image: '" << texturePath << "' - " << IMG_GetError() << END;
        return nullptr;
    } else {
        INFO << "Loaded image '" << texturePath << "'" << END;
    }

    return textureData;
}

void TextureBuffer::deleteSDLSurface(SDL_Surface* surface) {
    SDL_FreeSurface (surface);
}

TextureBuffer::TextureBuffer(std::string fileName,
    GLenum sWrapMethod, GLenum tWrapMethod, GLenum magFilter, GLenum minFilter) : bufferId(-1u) {
    SDL_Surface *textureData = TextureBuffer::loadRawImage(fileName);
    if (!textureData) {
        std::string errorMsg = "Unable to load '" + fileName + "' as a texture!";
        throw std::invalid_argument(errorMsg);
    }

    glGenTextures(1, &this->bufferId);
    glBindTexture(GL_TEXTURE_2D, this->bufferId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureData->w, textureData->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData->pixels);

    TextureBuffer::deleteSDLSurface(textureData);

    // Sets the wrap parameter for texture in the S and T dimensions (width and height?). Options are:
    // GL_CLAMP_TO_EDGE - clamps to relevant dimension
    // GL_CLAMP_TO_BORDER - basically uses separately specified colour if outside texture
    // GL_REPEAT - as described
    // GL_MIRRORED_REPEAT - repeats texture but mirroring in each successive repeat
    // GL_MIRROR_CLAMP_TO_EDGE - Repeats for a single repetition
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrapMethod);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrapMethod);
    // Specify how colours determined when texture is magnified. Options are:
    // GL_NEAREST - use nearest texture element
    // GL_LINEAR - use weighted average of elements closest
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    // Specify how colours determined when texture is minified. Options are:
    // GL_NEAREST, GL_LINEAR - as above
    // Various parameters for MipMap textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);

    // Required if Min Filter uses MipMap for filtering
    glGenerateMipmap(GL_TEXTURE_2D);
}

TextureBuffer::TextureBuffer(TextureBuffer&& orig) noexcept : bufferId(-1u) {
    this->bufferId = orig.bufferId;
    orig.bufferId = -1u;
}

TextureBuffer& TextureBuffer::operator=(TextureBuffer&& orig) noexcept {
    auto tempId = this->bufferId;

    this->bufferId = orig.bufferId;
    orig.bufferId = tempId;

    return *this;
}

bool TextureBuffer::bindTexture(ShaderManager &shader, std::string textureLoc) {
    return shader.bindTexture(textureLoc, this->bufferId);
}

TextureBuffer::~TextureBuffer(void) {
    if (this->bufferId != -1u) {
        glDeleteTextures(1, &this->bufferId);
    }
}
