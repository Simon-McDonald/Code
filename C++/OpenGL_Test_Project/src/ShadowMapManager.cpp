/*
 * ShadowMapManager.cpp
 *
 *  Created on: Jun 19, 2016
 *      Author: Simon
 */

#include "ShadowMapManager.h"

#include <iostream>

ShadowMapManager::ShadowMapManager(void) : textureID(-1u), fboID(-1u) {
	std::cout << "This is the shadow map constructor" << std::endl;
}

bool ShadowMapManager::InitialiseShadowMap(void) {
	/*glGenFramebuffers(1, &this->fboID);
	glGenTextures(1, &this->textureID);

	glBindTexture(GL_TEXTURE_2D, this->textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			800, 600, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
			//800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, this->fboID);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->textureID, 0);
	//glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
				//GL_RGB, GL_TEXTURE_2D, this->textureID, 0);

	//glEnable(GL_DEPTH_TEST);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer error!!!" << std::endl;
	}

	this->defaultFramebuffer();*/






    // Create the FBO
    glGenFramebuffers(1, &this->fboID);

    // Create the depth buffer
    glGenTextures(1, &this->textureID);
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 800, 600, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    glBindFramebuffer(GL_FRAMEBUFFER, this->fboID);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->textureID, 0);

    // Disable writes to the color buffer
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);





/*	  //32 bit depth texture, 256x256
	  glGenTextures(1, &this->textureID);
	  glBindTexture(GL_TEXTURE_2D, this->textureID);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	  glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	  //NULL means reserve texture memory, but texels are undefined
	  //You can also try GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT24 for the internal format.
	  //If GL_DEPTH24_STENCIL8_EXT, go ahead and use it (GL_EXT_packed_depth_stencil)
	  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, 800, 600, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
	  //-------------------------
	  glGenFramebuffers(1, &this->fboID);
	  glBindFramebuffer(GL_FRAMEBUFFER, this->fboID);
	  //Attach
	  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->textureID, 0);
	  //-------------------------
	  //Does the GPU support current FBO configuration?
	  //Before checking the configuration, you should call these 2 according to the spec.
	  //At the very least, you need to call glDrawBuffer(GL_NONE)
	  glDrawBuffer(GL_NONE);
	  glReadBuffer(GL_NONE);*/



	////////////////////////////////
	// RENDER TO TEXTURE
	////////////////////////////////

	   //RGBA8 2D texture, 24 bit depth texture, 256x256
	   /*glGenTextures(1, &this->textureID);
	   glBindTexture(GL_TEXTURE_2D, this->textureID);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	   //NULL means reserve texture memory, but texels are undefined
	   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 800, 600, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	   //-------------------------
	   glGenFramebuffers(1, &this->fboID);
	   glBindFramebuffer(GL_FRAMEBUFFER, this->fboID);
	   //Attach 2D texture to this FBO
	   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->textureID, 0);
	   //-------------------------
	   GLuint depth_rb;
	   glGenRenderbuffers(1, &depth_rb);
	   glBindRenderbuffer(GL_RENDERBUFFER, depth_rb);
	   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 800, 600);
	   //-------------------------
	   //Attach depth buffer to FBO
	   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rb);*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*	   	glGenFramebuffers(1, &this->fboID);
	   	glBindFramebuffer(GL_FRAMEBUFFER, this->fboID);

	   	glGenTextures(1, &this->textureID);
	   	glBindTexture(GL_TEXTURE_2D, this->textureID);
	   	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 800, 600, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);////
		   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	   	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->textureID, 0);

	   	GLuint depthrenderbuffer;
	   	glGenRenderbuffers(1, &depthrenderbuffer);
	   	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	   	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 800, 600);////
	   	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);



	   	//GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	   	//glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers*/




















	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		ERR << "Failed to generate the appropiate framebuffer!!!" << std::endl;

		return false;
	} else {
		DEBUG << "Framebuffer successfully generated." << std::endl;
	}

	return true;
}

void ShadowMapManager::BindTexture(void) {
	//glActiveTexture(this->managedTextureUnit);
	//glBindTexture(GL_TEXTURE_2D, this->textureID);
}

void ShadowMapManager::WriteTexture(void) {
	   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->fboID);
	   //glClearColor(0.0, 0.0, 0.0, 0.0);
	   //glClearDepth(1.0f);
	   //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fboID);
	  glDrawBuffer(GL_NONE);
	  glReadBuffer(GL_NONE);*/
}

GLuint ShadowMapManager::getTextureID(void) {
	return this->textureID;
}

void ShadowMapManager::defaultFramebuffer(void) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	  glDrawBuffer(GL_BACK);
	  glReadBuffer(GL_BACK);
}

ShadowMapManager::~ShadowMapManager(void) {
	if (this->fboID != 0) {
		glDeleteFramebuffers(1, &this->fboID);
	}

	if (this->textureID != 0) {
		glDeleteTextures(1, &this->textureID);
	}
}
