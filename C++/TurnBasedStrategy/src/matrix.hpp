/*
 * matrix.hpp
 *
 *  Created on: Feb 8, 2017
 *      Author: Simon
 */

#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <functional>
#include <ostream>

template <size_t dims>
class matrixIdx {
public:
	template <typename... Args>
	matrixIdx(size_t arg1, Args... args) : sizei(arg1), sizen(args...) {
	}

	size_t getSize(void) const {
		return this->sizei;
	}

	size_t getSizei(size_t targetSize) const {
		if (targetSize == 0) {
			return this->sizei;
		}
		return this->sizen.getSizei(targetSize - 1);
	}

	matrixIdx<dims - 1> getSizen(void) const {
		return this->sizen;
	}

	bool operator==(const matrixIdx<dims> &cmpIdx) const {
		return (this->sizei == cmpIdx.sizei) && (this->sizen == cmpIdx.sizen);
	}

	bool operator!=(const matrixIdx<dims> &cmpIdx) const {
		return !(this->operator==(cmpIdx));
	}

	void print(std::ostream &os) {
		os << "(" << this->getSize();
		this->sizen.innerPrint(os);
		os << ")";
	}

	void innerPrint(std::ostream &os) {
		os << ", " << this->getSize();
		this->sizen.innerPrint();
	}

private:
	size_t sizei;
	matrixIdx<dims - 1> sizen;
};

template <>
class matrixIdx<1> {
public:
	matrixIdx(size_t arg1) : sizei(arg1) {
	}

	size_t getSize(void) const {
		return this->sizei;
	}

	size_t getSizei(const size_t targetSize) const {
		return this->getSize();
	}

	size_t getSizen(const size_t targetSize) const {
		return this->getSize();
	}

	bool operator==(const matrixIdx<1> &cmpIdx) const {
		return (this->sizei == cmpIdx.sizei);
	}

	bool operator!=(const matrixIdx<1> &cmpIdx) const {
		return !(this->operator==(cmpIdx));
	}

	void print(std::ostream &os) {
		os << "(" << this->getSize() << ")";
	}

	void innerPrint(std::ostream &os) {
		os << ", " << this->getSize();
	}

private:
	size_t sizei;
};

template <size_t dims>
std::ostream& operator<<(std::ostream& os, matrixIdx<dims> matIdx) {
	matIdx.print(os);
	return os;
}

template <typename T, size_t dims>
class matrix {
public:
	template <typename... Args>
	matrix(T def, size_t arg1, size_t arg2, Args... args) :
			innerMat(arg1, matrix<T, dims - 1>(def, arg2, args...)) {
	}

	template <typename... Args>
	matrix(size_t arg1, size_t arg2, Args... args) :
			innerMat(arg1, matrix<T, dims - 1>(arg2, args...)) {
	}

	template <typename... tupleArgs>
	matrix(T defaultValue, std::tuple<tupleArgs...> sizeTuple) :
			innerMat(std::get<0>(sizeTuple), matrix<T, dims - 1>(defaultValue, std::get<1>(sizeTuple))) {
	}

	template <typename... tupleArgs>
	matrix(std::tuple<tupleArgs...> sizeTuple) :
			innerMat(std::get<0>(sizeTuple), matrix<T, dims - 1>(std::get<1>(sizeTuple))) {
	}

	matrix<T, dims - 1>& operator[](const size_t idx) {
		return this->innerMat[idx];
	}

	void apply(std::function<void(T&)> lambda) {
		for (auto &innerMatElem : innerMat) {
			innerMatElem.apply(lambda);
		}
	}

	T& operator[](const matrixIdx<dims>& matIdx) {
		return this->innerMat[matIdx.getSize()][matIdx.getSizen()];
	}

	T& get(matrixIdx<dims>& matIdx) {
		return this->innerMat[matIdx.getSize()].get(matIdx.getSizen());
	}

	//get iterator stuff

	//check equality?

	auto getSize(void) {
		return std::make_tuple(innerMat.size(), innerMat[0].getSize());
	}
private:
	std::vector<matrix<T, dims - 1>> innerMat;
};

template <typename T>
class matrix<T, 1> {
public:
	matrix (T &def, size_t size) : innerVec(size, def) {
	}

	matrix(size_t size) : innerVec(size) {
	}

	T& operator[](const size_t idx) {
		return this->innerVec[idx];
	}

	T& operator[](const matrixIdx<1> matIdx) {
		return this->innerVec[matIdx.getSize()];
	}

	T& get(const matrixIdx<1> matIdx) {
		return this->innerVec[matIdx.getSize()];
	}

	size_t getSize(void) const {
		return innerVec.size();
	}

	void apply(std::function<void(T&)> lambda) {
		for (auto &elem : innerVec) {
			lambda(elem);
		}
	}

private:
	std::vector<T> innerVec;
};


#endif /* MATRIX_HPP_ */
