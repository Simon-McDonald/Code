/*
 * RenderableText.hpp
 *
 *  Created on: Dec 27, 2017
 *      Author: Simon
 */

#ifndef SRC_RENDERABLETEXT_HPP_
#define SRC_RENDERABLETEXT_HPP_

#include "TextImage.hpp"

/*
 * Instance of a string of characters to write to screen. Upon initialisation the text must be defined with a TextImage
 * instance that will define the dimensions.
 */
class RenderableText : protected UtilityManager, protected WorldManager {
public:
    /*
     * Define the text to display.
     * textImage: defines the font and sizing of the text
     * renderableString: string to display
     * xPos, yPos: screen position of text
     * textSize, textSizeWidth: define scaling height and width, if textSizeWidth 0 or omitted will scale equivalently
     *     in each direction.
     * hAlignment, vAlignment: define how text aligned with respect to position.
     * widthSpace, heightSpace: additional spacing between letters horizontally and vertically
     * textColour: text colour
     */
    RenderableText(TextImage *textImage, std::string renderableString,
            GLfloat xPos, GLfloat yPos,
            GLfloat textSize, GLfloat textSizeWidth = 0.0,
            TextImage::TextAlignment hAlignment = TextImage::TextAlignment::LEFT,
            TextImage::TextAlignment vAlignment = TextImage::TextAlignment::TOP,
            GLfloat widthSpace = 0.0, GLfloat heightSpace = 0.0,
            GLColour<GLfloat> textColour = {0.0, 0.0, 0.0});

    /*
     * Update some parameters of the text.
     */
    void setColour(GLColour<GLfloat> textColour);
    void setSize(GLfloat widthSize, GLfloat heightSize = 0.0);
    void updateText(std::string renderableString);

    /*
     * Render the text.
     */
    void Render(void);

private:
    const TextImage *textImage;
    std::string renderableString;
    buf::glBuffer<GLubyte, GL_DYNAMIC_DRAW, GL_ARRAY_BUFFER, 1> renderText;
    buf::glBuffer<GLfloat, GL_DYNAMIC_DRAW, GL_ARRAY_BUFFER, 2> renderTextSpacing;

    GLfloat screenPosition[2];
    TextImage::fontSizing fontSizeInfo;
    TextImage::TextAlignment horizontalAlignment;
    TextImage::TextAlignment verticalAlignment;

    GLColour<GLfloat> textColour;
};

#endif /* SRC_RENDERABLETEXT_HPP_ */
