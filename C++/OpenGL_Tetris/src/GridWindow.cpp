/*
 * GridWindow.cpp
 *
 *  Created on: Apr 19, 2017
 *      Author: Simon
 */

#include "GridWindow.hpp"

GridWindow::GridWindow(unsigned height, unsigned width) :
		height(height), width(width),
		blockMatrix(height * width * 4, {0}),
		isPopulated(height * width, -1),
		currentBlock(height * width - 1) {
}

size_t GridWindow::getHeight(void) const {
	return this->height;
}

size_t GridWindow::getWidth() const {
	return this->width;
}

bool GridWindow::operator() (size_t col, size_t row) const {
    col %= this->width;
    col = col < 0 ? col + this->width : col;

	if (col >= this->width || row >= this->height) {
		return false;
	}

	return this->isPopulated[row * this->width + col] >= 0;
}

int GridWindow::getNextBlock(int curBlock) {
	size_t numBlocks = this->height * this->width;
	int nextBlock = (curBlock + 1) % numBlocks;

	while (this->blockMatrix[nextBlock].renderBlock != 0 && nextBlock != curBlock) {
		nextBlock = (nextBlock + 1) % numBlocks;
	}

	if (nextBlock == curBlock) {
		return -1;
	}

	return nextBlock;
}

void GridWindow::validateBoard(void) {
    size_t popCount = 0, blockCount = 0;

    for (size_t row = 0; row < this->height; ++row) {
        for (size_t col = 0; col < this->width; ++col) {
            size_t vecIdx = row * this->width + col;

            if (this->isPopulated[vecIdx] >= 0) {
                ++popCount;
            }
            if (this->blockMatrix[vecIdx].renderBlock > 0) {
                ++blockCount;
            }
        }
    }

    if (popCount != blockCount) {
        ERR << "Count mismatch, pop: " << popCount << ", block: " << blockCount << END;
    }

    for (size_t row = 0; row < this->height; ++row) {
        for (size_t col = 0; col < this->width; ++col) {
            size_t vecIdx = row * this->width + col;

            if (this->isPopulated[vecIdx] >= 0) {

                if (this->blockMatrix[this->isPopulated[vecIdx]].renderBlock <= 0) {
                    ERR << "Missing block, containerIdx: " << vecIdx << ", blockId: "
                        << this->blockMatrix[this->isPopulated[vecIdx]].blockIndex << END;
                }

                if (this->blockMatrix[this->isPopulated[vecIdx]].blockIndex != vecIdx) {
                    ERR << "Index mismatch, containerIdx: " << vecIdx << ", dataIdx: " << this->isPopulated[vecIdx]
                        << ", act: " << (int) this->blockMatrix[this->isPopulated[vecIdx]].blockIndex << END;
                }
            }
        }
    }
}

void GridWindow::insertIntoBottom(size_t rowsToShift, GLColour<GLubyte> colour, std::vector<GLubyte> blockIndices) {
    // Remove top 'rowsToShift' number of rows
    for (size_t row = this->height - rowsToShift; row < this->height; ++row) {
        for (size_t col = 0; col < this->width; ++col) {
            if (this->isPopulated[row * this->width + col] != -1) {
                this->blockMatrix[this->isPopulated[row * this->width + col]].renderBlock = 0;
                this->isPopulated[row * this->width + col] = -1;
            }
        }
    }

    // Shift remaining rows up by 'rowsToShift' number of rows
    for (size_t row = this->height - rowsToShift - 1; row < this->height; --row) {
        for (size_t col = 0; col < this->width; ++col) {

            size_t oldBoardPos = row * this->width + col;
            size_t newBoardPos = (row + rowsToShift) * this->width + col;

            if (this->isPopulated[oldBoardPos] >= 0 && oldBoardPos != newBoardPos) {
                this->blockMatrix[this->isPopulated[oldBoardPos]].blockIndex = newBoardPos;

                this->isPopulated[newBoardPos] = this->isPopulated[oldBoardPos];
                this->isPopulated[oldBoardPos] = -1;
            }
        }
    }

    // Insert new pieces
    for (GLubyte boardPos : blockIndices) {
        INFO << "BlockIndices: " << (int)boardPos << END;

        size_t nextInfoPos = getNextBlock(this->currentBlock);

        this->isPopulated[boardPos] = nextInfoPos;

        this->blockMatrix[nextInfoPos].blockIndex = boardPos; // board location
        this->blockMatrix[nextInfoPos].textureIndex = 0;
        this->blockMatrix[nextInfoPos].rotations = 0;
        this->blockMatrix[nextInfoPos].textureNum = 0;
        this->blockMatrix[nextInfoPos].renderBlock = 1;
        this->blockMatrix[nextInfoPos].blockColour = colour;

        this->currentBlock = nextInfoPos;
    }

    // Remove full rows and update buffer
    std::vector<size_t> fullRows = this->listFullRows();
    this->removeRows(fullRows);

    indexBuffer.resetBuffer(this->height * this->width, &this->blockMatrix[0].blockIndex);

    this->validateBoard();
}

