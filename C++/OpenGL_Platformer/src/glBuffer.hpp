/*
 * TetrisCubeBuffer.hpp
 *
 *  Created on: May 7, 2017
 *      Author: Simon
 */

#ifndef DATABUFFER_HPP_
#define DATABUFFER_HPP_

#include <utility>

#include <GL/glew.h>

#include <CheckErrors.h>
#include <UtilityManager.hpp>

namespace buf {

/*
 * Conversion functions, from a given OpenGL data type get the appropriate enum identifier.
 */
template <typename GLtype> constexpr GLenum getEnumForType(typename std::enable_if<std::is_same<GLtype, GLfloat>::value, void>::type* = nullptr) { return GL_FLOAT; }
template <typename GLtype> constexpr GLenum getEnumForType(typename std::enable_if<std::is_same<GLtype, GLdouble>::value, void>::type* = nullptr) { return GL_DOUBLE; }
template <typename GLtype> constexpr GLenum getEnumForType(typename std::enable_if<std::is_same<GLtype, GLbyte>::value, void>::type* = nullptr) { return GL_BYTE; }
template <typename GLtype> constexpr GLenum getEnumForType(typename std::enable_if<std::is_same<GLtype, GLubyte>::value, void>::type* = nullptr) { return GL_UNSIGNED_BYTE; }
template <typename GLtype> constexpr GLenum getEnumForType(typename std::enable_if<std::is_same<GLtype, GLshort>::value, void>::type* = nullptr) { return GL_SHORT; }
template <typename GLtype> constexpr GLenum getEnumForType(typename std::enable_if<std::is_same<GLtype, GLushort>::value, void>::type* = nullptr) { return GL_UNSIGNED_SHORT; }
template <typename GLtype> constexpr GLenum getEnumForType(typename std::enable_if<std::is_same<GLtype, GLint>::value, void>::type* = nullptr) { return GL_INT; }
template <typename GLtype> constexpr GLenum getEnumForType(typename std::enable_if<std::is_same<GLtype, GLuint>::value, void>::type* = nullptr) { return GL_UNSIGNED_INT; }

/*
 * Get the size of a given data type.
 */
template <typename GLtype> constexpr GLuint getTypeSize(void) {
	return sizeof(GLtype);
}

/*
 * Wrappers for the glVertexAttribPointer functions based on data type. Accounts for differing data type sizes.
 */
template <typename GLtype>
typename std::enable_if<std::is_same<GLtype, GLfloat>::value || std::is_same<GLtype, GLhalf>::value, void>::type
enableLayoutPointer(const GLuint &layoutId, const GLuint &elemSize,
                    const GLuint &dataStride, const GLuint &dataOffset) {
	GLvoid const *pointer = static_cast<char const*>(0) + dataOffset * getTypeSize<GLtype>();
	glVertexAttribPointer(layoutId, elemSize, getEnumForType<GLtype>(),
	                      GL_FALSE, dataStride * getTypeSize<GLtype>(), pointer);
}

template <typename GLtype>
typename std::enable_if<std::is_same<GLtype, GLbyte>::value || std::is_same<GLtype, GLubyte>::value ||
						std::is_same<GLtype, GLshort>::value || std::is_same<GLtype, GLushort>::value ||
						std::is_same<GLtype, GLint>::value || std::is_same<GLtype, GLuint>::value, void>::type
enableLayoutPointer(const GLuint &layoutId, const GLuint &elemSize,
                    const GLuint &dataStride, const GLuint &dataOffset) {
	GLvoid const *pointer = static_cast<char const*>(0) + dataOffset * getTypeSize<GLtype>();
	glVertexAttribIPointer(layoutId, elemSize, getEnumForType<GLtype>(), dataStride * getTypeSize<GLtype>(), pointer);
}

template <typename GLtype>
typename std::enable_if<std::is_same<GLtype, GLdouble>::value, void>::type
enableLayoutPointer(const GLuint &layoutId, const GLuint &elemSize,
                    const GLuint &dataStride, const GLuint &dataOffset) {
	GLvoid const *pointer = static_cast<char const*>(0) + dataOffset * getTypeSize<GLtype>();
	glVertexAttribLPointer(layoutId, elemSize, getEnumForType<GLtype>(), dataStride * getTypeSize<GLtype>(), pointer);
}

/*
 * Base case of the recursive class.
 */
template <typename GLtype, GLenum bufUsage, GLenum bufType, GLuint... elemSizes> class glBuffer : protected UtilityManager {
public:
    /*
     * Constructor.
     * dataLength: number of elements in buffer
     * strideLength: number of data points that make up an element
     * dataBuffer: pointer to the data to buffer
     */
	glBuffer(GLuint dataLength, GLuint strideLength, GLtype *dataBuffer) :
			bufferId(-1u), numElements(dataLength) {

		if (!dataBuffer || !numElements) {
			numElements = 0;
			return;
		}

		glGenBuffers(1, &this->bufferId);
		glBindBuffer(bufType, this->bufferId);

		glBufferData(bufType, dataLength * strideLength * sizeof(GLtype), dataBuffer, bufUsage);
		glBindBuffer(bufType, 0);
	}

	glBuffer(void) {}

	/*
	 * Copy assignment and construction disabled. Move assignment and construction only.
	 */
	glBuffer(const glBuffer& orig) = delete;
	glBuffer& operator=(const glBuffer& orig) = delete;

	glBuffer(glBuffer&& orig) noexcept :
			bufferId(-1u), numElements(0) {

		GLuint tempBufferId = orig.bufferId;
		GLuint tempNumElements = orig.numElements;

		orig.bufferId = this->bufferId;
		orig.numElements = this->numElements;

		this->bufferId = tempBufferId;
		this->numElements = tempNumElements;
	}

	glBuffer& operator=(glBuffer&& orig) noexcept {
		GLuint tempBufferId = orig.bufferId;
		GLuint tempNumElements = orig.numElements;

		orig.bufferId = this->bufferId;
		orig.numElements = this->numElements;

		this->bufferId = tempBufferId;
		this->numElements = tempNumElements;

		return *this;
	}

	/*
	 * Change the contents of the buffer. Buffer should have been initialised with GL_DYNAMIC_DRAW.
	 */
	void resetBuffer(int dataLength, int strideLength, GLtype *dataBuffer) {
		if (this->bufferId == -1u) {
			glGenBuffers(1, &this->bufferId);
		} else if (bufUsage == GL_STATIC_DRAW) {
		    ERR << "Buffer " << this->bufferId << " is set as static draw and cannot be reset" << END;
		    return;
		}

		this->bindBuffer();

		glBufferData(bufType, dataLength * strideLength * sizeof(GLtype), dataBuffer, bufUsage);
		this->numElements = dataLength;

		this->unbindBuffer();
	}

	/*
	 * Base case functions for recursive calls.
	 */
	GLuint getStride(void) const noexcept {
		return 0;
	}

	void bindBuffer() const noexcept {
		if (this->bufferId != -1u) {
			glBindBuffer(bufType, this->bufferId);
		} else {
		    ERR << "Cannot bind an uncreated buffer" << END;
		}
	}

    void unbindBuffer(void) const noexcept {
        glBindBuffer(bufType, 0);
    }

    bool isValid(void) const noexcept {
        return this->bufferId != -1u;
    }

    /*
     * Render the data.
     */
    void render(GLenum primitive = GL_POINTS, GLint offset = 0) const noexcept {
        this->render(primitive, offset, this->numElements - offset);
    }
	void render(GLenum primitive, GLint offset, GLsizei numElems) const noexcept {
		if (this->isValid()) {
		    glDrawArrays(primitive, offset, numElems);
		}
	}

	/*
	 * Only cleanup is deleting the data buffer if one exists.
	 */
	~glBuffer(void) {
		if (this->bufferId != -1u) {
			glDeleteBuffers(1, &this->bufferId);
			this->bufferId = -1u;
		}
	}

    GLuint getId(void) const noexcept {
        return this->bufferId;
    }

    GLsizei size(void) const noexcept {
        return this->numElements;
    }

protected:
    void _enableLayoutPointers(GLuint stride, GLuint offset) const noexcept {}
    void disableLayoutPointers(void) const noexcept {}

private:
	GLuint bufferId{-1u};
	GLuint numElements{0};
};

/*
 * General case of the recursive class. Recursion achieved through inheritance.
 */
template <typename GLtype, GLenum bufUsage, GLenum bufType, GLuint elem1, GLuint... elemSizes>
struct glBuffer<GLtype, bufUsage, bufType, elem1, elemSizes...> : protected glBuffer<GLtype, bufUsage, bufType, elemSizes...> {
public:
    /*
     * Constructor.
     * dataLength: number of elements in buffer
     * dataBuffer: pointer to the data to buffer
     */
    glBuffer(GLuint dataLength, GLtype *dataBuffer) :
        glBuffer<GLtype, bufUsage, bufType, elemSizes...>(dataLength, elem1, dataBuffer) {}

