/*
 * RenderableText.cpp
 *
 *  Created on: Dec 27, 2017
 *      Author: Simon
 */

#include "RenderableText.hpp"

RenderableText::RenderableText(TextImage *textImage, std::string renderableString, GLfloat xPos, GLfloat yPos,
    GLfloat textSize, GLfloat textSizeWidth, TextImage::TextAlignment hAlignment, TextImage::TextAlignment vAlignment,
    GLfloat widthSpace, GLfloat heightSpace, GLColour<GLfloat> textColour) :
        textImage(textImage), renderableString(renderableString), screenPosition{xPos, yPos},
        fontSizeInfo{textSize, textSizeWidth > 0 ? textSizeWidth : textSize, widthSpace, heightSpace},
        horizontalAlignment(hAlignment), verticalAlignment(vAlignment), textColour(textColour) {
    this->updateText(renderableString);
}

void RenderableText::setColour(GLColour<GLfloat> newColour) {
    this->textColour = newColour;
}

void RenderableText::setSize(GLfloat widthSize, GLfloat heightSize) {
    this->fontSizeInfo.textSizeX = widthSize;
    this->fontSizeInfo.textSizeY = heightSize > 0.0 ? heightSize : widthSize;
}

void RenderableText::updateText(std::string renderableString) {
    this->textImage->generateStringBuffers(renderableString, this->renderText, this->renderTextSpacing,
        fontSizeInfo, horizontalAlignment, verticalAlignment);
}

void RenderableText::Render(void) {

    CHECKERRORS();

    this->textImage->setShaderUniforms();

    CHECKERRORS();

    this->getShaderManager().bindVector2("uScreenPosition", &screenPosition[0]);
    this->getShaderManager().bindVector2("uTextScaling", this->fontSizeInfo.textSizeX, this->fontSizeInfo.textSizeY);
    this->getShaderManager().bindVector3("uTextColour", &this->textColour.r);

    CHECKERRORS();

    this->renderTextSpacing.bindEnableLayoutPointers(1);
    this->renderText.manageRender(0);
    this->renderTextSpacing.disableLayoutPointers(1);
}
