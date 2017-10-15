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

class TextImage : protected UtilityManager, protected WorldManager {
public:
	struct letterInfo {
		GLfloat startX;
		GLfloat startY;
		GLfloat width;
		GLfloat height;
	};

	TextImage(Config::ConfigHeader configHeader);

	void setShaderUniforms(void) const;
	//void Render(void);

	void generateStringBuffers(std::string renderString, DataBuffer<GLubyte, 1>& textBuffer, DataBuffer<GLfloat, 1> &spacingBuffer) const;
	/*size_t getOffset(void) const noexcept;
	const letterInfo& getInfo(size_t index) const noexcept;*/

	~TextImage(void);

private:
	GLuint textureId;
	unsigned ASCIIOffset;
	std::vector<letterInfo> letterInfoVec;
	DataBuffer<GLubyte, 1> renderText;
	DataBuffer<GLfloat, 1> renderTextSpacing;

	SDL_Surface* getImageObject(Config::ConfigHeader configHeader);
	size_t getBlockHeight(size_t textureWidth, size_t textureHeight, uint8_t *data);
	size_t getBlockWidth(size_t textureWidth, size_t textureHeight, uint8_t *data);
	void calculatePieceDims(size_t texWidth, size_t texHeight, size_t texBlockWidth, size_t texBlockHeight, uint8_t *data, std::vector<letterInfo> &infoVec);

};

class RenderableText : protected UtilityManager, protected WorldManager {
public:
	RenderableText(TextImage *textImage, std::string renderableString, GLfloat xPos, GLfloat yPos, GLfloat textSize);

	void updateText(std::string renderableString);

	void Render(void);

private:
	const TextImage *textImage;
	std::string renderableString;
	DataBuffer<GLubyte, 1> renderText;
	DataBuffer<GLfloat, 1> renderTextSpacing;
	GLfloat textSize;
	GLfloat screenPosition[2];
};

#endif /* TEXTIMAGE_HPP_ */
