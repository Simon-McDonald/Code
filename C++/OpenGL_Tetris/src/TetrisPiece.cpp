/*
 * TetrisPiece.cpp
 *
 *  Created on: Apr 19, 2017
 *      Author: Simon
 */

#include <array>
#include <cassert>
#include <tuple>

#include "DataBuffer.hpp"
#include "TetrisPiece.hpp"

constexpr int stepsIn360deg = 4;

std::vector<TetrisPieceTemplate> TetrisPiece::pieceDefinitions;
std::vector<GLuint> TetrisPiece::pieceTextureIds;
std::vector<GLint> TetrisPiece::pieceTextureDims;

int TetrisPiece::count = 0;
std::uniform_int_distribution<size_t> TetrisPiece::randomGen = std::uniform_int_distribution<size_t>(0, 0);
std::uniform_int_distribution<size_t> TetrisPiece::randomGen2 = std::uniform_int_distribution<size_t>(0, 255);
std::uniform_int_distribution<size_t> TetrisPiece::randomGen3 = std::uniform_int_distribution<size_t>(0, 256);
std::default_random_engine TetrisPiece::generator( (unsigned int) time(0));

GLColour<GLubyte> TetrisPiece::generateRandomColour(void) {
	size_t num1 = TetrisPiece::randomGen2(TetrisPiece::generator);
	size_t num2 = TetrisPiece::randomGen3(TetrisPiece::generator);

	if (num1 + num2 > 255) {
		num1 = TetrisPiece::randomGen2.max() - num1;
		num2 = TetrisPiece::randomGen3.max() - num2;
	}

	GLColour<GLubyte> nextColour = {
			(GLubyte) num1,
			(GLubyte) num2,
			(GLubyte) (255 - num1 - num2)
	};

	return nextColour;
}

bool TetrisPiece::setupShaderTextures(std::vector<PieceDefinitionInformation> &defInfoVector,
									  std::vector<GLuint> &textureIds,
									  std::vector<GLint> &textureDims) {
	textureIds.resize(defInfoVector.size() + 1);
	pieceTextureDims.resize((defInfoVector.size() + 1) * 2);

	bool success = true;

	// Load the texture for generic blocks not part of any piece.
	textureIds[0] = ResourceManager::loadTexture("block.bmp");
	textureDims[0] = 1;
	textureDims[1] = 1;

	for (size_t vecIdx = 0; vecIdx < defInfoVector.size(); ++vecIdx) {
	    size_t vecIdxPlus = vecIdx + 1;

		textureIds[vecIdxPlus] = ResourceManager::loadTexture(defInfoVector[vecIdx].imageFile);
		textureDims[vecIdxPlus * 2] = defInfoVector[vecIdx].textureDims.first;
		textureDims[vecIdxPlus * 2 + 1] = defInfoVector[vecIdx].textureDims.second;
		success &= textureIds[vecIdxPlus] != -1u;
	}

	return success;
}

bool TetrisPiece::loadConfiguredPieces(void) {
	if (TetrisPiece::pieceDefinitions.size()) {
		return true;
	}

	std::vector<PieceDefinitionInformation> defInfoVector;
	defInfoVector.reserve(7);

	// TODO enable pieces to be specified in configuration file rather than hardcoded.
	defInfoVector.emplace_back(PieceDefinitionInformation(std::string("x,1,x;3,4,5"), std::string("tetris-T.bmp"), std::make_pair<GLuint, GLuint>(3, 2)));
	defInfoVector.emplace_back(PieceDefinitionInformation(std::string("x,1,2;3,4,x"), std::string("tetris-S.bmp"), std::make_pair<GLuint, GLuint>(3, 2)));
	defInfoVector.emplace_back(PieceDefinitionInformation(std::string("0,1,x;x,4,5"), std::string("tetris-Z.bmp"), std::make_pair<GLuint, GLuint>(3, 2)));
	defInfoVector.emplace_back(PieceDefinitionInformation(std::string("0,1;2,3"), std::string("tetris-square.bmp"), std::make_pair<GLuint, GLuint>(2, 2)));
	defInfoVector.emplace_back(PieceDefinitionInformation(std::string("x,x,2;3,4,5"), std::string("tetris-L.bmp"), std::make_pair<GLuint, GLuint>(3, 2)));
	defInfoVector.emplace_back(PieceDefinitionInformation(std::string("0,x,x;3,4,5"), std::string("tetris-revL.bmp"), std::make_pair<GLuint, GLuint>(3, 2)));
	defInfoVector.emplace_back(PieceDefinitionInformation(std::string("0,1,2,3"), std::string("tetris-line.bmp"), std::make_pair<GLuint, GLuint>(4, 1)));

	TetrisPiece::setupShaderTextures(defInfoVector, TetrisPiece::pieceTextureIds, TetrisPiece::pieceTextureDims);

	TetrisPiece::pieceDefinitions.reserve(defInfoVector.size());

	for (size_t idx = 0; idx < defInfoVector.size(); ++idx) {
		TetrisPiece::pieceDefinitions.emplace_back(defInfoVector[idx].layoutDefinition, idx + 1);
	}

	TetrisPiece::randomGen = std::uniform_int_distribution<size_t>(0, TetrisPiece::pieceDefinitions.size() - 1);

	return TetrisPiece::pieceDefinitions.size();
}

bool TetrisPiece::prepShaderTextures(void) {
	CHECKERRORS();
	TetrisPiece::getShaderManager().bindVector2iv("uTextureDims", TetrisPiece::pieceTextureDims.size() / 2, &TetrisPiece::pieceTextureDims[0]);
	CHECKERRORS();
	return TetrisPiece::getShaderManager().bindTextureArray("gSamplerArray", TetrisPiece::pieceTextureIds.size(), &TetrisPiece::pieceTextureIds[0], 0);
}