    glBuffer(void) : glBuffer<GLtype, bufUsage, bufType, elemSizes...>() {}

	/*
	 * Change the data contained within the buffer.
	 */
	void resetBuffer(int dataLength, GLtype *dataBuffer) {
	    this->glBuffer<GLtype, bufUsage, bufType>::resetBuffer(dataLength, this->getStride(), dataBuffer);
	}

	/*
	 * Bind/unbind buffer.
	 */
    void bindBuffer() const noexcept {
        this->glBuffer<GLtype, bufUsage, bufType>::bindBuffer();
    }

    void unbindBuffer() const noexcept {
        this->glBuffer<GLtype, bufUsage, bufType>::unbindBuffer();
    }

	/*
	 * Enables/disables layout pointers. Buffer must have been bound previously.
	 */
	template<typename... Layouts>
	void enableLayoutPointers(Layouts... layouts) const noexcept {
		GLuint stride = this->getStride();
		this->_enableLayoutPointers(stride, 0, layouts...);
	}

    template<typename... Layouts>
    void disableLayoutPointers(int layout1, Layouts... layouts) const noexcept {
        glDisableVertexAttribArray(layout1);
        this->glBuffer<GLtype, bufUsage, bufType, elemSizes...>::disableLayoutPointers(layouts...);
    }

