/*
 * TextImage.cpp
 *
 *  Created on: Oct 1, 2017
 *      Author: Simon
 */

#include <stdexcept>

#include "TextImage.hpp"

#include "ResourceManager.hpp"

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

    std::string fileName = this->getConfig().getValue<std::string>(configHeader, "file");
    ASCIIOffset = this->getConfig().getValue<unsigned>(configHeader, "start_char");

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

        size_t index = texPixelWidth * (blockPixelHeight * blockHeightIdx)
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

        size_t letterHeight = 0;
        while (letterHeight < 2 * blockPixelHeight) {
            if ((data[3 * (index + letterHeight * texPixelWidth)] == 0 &&
                data[3 * (index + letterHeight * texPixelWidth) + 1] >= 200 &&
                data[3 * (index + letterHeight * texPixelWidth) + 2] >= 200) ||
                (data[3 * (index + letterHeight * texPixelWidth)] >= 200 &&
                data[3 * (index + letterHeight * texPixelWidth) + 1] == 0 &&
                data[3 * (index + letterHeight * texPixelWidth) + 2] >= 200)) {

                ++letterHeight;
                break;
            } else {
                ++letterHeight;
            }
        }

        infoVec[blockIdx] = {
            ((float) pixelWidthIdx) / texPixelWidth,
            ((float) pixelHeightIdx) / texPixelHeight,
            ((float) (letterWidth - 1)) / texPixelWidth,
            ((float) (letterHeight - 1)) / texPixelHeight
        };
    }
}

void TextImage::generateStringBuffers(std::string renderString,
    buf::glBuffer<GLubyte, GL_DYNAMIC_DRAW, GL_ARRAY_BUFFER, 1>& textBuffer,
    buf::glBuffer<GLfloat, GL_DYNAMIC_DRAW, GL_ARRAY_BUFFER, 2> &spacingBuffer,
    fontSizing &fontSizeInfo, TextAlignment hAlignment, TextAlignment vAlignment) const {

    const size_t maxStringLength = renderString.size();

    GLubyte charBuffer[maxStringLength];
    letterSpacing spaceBuffer[maxStringLength];

    GLfloat xOffset = 0, yOffset = 0;
    size_t stringIdx = 0;
    size_t bufferIdx = 0, lineStartIdx = 0;

    while (stringIdx < maxStringLength) {
        if (renderString.at(stringIdx) == '\n') { // newline

            GLfloat textWidth = xOffset - fontSizeInfo.textSpacingX;
            switch (hAlignment) {
            case TextAlignment::LEFT:
                textWidth = 0.0;
                break;
            case TextAlignment::CENTRE:
                textWidth /= 2.0;
                break;
            case TextAlignment::RIGHT:
            default:
                // Do nothing
                break;
            }

            for (size_t charIdx = lineStartIdx; charIdx < bufferIdx; ++charIdx) {
                spaceBuffer[charIdx].spacingX -= textWidth;
            }

            yOffset += this->letterInfoVec[0].height + fontSizeInfo.textSpacingY;
            lineStartIdx = bufferIdx;
            xOffset = 0.0;

        } else {
            charBuffer[bufferIdx] = renderString.at(stringIdx) - this->ASCIIOffset;
            spaceBuffer[bufferIdx] = {xOffset, -yOffset};

            xOffset += this->letterInfoVec[charBuffer[bufferIdx]].width + fontSizeInfo.textSpacingX;
            ++bufferIdx;
        }

        ++stringIdx;
    }

    GLfloat textWidth = xOffset - fontSizeInfo.textSpacingX;
    switch (hAlignment) {
    case TextAlignment::LEFT:
        textWidth = 0.0;
        break;
    case TextAlignment::CENTRE:
        textWidth /= 2.0;
        break;
    case TextAlignment::RIGHT:
    default:
        // Do nothing
        break;
    }

    INFO << "lineStartIdx: " << lineStartIdx << ", bufferIdx: " << bufferIdx << END;
    for (size_t charIdx = lineStartIdx; charIdx < bufferIdx; ++charIdx) {
        spaceBuffer[charIdx].spacingX -= textWidth;
    }

    yOffset += this->letterInfoVec[0].height + fontSizeInfo.textSpacingY;

    GLfloat textHeight = yOffset - fontSizeInfo.textSpacingY;

    switch (vAlignment) {
    case TextAlignment::TOP:
        textHeight = 0.0;
        break;
    case TextAlignment::CENTRE:
        textHeight /= 2.0;
        break;
    case TextAlignment::BOTTOM:
    default:
        // Do nothing
        break;
    }

    for (size_t charIdx = 0; charIdx < bufferIdx; ++charIdx) {
        spaceBuffer[charIdx].spacingY -= textHeight;
    }

    textBuffer.resetBuffer(bufferIdx, &charBuffer[0]);
    spacingBuffer.resetBuffer(bufferIdx, &spaceBuffer[0].spacingX);
}

TextImage::~TextImage(void) {
    ResourceManager::deleteTexture(this->textureId);
}
