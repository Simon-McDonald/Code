/*
 * TetrisCubeBuffer.hpp
 *
 *  Created on: May 7, 2017
 *      Author: Simon
 */

#ifndef DATABUFFER_HPP_
#define DATABUFFER_HPP_

#include <GL/glew.h>

#include <CheckErrors.h>
#include <UtilityManager.hpp>

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
template <typename GLtype, GLuint... elemSizes> class DataBuffer : protected UtilityManager {
public:
    /*
     * Constructor.
     * dataLength: number of elements in buffer
     * strideLength: number of data points that make up an element
     * dataBuffer: pointer to the data to buffer
     */
	DataBuffer(GLuint dataLength, GLuint strideLength, GLtype *dataBuffer, GLenum usage = GL_STATIC_DRAW) :
			bufferId(-1u), numElements(dataLength) {

		if (!dataBuffer || !numElements) {
			numElements = 0;
			return;
		}

		glGenBuffers(1, &this->bufferId);
		glBindBuffer(GL_ARRAY_BUFFER, this->bufferId);

		glBufferData(GL_ARRAY_BUFFER, dataLength * strideLength * sizeof(GLtype), dataBuffer, usage);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	DataBuffer(void) : bufferId(-1u), numElements(0) {}

	/*
	 * Copy assignment and construction disabled. Move assignment and construction only.
	 */
	DataBuffer(const DataBuffer& orig) = delete;
	DataBuffer& operator=(const	DataBuffer& orig) = delete;

	DataBuffer(DataBuffer&& orig) noexcept :
			bufferId(-1u), numElements(0) {

		GLuint tempBufferId = orig.bufferId;
		GLuint tempNumElements = orig.numElements;

		orig.bufferId = this->bufferId;
		orig.numElements = this->numElements;

		this->bufferId = tempBufferId;
		this->numElements = tempNumElements;
	}

	DataBuffer& operator=(DataBuffer&& orig) noexcept {
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
		}

		glBindBuffer(GL_ARRAY_BUFFER, this->bufferId);

		glBufferData(GL_ARRAY_BUFFER, dataLength * strideLength * sizeof(GLtype), dataBuffer, GL_DYNAMIC_DRAW);
		this->numElements = dataLength;

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/*
	 * Base case functions for recursive calls.
	 */
	GLuint getStride(void) const noexcept {
		return 0;
	}

	void bindBuffer() const noexcept {
		if (this->bufferId != -1u) {
			glBindBuffer(GL_ARRAY_BUFFER, this->bufferId);
		}
	}

	void enableLayoutPointers(std::pair<GLuint, GLuint> dimensions) const noexcept {}

    void disableLayoutPointers(void) const noexcept {}

    void unbindBuffer(void) const noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    bool isValid(void) const noexcept {
        return this->bufferId != -1u;
    }

	/*
	 * Render the data. TODO render only part of the data.
	 */
	void render(void) const noexcept {
		if (this->numElements) {
			glDrawArrays(GL_POINTS, 0, this->numElements);
		}
	}

	/*
	 * Only cleanup is deleting the data buffer if one exists.
	 */
	~DataBuffer(void) {
		if (this->bufferId != -1u) {
			glDeleteBuffers(1, &this->bufferId);
			this->bufferId = -1u;
		}
	}

private:
	GLuint bufferId;
	GLuint numElements;
};

/*
 * General case of the recursive class. Recursion achieved through inheritance.
 */
template <typename GLtype, GLuint elem1, GLuint... elemSizes>
struct DataBuffer<GLtype, elem1, elemSizes...> : protected DataBuffer<GLtype, elemSizes...> {
public:
    /*
     * Constructor.
     * dataLength: number of elements in buffer
     * dataBuffer: pointer to the data to buffer
     */
	DataBuffer(GLuint dataLength, GLtype *dataBuffer, GLenum usage = GL_STATIC_DRAW) :
		DataBuffer<GLtype, elemSizes...>(dataLength, elem1, dataBuffer) {}

	DataBuffer(void) : DataBuffer<GLtype, elemSizes...>() {}

	/*
	 * Change the data contained within the buffer.
	 */
	void resetBuffer(int dataLength, GLtype *dataBuffer) {
		this->DataBuffer<GLtype, elemSizes...>::resetBuffer(dataLength, elem1, dataBuffer);
	}

	/*
	 * Bind/unbind buffer.
	 */
    void bindBuffer() const noexcept {
        this->DataBuffer<GLtype, elemSizes...>::bindBuffer();
    }

    void unbindBuffer() const noexcept {
        this->DataBuffer<GLtype, elemSizes...>::unbindBuffer();
    }

	/*
	 * Enables/disables layout pointers. Buffer must have been bound previously.
	 */
	template<typename... Layouts>
	void enableLayoutPointers(GLuint layout1, Layouts... layouts) const noexcept {
		GLuint stride = this->getStride();

		glEnableVertexAttribArray(layout1);
		enableLayoutPointer<GLtype>(layout1, elem1, stride, 0u);

		this->DataBuffer<GLtype, elemSizes...>::enableLayoutPointers(std::make_pair<GLuint, GLuint>(std::move(stride), elem1), layouts...);
	}

    template<typename... Layouts>
    void disableLayoutPointers(int layout1, Layouts... layouts) const noexcept {
        glDisableVertexAttribArray(layout1);
        this->DataBuffer<GLtype, elemSizes...>::disableLayoutPointers(layouts...);
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
		this->DataBuffer<GLtype, elemSizes...>::render();
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

protected:
	/*
	 * Private constructor to assist in recursion.
	 */
	template <typename DataType>
	DataBuffer(GLuint dataLength, GLuint strideLength, DataType *dataBuffer, GLenum usage = GL_STATIC_DRAW) :
		DataBuffer<GLtype, elemSizes...>(dataLength, strideLength + elem1, dataBuffer) {}

	/*
	 * Private resetBuffer function to assist in recursion.
	 */
	template <typename DataType>
	void resetBuffer(int dataLength, int strideLength, DataType *dataBuffer) {
		this->DataBuffer<GLtype, elemSizes...>::resetBuffer(dataLength, strideLength + elem1, dataBuffer);
	}

	/*
	 * Private enableLayoutPointers function to assist in recursion.
	 */
	template<typename... Layouts>
	void enableLayoutPointers(std::pair<GLuint, GLuint> dimensions, GLuint layout1, Layouts... layouts) const noexcept {
		glEnableVertexAttribArray(layout1);
		enableLayoutPointer<GLtype>(layout1, elem1, dimensions.first, dimensions.second);

		this->DataBuffer<GLtype, elemSizes...>::enableLayoutPointers(std::make_pair<GLuint, GLuint>(std::move(dimensions.first), elem1 + dimensions.second), layouts...);
	}

	/*
	 * Helpers.
	 */
	GLuint getStride(void) const noexcept {
		return this->DataBuffer<GLtype, elemSizes...>::getStride() + elem1;
	}

	bool isValid(void) const noexcept {
		return this->DataBuffer<GLtype, elemSizes...>::isValid();
	}
};

#endif /* DATABUFFER_HPP_ */