void GridWindow::setRowDefault(size_t row, GLColour<GLubyte> colour) {
    for (size_t col = 0; col < this->getWidth(); ++col) {
        size_t boardPos = row * this->width + col;

        // Clear the row
        if (this->isPopulated[row * this->width + col] != -1) {
            this->blockMatrix[this->isPopulated[boardPos]].renderBlock = 0;
            this->isPopulated[boardPos] = -1;
        }

        // Put generic blocks in place
        size_t nextInfoPos = getNextBlock(this->currentBlock);

        this->isPopulated[boardPos] = nextInfoPos;

        this->blockMatrix[nextInfoPos].blockIndex = boardPos; // board location
        this->blockMatrix[nextInfoPos].textureIndex = 0;
        this->blockMatrix[nextInfoPos].rotations = 0;
        this->blockMatrix[nextInfoPos].textureNum = 0;
        this->blockMatrix[nextInfoPos].renderBlock = 1;
        this->blockMatrix[nextInfoPos].blockColour = colour;

        this->currentBlock = nextInfoPos;
    }

    indexBuffer.resetBuffer(this->height * this->width, &this->blockMatrix[0].blockIndex);
    this->validateBoard();
}

void GridWindow::removeRows(const std::vector<size_t> &rows) {
    for (size_t row : rows) {
        for (size_t col = 0; col < this->width; ++col) {
            this->blockMatrix[this->isPopulated[row * this->width + col]].renderBlock = 0;

            this->isPopulated[row * this->width + col] = -1;
        }
    }

    for (size_t row = 0, lostRows = 0; row < this->height; ++row) {
        if ((lostRows < rows.size()) && (row == rows[lostRows])) {
            ++lostRows;
        }

        for (size_t col = 0; col < this->width; ++col) {

            size_t oldBoardPos = row * this->width + col;
            size_t newBoardPos = (row - lostRows) * this->width + col;

            if (this->isPopulated[oldBoardPos] >= 0 && oldBoardPos != newBoardPos) {
                this->blockMatrix[this->isPopulated[oldBoardPos]].blockIndex = newBoardPos;

                this->isPopulated[newBoardPos] = this->isPopulated[oldBoardPos];
                this->isPopulated[oldBoardPos] = -1;
            }
        }
    }
}

size_t GridWindow::lockNextPiece(const TetrisPiece &newPiece, const std::pair<size_t, size_t> &piecePosition) {
	for (size_t heightIndex = 0; heightIndex < newPiece.getHeight(); ++heightIndex) {
		for (size_t widthIndex = 0; widthIndex < newPiece.getWidth(); ++widthIndex) {
			if (newPiece(widthIndex, heightIndex)) {
				size_t nextInfoPos = getNextBlock(this->currentBlock);

				size_t xPos = (piecePosition.first + widthIndex + this->width) % this->width;
				xPos = xPos < 0 ? xPos + this->width : xPos;

				size_t boardPos = xPos + (piecePosition.second + heightIndex) * this->width;

				this->isPopulated[boardPos] = nextInfoPos;

				this->blockMatrix[nextInfoPos].blockIndex = boardPos; // board location
				this->blockMatrix[nextInfoPos].textureIndex = newPiece.check(widthIndex, heightIndex); // texture index
				this->blockMatrix[nextInfoPos].rotations = newPiece.getNumRotations();
				this->blockMatrix[nextInfoPos].textureNum = newPiece.getTextureNumber();
				this->blockMatrix[nextInfoPos].renderBlock = 1;
				this->blockMatrix[nextInfoPos].blockColour = newPiece.getBlockColour();

				this->currentBlock = nextInfoPos;
			}
		}
	}

	// Remove full rows and update data buffer.
	std::vector<size_t> fullRows = this->listFullRows();
	size_t numFullRows = fullRows.size();
	this->removeRows(fullRows);

	indexBuffer.resetBuffer(this->height * this->width, &this->blockMatrix[0].blockIndex);

	this->validateBoard();

	return numFullRows;
}

std::vector<size_t> GridWindow::listFullRows(void) {
	std::vector<size_t> listRows;

	for (size_t row = 0; row < this->height; ++row) {
		bool isFull = true;

		for (size_t col = 0; (col < this->width) && isFull; ++col) {
			isFull &= this->isPopulated[row * this->width + col] >= 0;
		}

		if (isFull) {
			listRows.push_back(row);
		}
	}

	return listRows;
}

void GridWindow::render(void) {
	CHECKERRORS();
	this->getShaderManager().setUniformBool("uUniformSettings", false);
	CHECKERRORS();
	this->getShaderManager().setUniformInt("uBlockMod", this->width);
	CHECKERRORS();
	indexBuffer.manageRender(1, 2, 3, 4, 5, 6);
	CHECKERRORS();
}

void GridWindow::setGridDimsInShader(void) {
	this->getShaderManager().bindVector2i("uWindowDims", this->width, this->height);
}

GridWindow::~GridWindow(void) {}

