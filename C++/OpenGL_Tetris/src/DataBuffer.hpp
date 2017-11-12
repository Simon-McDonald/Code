/*
 * TetrisCubeBuffer.hpp
 *
 *  Created on: May 7, 2017
 *      Author: Simon
 */

#ifndef DATABUFFER_HPP_
#define DATABUFFER_HPP_

#include <GL/glew.h>
#include <glm/vec3.hpp>

#include "CheckErrors.h"
#include <UtilityManager.hpp>


struct Colour {
	GLubyte r;
	GLubyte g;
	GLubyte b;
};

/*template<class T>
struct check
{
	template<class Q = T>
	bool test1(typename std::enable_if<std::is_same<Q, int>::value, bool>::type* = nullptr)
	{
		return true;
	}

	template<class Q = T>
	bool test1(typename std::enable_if<!std::is_same<Q, int>::value, bool>::type* = nullptr)
	{
		return false;
	}

	template<class Q = T>
    typename std::enable_if<std::is_same<Q, int>::value, bool>::type test2()
    {
		return true;
    }

    template<class Q = T>
    typename std::enable_if<!std::is_same<Q, int>::value, bool>::type test2()
    {
		return false;
    }
};*/


template <typename GLtype> constexpr GLenum getEnumForType(typename std::enable_if<std::is_same<GLtype, GLfloat>::value, void>::type* = nullptr) { return GL_FLOAT; }
template <typename GLtype> constexpr GLenum getEnumForType(typename std::enable_if<std::is_same<GLtype, GLdouble>::value, void>::type* = nullptr) { return GL_DOUBLE; }
template <typename GLtype> constexpr GLenum getEnumForType(typename std::enable_if<std::is_same<GLtype, GLbyte>::value, void>::type* = nullptr) { return GL_BYTE; }
template <typename GLtype> constexpr GLenum getEnumForType(typename std::enable_if<std::is_same<GLtype, GLubyte>::value, void>::type* = nullptr) { return GL_UNSIGNED_BYTE; }
template <typename GLtype> constexpr GLenum getEnumForType(typename std::enable_if<std::is_same<GLtype, GLshort>::value, void>::type* = nullptr) { return GL_SHORT; }
template <typename GLtype> constexpr GLenum getEnumForType(typename std::enable_if<std::is_same<GLtype, GLushort>::value, void>::type* = nullptr) { return GL_UNSIGNED_SHORT; }
template <typename GLtype> constexpr GLenum getEnumForType(typename std::enable_if<std::is_same<GLtype, GLint>::value, void>::type* = nullptr) { return GL_INT; }
template <typename GLtype> constexpr GLenum getEnumForType(typename std::enable_if<std::is_same<GLtype, GLuint>::value, void>::type* = nullptr) { return GL_UNSIGNED_INT; }

template <typename GLtype> constexpr GLuint getTypeSize(void) {
	return sizeof(GLtype);
}

// ADD IN GLtype size
template <typename GLtype>
typename std::enable_if<std::is_same<GLtype, GLfloat>::value || std::is_same<GLtype, GLhalf>::value, void>::type
enableLayoutPointer(const GLuint &layoutId, const GLuint &elemSize, const GLuint &dataStride, const GLuint &dataOffset) {
	GLvoid const *pointer = static_cast<char const*>(0) + dataOffset * getTypeSize<GLtype>();
	glVertexAttribPointer(layoutId, elemSize, getEnumForType<GLtype>(), GL_FALSE, dataStride * getTypeSize<GLtype>(), pointer);
}

template <typename GLtype>
typename std::enable_if<std::is_same<GLtype, GLbyte>::value || std::is_same<GLtype, GLubyte>::value ||
						std::is_same<GLtype, GLshort>::value || std::is_same<GLtype, GLushort>::value ||
						std::is_same<GLtype, GLint>::value || std::is_same<GLtype, GLuint>::value, void>::type
enableLayoutPointer(const GLuint &layoutId, const GLuint &elemSize, const GLuint &dataStride, const GLuint &dataOffset) {
	GLvoid const *pointer = static_cast<char const*>(0) + dataOffset * getTypeSize<GLtype>();
	glVertexAttribIPointer(layoutId, elemSize, getEnumForType<GLtype>(), dataStride * getTypeSize<GLtype>(), pointer);
}

