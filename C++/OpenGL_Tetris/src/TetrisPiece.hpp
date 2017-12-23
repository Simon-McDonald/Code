/*
 * TetrisPiece.hpp
 *
 *  Created on: Apr 19, 2017
 *      Author: Simon
 */

#ifndef TETRISPIECE_HPP_
#define TETRISPIECE_HPP_

#include <UtilityManager.hpp>
#include <WorldManager.hpp>

#include <vector>
#include <random>

#include "CheckErrors.h"
#include "TetrisPieceTemplate.hpp"

class TetrisPiece : protected UtilityManager, protected WorldManager {
public:
	static bool loadConfiguredPieces(void);
	static TetrisPiece generateNextTetrisPiece(void);
	static bool prepShaderTextures(void);
	static Colour generateRandomColour(void);

	TetrisPiece& operator=(const TetrisPiece &orig);
	TetrisPiece(const TetrisPiece &orig);

	size_t getHeight() const noexcept;
	size_t getWidth() const noexcept;
	bool operator() (size_t row, size_t col) const noexcept;
	int check(size_t row, size_t col) const noexcept;
	std::pair<size_t, size_t> at(size_t pieceIndex) const noexcept;
	size_t getNumBlocks(void) const noexcept;
	std::pair<size_t, size_t> getPieceLoc(void) const noexcept;
	size_t getNumRotations(void) const noexcept;
	GLuint getTextureNumber(void) const noexcept;
	Colour getBlockColour(void) const noexcept;

	TetrisPiece& rotateCW(void) noexcept;
	TetrisPiece& rotateCCW(void) noexcept;
	TetrisPiece& setPieceLoc(size_t, size_t) noexcept;
	TetrisPiece& movePieceLeft(size_t width) noexcept;
	TetrisPiece& movePieceRight(size_t width) noexcept;
	TetrisPiece& movePieceDown(void) noexcept;

	void RenderPiece(void) noexcept;

	~TetrisPiece(void);

private:
	struct PieceDefinitionInformation {
		std::string layoutDefinition;
		std::string imageFile;
		std::pair<GLuint, GLuint> textureDims;

		PieceDefinitionInformation(std::string layoutDefinition, std::string imageFile, std::pair<GLuint, GLuint> textureDims) :
				layoutDefinition(layoutDefinition), imageFile(imageFile), textureDims(textureDims) {}
	};

	static std::vector<TetrisPieceTemplate> pieceDefinitions;
	static std::vector<GLuint> pieceTextureIds;
	static std::vector<GLint> pieceTextureDims;

	static int count;
	static std::default_random_engine generator;
	static std::uniform_int_distribution<size_t> randomGen;
	static std::uniform_int_distribution<size_t> randomGen2;
	static std::uniform_int_distribution<size_t> randomGen3;

	static bool setupShaderTextures(std::vector<PieceDefinitionInformation> &defInfoVector,
								  std::vector<GLuint> &textureIds,
								  std::vector<GLint> &textureDims);

	int numCWrotations;
	std::pair<size_t, size_t> pieceLoc;
	TetrisPieceTemplate *pieceTemplate;

	Colour pieceColour;

	size_t getColIdxRotation(size_t widthIdx, size_t heightIdx) const noexcept;
	size_t getRowIdxRotation(size_t widthIdx, size_t heightIdx) const noexcept;

	TetrisPiece(TetrisPieceTemplate *pieceTemplate, const Colour && colour);
};

#endif /* TETRISPIECE_HPP_ */
