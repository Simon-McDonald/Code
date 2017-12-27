/*
 * GridWindow.hpp
 *
 *  Created on: Apr 19, 2017
 *      Author: Simon
 */

#ifndef GRIDWINDOW_HPP_
#define GRIDWINDOW_HPP_

#include "TetrisPiece.hpp"
#include "DataBuffer.hpp"
#include "ResourceManager.hpp"

class GridWindow : protected UtilityManager, protected WorldManager {
public:
    /*
     * Constructor with height and width of window.
     */
	GridWindow(unsigned height, unsigned width);

    size_t getHeight(void) const;
    size_t getWidth(void) const;

	/*
	 * Check if block at given row/col occupied.
	 */
    bool operator() (size_t row, size_t col) const;

    /*
     * Get the next empty spot in blockMatrix.
     */
    int getNextBlock(int curBlock);

    /*
     * Check that the board indexing appears consistent.
     */
    void validateBoard(void);

	/*
	 * Insert rows into the bottom of the grid. Checks are not done to avoid overriding blocks.
	 * rowsToShift: number of rows that the existing grid will be shifted up by
	 * colour: colour of the new blocks
	 * blockIndices: indices of the blocks to insert, 0 is the bottom left block of the grid.
	 */
	void insertIntoBottom(size_t rowsToShift, GLColour<GLubyte> colour, std::vector<GLubyte> blockIndices);

	/*
	 * Set given row to be full with blocks of given colour. Existing blocks are removed.
	 */
    void setRowDefault(size_t row, GLColour<GLubyte> colour);

    /*
     * Remove list of rows. Shift blocks above rows down.
     */
    void removeRows(const std::vector<size_t> &rows);

	/*
	 * Lock the given piece into the grid. Checks are not done to avoid overriding blocks.
	 * newPiece: piece to insert
	 * piecePosition: position of piece to insert
	 */
	size_t lockNextPiece(const TetrisPiece &newPiece, const std::pair<size_t, size_t> &piecePosition);

	/*
	 * Get a list of full rows.
	 */
    std::vector<size_t> listFullRows(void);

	/*
	 * Render the board
	 */
	void render(void);

	/*
	 * Set uWindowDims in the current program.
	 */
	void setGridDimsInShader(void);

	~GridWindow(void);

private:
	/*
	 * Data required for each individual block rendered in the board.
	 */
	struct gridBlock {
		GLubyte blockIndex;
		GLubyte textureIndex;
		GLubyte rotations;
		GLubyte textureNum;
		GLubyte renderBlock;
		GLColour<GLubyte> blockColour;
	};

	size_t height, width;

	std::vector<gridBlock> blockMatrix;

	DataBuffer<GLubyte, 1, 1, 1, 1, 1, 3> indexBuffer;

	std::vector<int> isPopulated;
	size_t currentBlock;
};

#endif /* GRIDWINDOW_HPP_ */