template <typename GLtype>
typename std::enable_if<std::is_same<GLtype, GLdouble>::value, void>::type
enableLayoutPointer(const GLuint &layoutId, const GLuint &elemSize, const GLuint &dataStride, const GLuint &dataOffset) {
	GLvoid const *pointer = static_cast<char const*>(0) + dataOffset * getTypeSize<GLtype>();
	glVertexAttribLPointer(layoutId, elemSize, getEnumForType<GLtype>(), dataStride * getTypeSize<GLtype>(), pointer);
}

/*template <GLenum glDataType, typename T = void,
		typename std::enable_if<glDataType == GL_FLOAT || glDataType == GL_HALF_FLOAT, T>::type* = nullptr>
void enableLayoutPointer(GLuint &layoutId, GLuint elemSize, GLuint &dataStride, GLuint dataOffset) {
	GLvoid const *pointer = static_cast<char const*>(0) + dataOffset;
	glVertexAttribPointer(layoutId, elemSize, glDataType, GL_FALSE, dataStride, pointer);
}

template <GLenum glDataType, typename T = void,
		typename std::enable_if<glDataType == GL_BYTE || glDataType == GL_UNSIGNED_BYTE ||
		glDataType == GL_SHORT || glDataType == GL_UNSIGNED_SHORT ||
		glDataType == GL_INT || glDataType == GL_UNSIGNED_INT, T>::type* = nullptr>
void enableLayoutPointer(GLuint &layoutId, GLuint elemSize, GLuint &dataStride, GLuint dataOffset) {
	GLvoid const *pointer = static_cast<char const*>(0) + dataOffset;
	glVertexAttribIPointer(layoutId, elemSize, glDataType, dataStride, pointer);
}

template <GLenum glDataType, typename T = void, typename std::enable_if<glDataType == GL_DOUBLE, T>::type* = nullptr>
void enableLayoutPointer(GLuint &layoutId, GLuint elemSize, GLuint &dataStride, GLuint dataOffset) {
	GLvoid const *pointer = static_cast<char const*>(0) + dataOffset;
	glVertexAttribLPointer(layoutId, elemSize, glDataType, dataStride, pointer);
}*/