TetrisPiece TetrisPiece::generateNextTetrisPiece(void) {
	if (loadConfiguredPieces()) {
		size_t randIndex = TetrisPiece::randomGen(TetrisPiece::generator);

		return TetrisPiece(&TetrisPiece::pieceDefinitions[randIndex], TetrisPiece::generateRandomColour());
	} else {
		throw std::range_error("No defined piece set found!");
	}
}

TetrisPiece::TetrisPiece(TetrisPieceTemplate *pieceTemplate, const GLColour<GLubyte> && colour) :
		numCWrotations(0), pieceLoc(std::make_pair<size_t, size_t>(0, 0)), pieceTemplate(pieceTemplate),
		pieceColour(colour) {
	TetrisPiece::count += 1;
}

TetrisPiece::TetrisPiece(const TetrisPiece &orig) {
	this->numCWrotations = orig.numCWrotations;
	this->pieceLoc = orig.pieceLoc;
	this->pieceTemplate = orig.pieceTemplate;

	this->pieceColour = orig.pieceColour;

	TetrisPiece::count += 1;
}

TetrisPiece& TetrisPiece::operator=(const TetrisPiece &orig) {
	this->numCWrotations = orig.numCWrotations;
	this->pieceLoc = orig.pieceLoc;
	this->pieceTemplate = orig.pieceTemplate;

	this->pieceColour = orig.pieceColour;

	return *this;
}

size_t TetrisPiece::getHeight() const noexcept {
	return pieceTemplate->getHeight();
}

size_t TetrisPiece::getWidth() const noexcept {
	return pieceTemplate->getWidth();
}

TetrisPiece& TetrisPiece::rotateCW(void) noexcept {
	++numCWrotations;
	numCWrotations %= stepsIn360deg;
	return *this;
}

TetrisPiece& TetrisPiece::rotateCCW(void) noexcept {
	numCWrotations += 3;
	numCWrotations %= stepsIn360deg;
	return *this;
}

size_t TetrisPiece::getNumRotations(void) const noexcept {
	return this->numCWrotations;
}

GLuint TetrisPiece::getTextureNumber(void) const noexcept{
	return this->pieceTemplate->getTextureNumber();
}

std::pair<size_t, size_t> TetrisPiece::getPieceLoc(void) const noexcept {
	return this->pieceLoc;
}

TetrisPiece& TetrisPiece::setPieceLoc(size_t widthIdx, size_t heightIdx) noexcept {
	this->pieceLoc = std::make_pair<size_t, size_t>(std::move(widthIdx), std::move(heightIdx));
	return *this;
}

TetrisPiece& TetrisPiece::movePieceLeft(size_t width) noexcept {
    if ((this->pieceLoc.first + this->getWidth()) == 0) {
        this->pieceLoc.first += width;
    }

	this->pieceLoc.first--;
	return *this;
}

TetrisPiece& TetrisPiece::movePieceRight(size_t width) noexcept {
    if (this->pieceLoc.first == width) {
        this->pieceLoc.first -= width;
    }

	this->pieceLoc.first++;
	return *this;
}

TetrisPiece& TetrisPiece::movePieceDown(void) noexcept {
	this->pieceLoc.second--;
	return *this;
}

GLColour<GLubyte> TetrisPiece::getBlockColour(void) const noexcept {
	return this->pieceColour;
}

bool TetrisPiece::operator() (size_t col, size_t row) const noexcept {
	return this->check(col, row) >= 0;
}

int TetrisPiece::check(size_t col, size_t row) const noexcept {
	return this->pieceTemplate->check(this->getColIdxRotation(col, row), this->getRowIdxRotation(col, row));
}

std::pair<size_t, size_t> TetrisPiece::at(size_t pieceIndex) const noexcept {
	std::pair<size_t, size_t> loc = this->pieceTemplate->at(pieceIndex);

	return std::make_pair<size_t, size_t>(this->getColIdxRotation(loc.first, loc.second), this->getRowIdxRotation(loc.first, loc.second));
}

size_t TetrisPiece::getColIdxRotation(size_t widthIdx, size_t heightIdx) const noexcept {
	switch(this->numCWrotations % stepsIn360deg) {
	case 1:
		return heightIdx;
	case 2:
		return this->pieceTemplate->getWidth() - widthIdx - 1;
	case 3:
		return this->pieceTemplate->getHeight() - heightIdx - 1;
	default: //case 0
		return widthIdx;
	}
}

size_t TetrisPiece::getRowIdxRotation(size_t widthIdx, size_t heightIdx) const noexcept {
	switch(this->numCWrotations % stepsIn360deg) {
	case 1:
		return this->pieceTemplate->getWidth() - widthIdx - 1;
	case 2:
		return this->pieceTemplate->getHeight() - heightIdx - 1;
	case 3:
		return widthIdx;
	default: //case 0
		return heightIdx;
	}
}

void TetrisPiece::RenderPiece(void) noexcept {

	this->getShaderManager().bindVector3i("uFilterColour", this->pieceColour.r, this->pieceColour.g, this->pieceColour.b);

	this->getShaderManager().setUniformInt("uNumRotations", this->numCWrotations);

	this->pieceTemplate->RenderPiece();
}

size_t TetrisPiece::getNumBlocks(void) const  noexcept{
	return pieceTemplate->getNumBlocks();
}

TetrisPiece::~TetrisPiece(void) {
	this->pieceTemplate = nullptr;

	TetrisPiece::count -= 1;

	if (!TetrisPiece::count) {
		INFO << "Clearing tetris piece definitions, size: " << TetrisPiece::count << END;
		pieceDefinitions.clear();
		INFO << "Finished clearing tetris piece definitions" << END;
	}
}
