/*
 * GridWindow.hpp
 *
 *  Created on: Apr 19, 2017
 *      Author: Simon
 */

#ifndef GRIDWINDOW_HPP_
#define GRIDWINDOW_HPP_

#include "ResourceManager.h"
#include "TetrisPiece.hpp"
#include "DataBuffer.hpp"

#include <array>

class GridWindow : protected UtilityManager, protected WorldManager {
public:
	GridWindow(unsigned height, unsigned width);

	void insertIntoBottom(size_t rowsToShift, Colour colour, std::vector<GLubyte> blockIndices);
	size_t lockNextPiece(const TetrisPiece &newPiece, const std::pair<size_t, size_t> &piecePosition);
	void render(void);
	void setGridDimsInShader(void);
	std::vector<size_t> listFullRows(void);
	void removeRows(const std::vector<size_t> &rows);
	void validateBoard(void);

	void setRowDefault(size_t row, Colour colour);

	int getNextBlock(int curBlock);

	bool operator() (size_t row, size_t col) const;

	size_t getHeight(void) const;
	size_t getWidth(void) const;

	~GridWindow(void);

private:
	struct gridBlock {
		GLubyte blockIndex;
		GLubyte textureIndex;
		GLubyte rotations;
		GLubyte textureNum;
		GLubyte renderBlock;
		Colour blockColour;
	};

	size_t height, width;

	std::vector<gridBlock> blockMatrix;

	DataBuffer<GLubyte, 1, 1, 1, 1, 1, 3> indexBuffer;

	std::vector<int> isPopulated;
	size_t currentBlock;
};

#endif /* GRIDWINDOW_HPP_ */
