/*
 * TetrisPiece.hpp
 *
 *  Created on: Apr 19, 2017
 *      Author: Simon
 */

#ifndef TETRISPIECE_HPP_
#define TETRISPIECE_HPP_

#include <vector>
#include <random>

#include <CheckErrors.h>
#include <UtilityManager.hpp>
#include <WorldManager.hpp>

#include "TetrisPieceTemplate.hpp"
#include "BufferObjects.hpp"

class TetrisPiece : protected UtilityManager, protected WorldManager {
public:
    /*
     * Allow tetris pieces to share the same textures across instances.
     */
	static bool loadConfiguredPieces(void);
	static TetrisPiece generateNextTetrisPiece(void);
	static bool prepShaderTextures(void);
	static GLColour<GLubyte> generateRandomColour(void);

	TetrisPiece& operator=(const TetrisPiece &orig);
	TetrisPiece(const TetrisPiece &orig);

	/*
	 * Basic get functions for piece state.
	 */
	size_t getHeight() const noexcept;
	size_t getWidth() const noexcept;
	bool operator() (size_t row, size_t col) const noexcept;
	int check(size_t row, size_t col) const noexcept;
	std::pair<size_t, size_t> at(size_t pieceIndex) const noexcept;
	size_t getNumBlocks(void) const noexcept;
	std::pair<size_t, size_t> getPieceLoc(void) const noexcept;
	size_t getNumRotations(void) const noexcept;
	GLuint getTextureNumber(void) const noexcept;
	GLColour<GLubyte> getBlockColour(void) const noexcept;

	/*
	 * Basic set functions for piece state.
	 */
	TetrisPiece& rotateCW(void) noexcept;
	TetrisPiece& rotateCCW(void) noexcept;
	TetrisPiece& setPieceLoc(size_t, size_t) noexcept;
	TetrisPiece& movePieceLeft(size_t width) noexcept;
	TetrisPiece& movePieceRight(size_t width) noexcept;
	TetrisPiece& movePieceDown(void) noexcept;

	/*
	 * Render the piece.
	 */
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

	/*
	 * Setup texture information in the shader programs.
	 */
	static bool setupShaderTextures(
	    std::vector<PieceDefinitionInformation> &defInfoVector,
        std::vector<GLuint> &textureIds,
        std::vector<GLint> &textureDims);

	int numCWrotations;
	std::pair<size_t, size_t> pieceLoc;
	TetrisPieceTemplate *pieceTemplate;

	GLColour<GLubyte> pieceColour;

	size_t getColIdxRotation(size_t widthIdx, size_t heightIdx) const noexcept;
	size_t getRowIdxRotation(size_t widthIdx, size_t heightIdx) const noexcept;

	/*
	 * Private constructor to prevent instances being created outside of copies and from generateNextTetrisPiece.
	 */
	TetrisPiece(TetrisPieceTemplate *pieceTemplate, const GLColour<GLubyte> && colour);
};

#endif /* TETRISPIECE_HPP_ */
