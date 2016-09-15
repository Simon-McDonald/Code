/*
 * FramebufferManager.cpp
 *
 *  Created on: Jun 21, 2016
 *      Author: Simon
 */

#include <iostream>

#include "FramebufferManager.h"

FramebufferManager::FramebufferManager(void) : textureID(-1u), fboID(-1u) {
}

void FramebufferManager::Initialise(void) {
	std::cout << "This is the framebuffer initialisation" << std::endl;

	glGenFramebuffers(1, &this->fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fboID);

	glGenTextures(1, &this->textureID);
	glBindTexture(GL_TEXTURE_2D, this->textureID);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 800, 600, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/*glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);*/

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->textureID, 0);

	/*glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->textureID, 0);*/
	/*glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
				GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->textureID, 0);*/

	//glEnable(GL_DEPTH_TEST);
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer error!!!" << std::endl;
	}

    glGenRenderbuffers(1, &renderbufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbufferID);


}

void FramebufferManager::WriteTexture(void) {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fboID);
}

void FramebufferManager::defaultFramebuffer(void) {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void FramebufferManager::ReadTexture(GLenum managedTextureUnit) {
	glActiveTexture(managedTextureUnit);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
}

GLuint FramebufferManager::getTextureID(void) {
	return this->textureID;
}
