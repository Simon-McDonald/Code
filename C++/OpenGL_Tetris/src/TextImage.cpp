/*
 * TextImage.cpp
 *
 *  Created on: Oct 1, 2017
 *      Author: Simon
 */

#include <stdexcept>

#include "TextImage.hpp"
#include "ResourceManager.h"

TextImage::TextImage(Config::ConfigHeader configHeader) :
    textureId(-1u), ASCIIOffset(0) {
    SDL_Surface *imageVar = this->getImageObject(configHeader);

    uint8_t *data = (uint8_t *) imageVar->pixels;

    size_t textureWidth = imageVar->w;
    size_t textureHeight = imageVar->h;

    size_t textureBlockWidth = this->getBlockWidth(textureWidth, textureHeight, data);
    size_t textureBlockHeight = this->getBlockHeight(textureWidth, textureHeight, data);

    this->calculatePieceDims(textureWidth, textureHeight, textureBlockWidth, textureBlockHeight, data,
        this->letterInfoVec);

    this->textureId = ResourceManager::dataToTexture(configHeader, imageVar);
}

void TextImage::setShaderUniforms(void) const {
    glUniform4fv(this->getShaderManager().getUniformLocation("letterInformation"), this->letterInfoVec.size(),
        &this->letterInfoVec[0].startX);
    this->getShaderManager().bindTexture("uTextSampler", this->textureId);
}

SDL_Surface* TextImage::getImageObject(Config::ConfigHeader configHeader) {
    if (!this->getConfig().containsHeader(configHeader)) {
        throw std::invalid_argument("Header '" + configHeader + "' does not exist!");
    }

    std::string fileName = this->getConfig().getString(configHeader, "file");
    ASCIIOffset = this->getConfig().getUInt(configHeader, "start_char");

    SDL_Surface* imageVar = ResourceManager::loadRawImage(fileName);

    if (!imageVar) {
        throw std::invalid_argument("Header '" + configHeader + "' could not be loaded!");
    }

    return imageVar;
}

size_t TextImage::getBlockHeight(size_t textureWidth, size_t textureHeight, uint8_t *data) {
    size_t blockHeight = 0;

    while (blockHeight < textureHeight) {
        if (data[blockHeight * textureWidth * 3] >= 200 && data[blockHeight * textureWidth * 3 + 1] == 0
            && data[blockHeight * textureWidth * 3 + 2] >= 200) {

            ++blockHeight;
            return blockHeight;
        } else {
            ++blockHeight;
        }
    }

    return blockHeight;
}

size_t TextImage::getBlockWidth(size_t textureWidth, size_t textureHeight, uint8_t *data) {
    size_t blockWidth = 0;

    while (blockWidth < textureWidth) {
        if (data[3 * blockWidth] >= 200 && data[3 * blockWidth + 1] == 0 && data[3 * blockWidth + 2] >= 200) {

            ++blockWidth;
            return blockWidth;
        } else {
            ++blockWidth;
        }
    }

    return blockWidth;
}