/*template <GLenum glDataType, GLuint... elemSizes> class DataBufferTemplate : protected UtilityManager {
public:
	template <typename DataType>
	DataBufferTemplate(GLuint dataLength, GLuint strideLength, DataType *dataBuffer, GLenum usage = GL_STATIC_DRAW) :
			bufferId(-1u), numElements(dataLength) {

		if (!dataBuffer || !numElements) {
			numElements = 0;
			return;
		}

		glGenBuffers(1, &this->bufferId);
		glBindBuffer(GL_ARRAY_BUFFER, this->bufferId);

		glBufferData(GL_ARRAY_BUFFER, dataLength * strideLength * sizeof(DataType), dataBuffer, usage);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	DataBufferTemplate(void) : bufferId(-1u), numElements(0) {
		//glGenBuffers(1, &this->bufferId);
	}

	//DataBufferTemplate(const DataBufferTemplate& orig) = delete;
	//DataBufferTemplate& operator=(const	DataBufferTemplate& orig) = delete;

	DataBufferTemplate(const DataBufferTemplate& orig) {
		this->bufferId = orig.bufferId;
		this->numElements = orig.numElements;
	}

	DataBufferTemplate& operator=(const	DataBufferTemplate& orig) {
		this->bufferId = orig.bufferId;
		this->numElements = orig.numElements;
		return *this;
	}

	DataBufferTemplate(DataBufferTemplate&& orig) noexcept :
			bufferId(-1u), numElements(0) {

		GLuint tempBufferId = orig.bufferId;
		GLuint tempNumElements = orig.numElements;

		orig.bufferId = this->bufferId;
		orig.numElements = this->numElements;

		this->bufferId = tempBufferId;
		this->numElements = tempNumElements;
	}

	DataBufferTemplate& operator=(DataBufferTemplate&& orig) noexcept {
		INFO << "Move Assignment: " << this->bufferId << END;

		GLuint tempBufferId = orig.bufferId;
		GLuint tempNumElements = orig.numElements;

		orig.bufferId = this->bufferId;
		orig.numElements = this->numElements;

		this->bufferId = tempBufferId;
		this->numElements = tempNumElements;

		return *this;
	}

	template <typename DataType>
	void resetBuffer(int dataLength, int strideLength, DataType *dataBuffer) {
		//INFO << "Create DataBuffer, numNodes: " << dataLength << ", stride: " << strideLength << " --> ";
		//for (size_t idx = 0, lim = dataLength * strideLength; idx < lim; ++idx) {
		//	APPEND << (int) dataBuffer[idx] << ", ";
		//}
		//APPEND << END;

		if (this->bufferId == -1u) {
			INFO << "NEw buffer!!!" << END;
			glGenBuffers(1, &this->bufferId);
		}

		glBindBuffer(GL_ARRAY_BUFFER, this->bufferId);


		//INFO << "Length: " << (dataLength * strideLength * sizeof(DataType)) << END;
		glBufferData(GL_ARRAY_BUFFER, dataLength * strideLength * sizeof(DataType), dataBuffer, GL_STATIC_DRAW);
		this->numElements = dataLength;
		//INFO << "Current buffer: " << this->bufferId << END;
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	GLuint getStride(void) const noexcept {
		return 0;
	}

	void bindBuffer() const noexcept {
		if (this->bufferId != -1u) {
			glBindBuffer(GL_ARRAY_BUFFER, this->bufferId);
		}
	}

	void enableLayoutPointers(std::pair<GLuint, GLuint> dimensions) const noexcept {}

	void render(void) const noexcept {
		if (this->numElements) {
			glDrawArrays(GL_POINTS, 0, this->numElements);
			//INFO << "Elements: " << numElements << END;
		}
	}

	void disableLayoutPointers(void) const noexcept {}

	void unbindBuffer(void) const noexcept {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	bool isValid(void) const noexcept {
		return this->bufferId != -1u;
	}

	~DataBufferTemplate(void) {
		if (this->bufferId != -1u) {
			INFO << "Deleting: " << this->bufferId << END;
			glDeleteBuffers(1, &this->bufferId);
			this->bufferId = -1u;
		}
	}

private:
	GLuint bufferId;
	GLuint numElements;
};

template <GLenum glDataType, GLuint elem1, GLuint... elemSizes>
struct DataBufferTemplate<glDataType, elem1, elemSizes...> : protected DataBufferTemplate<glDataType, elemSizes...> {
public:
	template <typename DataType>
	DataBufferTemplate(GLuint dataLength, DataType *dataBuffer, GLenum usage = GL_STATIC_DRAW) :
			DataBufferTemplate<glDataType, elemSizes...>(dataLength, elem1, dataBuffer) {}

	DataBufferTemplate(void) : DataBufferTemplate<glDataType, elemSizes...>() {}

	//DataBufferTemplate(DataBufferTemplate<glDataType, elem1, elemSizes...>&& orig) noexcept :
	//		DataBufferTemplate<glDataType, elemSizes...>(std::move(orig.DataBufferTemplate<glDataType, elemSizes...>)) {}

	template <typename DataType>
	void resetBuffer(int dataLength, DataType *dataBuffer) {
		this->DataBufferTemplate<glDataType, elemSizes...>::resetBuffer(dataLength, elem1, dataBuffer);
	}

	template<typename... Layouts>
	void enableLayoutPointers(GLuint layout1, Layouts... layouts) const noexcept {
		GLuint stride = this->getStride();

		glEnableVertexAttribArray(layout1);
		enableLayoutPointer<glDataType>(layout1, elem1, stride, 0u);

		this->DataBufferTemplate<glDataType, elemSizes...>::enableLayoutPointers(std::make_pair<GLuint, GLuint>(std::move(stride), elem1), layouts...);
	}

	void render(void) const noexcept {
		this->DataBufferTemplate<glDataType, elemSizes...>::render();
	}

	template<typename... Layouts>
	void disableLayoutPointers(int layout1, Layouts... layouts) const noexcept {
		glDisableVertexAttribArray(layout1);
		this->DataBufferTemplate<glDataType, elemSizes...>::disableLayoutPointers(layouts...);
	}

	template<typename... Layouts>
	void manageRender(int layout1, Layouts... layouts) const noexcept {
		if (this->isValid()) {
			this->bindBuffer();
			this->enableLayoutPointers(layout1, layouts...);
			this->unbindBuffer();
			CHECKERRORS();

			this->render();
			CHECKERRORS();

			this->disableLayoutPointers(layout1, layouts...);
			CHECKERRORS();
		}
	}

	void bindBuffer() const noexcept {
		this->DataBufferTemplate<glDataType, elemSizes...>::bindBuffer();
	}

	void unbindBuffer() const noexcept {
		this->DataBufferTemplate<glDataType, elemSizes...>::unbindBuffer();
	}

protected:
	template <typename DataType>
	DataBufferTemplate(GLuint dataLength, GLuint strideLength, DataType *dataBuffer, GLenum usage = GL_STATIC_DRAW) :
			DataBufferTemplate<glDataType, elemSizes...>(dataLength, strideLength + elem1, dataBuffer) {}

	template <typename DataType>
	void resetBuffer(int dataLength, int strideLength, DataType *dataBuffer) {
		this->DataBufferTemplate<glDataType, elemSizes...>::resetBuffer(dataLength, strideLength + elem1, dataBuffer);
	}

	template<typename... Layouts>
	void enableLayoutPointers(std::pair<GLuint, GLuint> dimensions, GLuint layout1, Layouts... layouts) const noexcept {
		glEnableVertexAttribArray(layout1);
		enableLayoutPointer<glDataType>(layout1, elem1, dimensions.first, dimensions.second);

		this->DataBufferTemplate<glDataType, elemSizes...>::enableLayoutPointers(std::make_pair<GLuint, GLuint>(std::move(dimensions.first), elem1 + dimensions.second), layouts...);
	}

	GLuint getStride(void) const noexcept {
		return this->DataBufferTemplate<glDataType, elemSizes...>::getStride() + elem1;
	}

	bool isValid(void) const noexcept {
		return this->DataBufferTemplate<glDataType, elemSizes...>::isValid();
	}
};*/