	/*
	 * Bind and then enable layout points. Unbind when done.
	 */
	template<typename... Layouts>
	void bindEnableLayoutPointers(Layouts... layouts) const noexcept {
		this->bindBuffer();
		this->enableLayoutPointers(layouts...);
		this->unbindBuffer();
	}

	/*
	 * Rending the entirety of the buffer.
	 */
	void render(void) const noexcept {
		this->glBuffer<GLtype, bufUsage, bufType>::render();
	}

    /*
     * Bind, enable layout pointers, render, disable layout pointers and unbind buffer. Layout IDs for each part of the
     * data must be listed.
     */
	template<typename... Layouts>
	void manageRender(int layout1, Layouts... layouts) const noexcept {
		if (this->isValid()) {
			CHECKERRORS();
			this->bindBuffer();
			CHECKERRORS();
			this->enableLayoutPointers(layout1, layouts...);
			CHECKERRORS();
			this->render();
			CHECKERRORS();
			this->unbindBuffer();
			CHECKERRORS();
			this->disableLayoutPointers(layout1, layouts...);
			CHECKERRORS();
		}
	}

    /*
     * Helpers.
     */
	GLuint getId(void) const noexcept {
	    return this->glBuffer<GLtype, bufUsage, bufType>::getId();
	}

	GLsizei size(void) const noexcept {
	    return this->glBuffer<GLtype, bufUsage, bufType>::size();
	}

    GLuint getStride(void) const noexcept {
        return this->glBuffer<GLtype, bufUsage, bufType, elemSizes...>::getStride() + elem1;
    }

    bool isValid(void) const noexcept {
        return this->glBuffer<GLtype, bufUsage, bufType>::isValid();
    }

protected:
	/*
	 * Private constructor to assist in recursion.
	 */
	template <typename DataType>
	glBuffer(GLuint dataLength, GLuint strideLength, DataType *dataBuffer) :
	glBuffer<GLtype, bufUsage, bufType, elemSizes...>(dataLength, strideLength + elem1, dataBuffer) {}

	/*
	 * Private resetBuffer function to assist in recursion.
	 */
	/*template <typename DataType>
	void resetBuffer(int dataLength, int strideLength, DataType *dataBuffer) {
		this->glBuffer<GLtype, bufUsage, bufType, elemSizes...>::resetBuffer(dataLength, strideLength + elem1, dataBuffer);
	}*/