void TextImage::calculatePieceDims(size_t texPixelWidth, size_t texPixelHeight, size_t blockPixelWidth,
    size_t blockPixelHeight, uint8_t *data, std::vector<letterInfo> &infoVec) {
    size_t texBlockWidth = texPixelWidth / blockPixelWidth;
    size_t texBlockHeight = texPixelHeight / blockPixelHeight;

    size_t numBlocks = texBlockWidth * texBlockHeight;
    infoVec.resize(numBlocks, { 0 });

    for (size_t blockIdx = 0; blockIdx < numBlocks; ++blockIdx) {
        size_t blockHeightIdx = blockIdx / texBlockWidth;
        size_t blockWidthIdx = blockIdx % texBlockWidth;

        size_t pixelWidthIdx = blockWidthIdx * blockPixelWidth;
        size_t pixelHeightIdx = blockHeightIdx * blockPixelHeight;

        size_t index = texPixelWidth * (blockPixelHeight * blockHeightIdx + blockPixelHeight / 2)
            + blockWidthIdx * blockPixelWidth;
        size_t letterWidth = 0;
        while (letterWidth < 2 * blockPixelWidth) {
            if ((data[3 * (index + letterWidth)] == 0 && data[3 * (index + letterWidth) + 1] >= 200
                && data[3 * (index + letterWidth) + 2] >= 200)
                || (data[3 * (index + letterWidth)] >= 200 && data[3 * (index + letterWidth) + 1] == 0
                    && data[3 * (index + letterWidth) + 2] >= 200)) {

                ++letterWidth;
                break;
            } else {
                ++letterWidth;
            }
        }

        infoVec[blockIdx] = {
            ((float) pixelWidthIdx) / texPixelWidth,
            ((float) pixelHeightIdx) / texPixelHeight,
            ((float) (letterWidth - 1)) / texPixelWidth,
            ((float) (blockPixelHeight - 1)) / texPixelHeight
        };
    }
}

void TextImage::generateStringBuffers(std::string renderString, TextAlignment alignment,
    DataBuffer<GLubyte, 1>& textBuffer, DataBuffer<GLfloat, 1> &spacingBuffer) const {
    const size_t stringLength = renderString.size();

    GLubyte charBuffer[stringLength];
    GLfloat spaceBuffer[stringLength];

    charBuffer[0] = renderString.at(0) - this->ASCIIOffset;
    spaceBuffer[0] = 0.0; //this->letterInfoVec[charBuffer[0]].width;

    for (size_t charIdx = 1; charIdx < stringLength; ++charIdx) {
        charBuffer[charIdx] = renderString.at(charIdx) - this->ASCIIOffset;
        spaceBuffer[charIdx] = this->letterInfoVec[charBuffer[charIdx - 1]].width + spaceBuffer[charIdx - 1] + 0.01;
    }

    GLfloat textWidth = spaceBuffer[stringLength - 1] + this->letterInfoVec[charBuffer[stringLength - 1]].width;
    switch (alignment) {
    case TextAlignment::RIGHT:
    case TextAlignment::BOTTOM:
        textWidth = 0.0;
        break;
    case TextAlignment::CENTRE:
        textWidth /= 2.0;
        break;
    case TextAlignment::LEFT:
    case TextAlignment::TOP:
        // Do nothing
        break;
    }

    for (size_t charIdx = 0; charIdx < stringLength; ++charIdx) {
        spaceBuffer[charIdx] = spaceBuffer[charIdx] - textWidth;
    }

    textBuffer.resetBuffer(stringLength, &charBuffer[0]);
    spacingBuffer.resetBuffer(stringLength, &spaceBuffer[0]);
}

TextImage::~TextImage(void) {
    // delete buffers/images
}

RenderableText::RenderableText(TextImage *textImage, std::string renderableString, GLfloat xPos, GLfloat yPos,
    GLfloat textSize, TextImage::TextAlignment hAlignment, TextImage::TextAlignment vAlignment) :
        textImage(textImage), renderableString(renderableString), textSize(textSize), screenPosition { xPos, yPos },
        horizontalAlignment(hAlignment), verticalAlignment(vAlignment) {
    this->updateText(renderableString);
}

void RenderableText::updateText(std::string renderableString) {
    this->textImage->generateStringBuffers(renderableString, this->horizontalAlignment, this->renderText,
        this->renderTextSpacing);
}

void RenderableText::Render(void) {

    CHECKERRORS();

    this->textImage->setShaderUniforms();

    CHECKERRORS();

    this->getShaderManager().bindVector2("uScreenPosition", &screenPosition[0]);
    this->getShaderManager().setUniformFloat("uTextScaling", this->textSize);

    CHECKERRORS();

    this->renderTextSpacing.bindEnableLayoutPointers(1);
    this->renderText.manageRender(0);
    this->renderTextSpacing.disableLayoutPointers(1);
}

