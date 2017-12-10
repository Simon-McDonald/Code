/*
 * TextImage.hpp
 *
 *  Created on: Oct 1, 2017
 *      Author: Simon
 */

#ifndef TEXTIMAGE_HPP_
#define TEXTIMAGE_HPP_

#include "UtilityManager.hpp"
#include "WorldManager.hpp"
#include "DataBuffer.hpp"

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

	TextImage(Config::ConfigHeader configHeader);

	void setShaderUniforms(void) const;

	void generateStringBuffers(std::string renderString,
	    DataBuffer<GLubyte, 1>& textBuffer, DataBuffer<GLfloat, 2> &spacingBuffer,
	    fontSizing &fontSizeInfo, TextAlignment hAlignment, TextAlignment vAlignment) const;

	~TextImage(void);

private:
	GLuint textureId;
	unsigned ASCIIOffset;
	std::vector<letterInfo> letterInfoVec;

	SDL_Surface* getImageObject(Config::ConfigHeader configHeader);
	size_t getBlockHeight(size_t textureWidth, size_t textureHeight, uint8_t *data);
	size_t getBlockWidth(size_t textureWidth, size_t textureHeight, uint8_t *data);
	void calculatePieceDims(size_t texWidth, size_t texHeight, size_t texBlockWidth, size_t texBlockHeight,
	        uint8_t *data, std::vector<letterInfo> &infoVec);
};

/*
 * Specific string of characters to render to screen.
 */
class RenderableText : protected UtilityManager, protected WorldManager {
public:
	RenderableText(TextImage *textImage, std::string renderableString,
			GLfloat xPos, GLfloat yPos,
			GLfloat textSize, GLfloat textSizeWidth = 0.0,
			TextImage::TextAlignment hAlignment = TextImage::TextAlignment::LEFT,
			TextImage::TextAlignment vAlignment = TextImage::TextAlignment::TOP,
			GLfloat widthSpace = 0.0, GLfloat heightSpace = 0.0,
			GLColour<GLfloat> textColour = {0.0, 0.0, 0.0});

	void setColour(GLColour<GLfloat> textColour);
	void setSize(GLfloat widthSize, GLfloat heightSize = 0.0);

	void updateText(std::string renderableString);

	void Render(void);

private:
	const TextImage *textImage;
	std::string renderableString;
	DataBuffer<GLubyte, 1> renderText;
	DataBuffer<GLfloat, 2> renderTextSpacing;

	GLfloat screenPosition[2];
	TextImage::fontSizing fontSizeInfo;
	TextImage::TextAlignment horizontalAlignment;
	TextImage::TextAlignment verticalAlignment;

	GLColour<GLfloat> textColour;
};

#endif /* TEXTIMAGE_HPP_ */