	/*
	 * Private enableLayoutPointers function to assist in recursion.
	 */
	template<typename... Layouts>
	void _enableLayoutPointers(GLuint stride, GLuint offset, GLuint layout1, Layouts... layouts) const noexcept {
		glEnableVertexAttribArray(layout1);
		enableLayoutPointer<GLtype>(layout1, elem1, stride, offset);

		this->glBuffer<GLtype, bufUsage, bufType, elemSizes...>::_enableLayoutPointers(stride, elem1 + offset, layouts...);
	}
};

/* Aliased buffers of usage types */
template <typename GLtype, GLenum bufType, GLuint... T>
using staticBuffer = glBuffer<GLtype, GL_STATIC_DRAW, bufType, T...>;

template <typename GLtype, GLenum bufType, GLuint... T>
using dynamicBuffer = glBuffer<GLtype, GL_DYNAMIC_DRAW, bufType, T...>;

/* Aliased uniform buffers. */
template <typename GLtype, GLenum bufUsage, GLuint... T>
using uniformBuffer = glBuffer<GLtype, bufUsage, GL_UNIFORM_BUFFER, T...>;

template <typename GLtype, GLuint... T>
using staticUniformBuffer = uniformBuffer<GLtype, GL_STATIC_DRAW, T...>;

template <typename GLtype, GLuint... T>
using dynamicUniformBuffer = uniformBuffer<GLtype, GL_DYNAMIC_DRAW, T...>;

/* Aliased array buffers. */
template <typename GLtype, GLenum bufUsage, GLuint... T>
using arrayBuffer = buf::glBuffer<GLtype, bufUsage, GL_ARRAY_BUFFER, T...>;

template <typename GLtype, GLuint... T>
using staticArrayBuffer = arrayBuffer<GLtype, GL_STATIC_DRAW, T...>;

template <typename GLtype, GLuint... T>
using dynamicArrayBuffer = arrayBuffer<GLtype, GL_DYNAMIC_DRAW, T...>;

/*
 * ARRAY BUFFER HELPERS
 */
template <typename GLtype, GLenum bufUsage, GLenum bufType, GLuint... elemSizes, typename... Layouts>
void bindArrayBuffer(const glBuffer<GLtype, bufUsage, bufType, elemSizes...> &buffer,
                     Layouts... layouts) {
    _bindArrayBuffer(glBuffer<GLtype, bufUsage, bufType, elemSizes...>{}, buffer.getStride(), 0, layouts...);
}

/* Why is this base case needed even with the size check? Is it a run-time check? */
template <typename GLtype, GLenum bufUsage, GLenum bufType, typename... Layouts>
void _bindArrayBuffer(const glBuffer<GLtype, bufUsage, bufType> &buffer,
                      GLuint stride, GLuint offset, Layouts... layouts) {}

template <typename GLtype, GLenum bufUsage, GLenum bufType, GLuint elem1, GLuint... elemSizes,
          typename Layout1, typename... Layouts>
void _bindArrayBuffer(const glBuffer<GLtype, bufUsage, bufType, elem1, elemSizes...> &buffer,
                      GLuint stride, GLuint offset, Layout1 layout1, Layouts... layouts) {
    glEnableVertexAttribArray(layout1);
    enableLayoutPointer<GLtype>(layout1, elem1, stride, offset);

    _bindArrayBuffer(glBuffer<GLtype, bufUsage, bufType, elemSizes...>{}, stride, elem1 + offset, layouts...);
}

template <typename... Layouts>
void unbindArrayBuffer(Layouts... layouts) {}

template <typename Layout1, typename... Layouts>
void unbindArrayBuffer(Layout1 layout1, Layouts... layouts) {
    glDisableVertexAttribArray(layout1);

    unbindArrayBuffer(layouts...);
}

template <typename GLtype, GLenum bufUsage, GLuint... elemSizes>
void renderArrayBuffer(const glBuffer<GLtype, bufUsage, GL_ARRAY_BUFFER, elemSizes...> &buffer,
                       GLenum primitive = GL_POINTS, GLint offset = 0, GLsizei numElems = -1u) {
    if (buffer.isValid()) {
        GLsizei newSize = (numElems == -1) ? buffer.size() - offset : numElems;

        glDrawArrays(primitive, offset, newSize);
    }
}

template <typename GLtype, GLenum bufUsage, GLuint... elemSizes, typename... Layouts>
void manageRenderArray(const glBuffer<GLtype, bufUsage, GL_ARRAY_BUFFER, elemSizes...> &buffer,
                       GLenum primitive, Layouts... layouts) {
    if (buffer.isValid()) {
        CHECKERRORS();
        buffer.bindBuffer();
        CHECKERRORS();
        bindArrayBuffer(buffer, layouts...);
        CHECKERRORS();
        renderArrayBuffer(buffer, primitive);
        CHECKERRORS();
        buffer.unbindBuffer();
        CHECKERRORS();
        unbindArrayBuffer(layouts...);
        CHECKERRORS();
    }
}

/*
 * UNIFORM BUFFER HELPERS
 */
template <typename GLtype, GLenum bufUsage, GLuint... elemSizes>
void bindUniformBuffer(glBuffer<GLtype, bufUsage, GL_UNIFORM_BUFFER, elemSizes...> &buffer,
                       GLuint shaderId, GLuint uniformBlockIndex, std::string uniformBlockName) {
    GLuint blockIndex = glGetUniformBlockIndex(shaderId, uniformBlockName.c_str());
    glUniformBlockBinding(shaderId, blockIndex, uniformBlockIndex);
    glBindBufferBase(GL_UNIFORM_BUFFER, uniformBlockIndex, buffer.getId());
}

};

#endif /* DATABUFFER_HPP_ */
