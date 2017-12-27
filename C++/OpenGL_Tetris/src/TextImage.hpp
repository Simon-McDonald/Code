/*
 * TextImage.hpp
 *
 *  Created on: Oct 1, 2017
 *      Author: Simon
 */

#ifndef TEXTIMAGE_HPP_
#define TEXTIMAGE_HPP_

#include <UtilityManager.hpp>
#include <WorldManager.hpp>

#include "DataBuffer.hpp"
#include "BufferObjects.hpp"

/*
 * Defines the font of the text to be displayed. Initialised by pointing to the configuration header containing the
 * required information for the font. Namely, the configuration must specify the texture that contains the characters
 * under the header 'file'. Optionally, if the texture in 'file' does not start at ASCII character 0 an offset can be
 * specified under the header 'start_char' that will effectively shift the ASCII based indexing.
 */
class TextImage : protected UtilityManager, protected WorldManager {
public:
	enum class TextAlignment {
		RIGHT,
		TOP,
		CENTRE,
		LEFT,
		BOTTOM
	};

	struct letterInfo {
		GLfloat startX;
		GLfloat startY;
		GLfloat width;
		GLfloat height;
	};

	struct fontSizing {
	    GLfloat textSizeX;
	    GLfloat textSizeY;
	    GLfloat textSpacingX;
	    GLfloat textSpacingY;
	};

	struct letterSpacing {
	    GLfloat spacingX;
	    GLfloat spacingY;
	};

	/*
	 * Construct font with a configuration header. Must contain 'file' pointing to appropriate image and 'start_char'
	 * indicating the starting character in the image. Image must have lines dividing characters and marking character
	 * sizes.
	 */
	TextImage(Config::ConfigHeader configHeader);

	/*
	 * Prepare to render text with this font.
	 */
	void setShaderUniforms(void) const;

	/*
	 * Generate the underlying buffer data to render text with this font.
	 */
	void generateStringBuffers(std::string renderString,
	    DataBuffer<GLubyte, 1>& textBuffer, DataBuffer<GLfloat, 2> &spacingBuffer,
	    fontSizing &fontSizeInfo, TextAlignment hAlignment, TextAlignment vAlignment) const;

	/*
	 * Free the texture buffer.
	 */
	~TextImage(void);

private:
	GLuint textureId;
	unsigned ASCIIOffset;
	std::vector<letterInfo> letterInfoVec;

	/*
	 * Helper functions for the generation of data.
	 */
	SDL_Surface* getImageObject(Config::ConfigHeader configHeader);
	size_t getBlockHeight(size_t textureWidth, size_t textureHeight, uint8_t *data);
	size_t getBlockWidth(size_t textureWidth, size_t textureHeight, uint8_t *data);
	void calculatePieceDims(size_t texWidth, size_t texHeight, size_t texBlockWidth, size_t texBlockHeight,
	        uint8_t *data, std::vector<letterInfo> &infoVec);
};

#endif /* TEXTIMAGE_HPP_ */