/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/

// GLenum glDataType should become getEnumForType(GLtype)
template <typename GLtype, GLuint... elemSizes> class DataBuffer : protected UtilityManager {
public:
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

	DataBuffer(void) : bufferId(-1u), numElements(0) {
		//glGenBuffers(1, &this->bufferId);
	}

	DataBuffer(const DataBuffer& orig) = delete;
	DataBuffer& operator=(const	DataBuffer& orig) = delete;

	/*DataBuffer(const DataBuffer& orig) {
		this->bufferId = orig.bufferId;
		this->numElements = orig.numElements;
	}

	DataBuffer& operator=(const	DataBuffer& orig) {
		this->bufferId = orig.bufferId;
		this->numElements = orig.numElements;
		return *this;
	}*/

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
		INFO << "Move Assignment: " << this->bufferId << END;

		GLuint tempBufferId = orig.bufferId;
		GLuint tempNumElements = orig.numElements;

		orig.bufferId = this->bufferId;
		orig.numElements = this->numElements;

		this->bufferId = tempBufferId;
		this->numElements = tempNumElements;

		return *this;
	}

	void resetBuffer(int dataLength, int strideLength, GLtype *dataBuffer) {
		if (this->bufferId == -1u) {
			//INFO << "New buffer!!!" << END;
			glGenBuffers(1, &this->bufferId);
		}

		glBindBuffer(GL_ARRAY_BUFFER, this->bufferId);

		glBufferData(GL_ARRAY_BUFFER, dataLength * strideLength * sizeof(GLtype), dataBuffer, GL_STATIC_DRAW);
		this->numElements = dataLength;

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	GLuint getStride(void) const noexcept {
		return 0;
	}

	void bindBuffer() const noexcept {
		if (this->bufferId != -1u) {
			glBindBuffer(GL_ARRAY_BUFFER, this->bufferId);
		}
	}

	void enableLayoutPointers(std::pair<GLuint, GLuint> dimensions) const noexcept {}

	void render(void) const noexcept {
		if (this->numElements) {
			//INFO << "Render: " << this->bufferId << END;
			glDrawArrays(GL_POINTS, 0, this->numElements);
		}
	}

	void disableLayoutPointers(void) const noexcept {}

	void unbindBuffer(void) const noexcept {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	bool isValid(void) const noexcept {
		return this->bufferId != -1u;
	}

	~DataBuffer(void) {
		if (this->bufferId != -1u) {
			INFO << "Deleting: " << this->bufferId << END;
			glDeleteBuffers(1, &this->bufferId);
			this->bufferId = -1u;
		}
	}

private:
	GLuint bufferId;
	GLuint numElements;
};

template <typename GLtype, GLuint elem1, GLuint... elemSizes>
struct DataBuffer<GLtype, elem1, elemSizes...> : protected DataBuffer<GLtype, elemSizes...> {
public:
	DataBuffer(GLuint dataLength, GLtype *dataBuffer, GLenum usage = GL_STATIC_DRAW) :
		DataBuffer<GLtype, elemSizes...>(dataLength, elem1, dataBuffer) {}

	DataBuffer(void) : DataBuffer<GLtype, elemSizes...>() {}

	void resetBuffer(int dataLength, GLtype *dataBuffer) {
		this->DataBuffer<GLtype, elemSizes...>::resetBuffer(dataLength, elem1, dataBuffer);
	}

	template<typename... Layouts>
	void enableLayoutPointers(GLuint layout1, Layouts... layouts) const noexcept {
		GLuint stride = this->getStride();

		glEnableVertexAttribArray(layout1);
		enableLayoutPointer<GLtype>(layout1, elem1, stride, 0u);

		this->DataBuffer<GLtype, elemSizes...>::enableLayoutPointers(std::make_pair<GLuint, GLuint>(std::move(stride), elem1), layouts...);
	}

	template<typename... Layouts>
	void bindEnableLayoutPointers(Layouts... layouts) const noexcept {
		this->bindBuffer();
		this->enableLayoutPointers(layouts...);
		this->unbindBuffer();
	}

	void render(void) const noexcept {
		this->DataBuffer<GLtype, elemSizes...>::render();
	}

	template<typename... Layouts>
	void disableLayoutPointers(int layout1, Layouts... layouts) const noexcept {
		glDisableVertexAttribArray(layout1);
		this->DataBuffer<GLtype, elemSizes...>::disableLayoutPointers(layouts...);
	}

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

	void bindBuffer() const noexcept {
		this->DataBuffer<GLtype, elemSizes...>::bindBuffer();
	}

	void unbindBuffer() const noexcept {
		this->DataBuffer<GLtype, elemSizes...>::unbindBuffer();
	}

protected:
	template <typename DataType>
	DataBuffer(GLuint dataLength, GLuint strideLength, DataType *dataBuffer, GLenum usage = GL_STATIC_DRAW) :
		DataBuffer<GLtype, elemSizes...>(dataLength, strideLength + elem1, dataBuffer) {}

	template <typename DataType>
	void resetBuffer(int dataLength, int strideLength, DataType *dataBuffer) {
		this->DataBuffer<GLtype, elemSizes...>::resetBuffer(dataLength, strideLength + elem1, dataBuffer);
	}

	template<typename... Layouts>
	void enableLayoutPointers(std::pair<GLuint, GLuint> dimensions, GLuint layout1, Layouts... layouts) const noexcept {
		glEnableVertexAttribArray(layout1);
		enableLayoutPointer<GLtype>(layout1, elem1, dimensions.first, dimensions.second);

		this->DataBuffer<GLtype, elemSizes...>::enableLayoutPointers(std::make_pair<GLuint, GLuint>(std::move(dimensions.first), elem1 + dimensions.second), layouts...);
	}

	GLuint getStride(void) const noexcept {
		return this->DataBuffer<GLtype, elemSizes...>::getStride() + elem1;
	}

	bool isValid(void) const noexcept {
		return this->DataBuffer<GLtype, elemSizes...>::isValid();
	}
};

#endif /* DATABUFFER_HPP_ */
