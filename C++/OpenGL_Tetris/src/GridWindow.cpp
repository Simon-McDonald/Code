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
		currentBlock(height * width - 1) {}

size_t GridWindow::getHeight(void) {
	return this->height;
}

size_t GridWindow::getWidth() {
	return this->width;
}

bool GridWindow::operator() (size_t col, size_t row) {
	if (col >= this->width || row >= this->height) {
		return false;
	}

	return this->isPopulated[row * this->width + col] >= 0;
}

bool GridWindow::updateUserInput(const UserInputStruct &userInput) {return true;}


bool GridWindow::update(double deltaTime_ms, const UserInputStruct & userInput) {
	return true;
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

size_t GridWindow::lockNextPiece(const TetrisPiece &newPiece, const std::pair<size_t, size_t> &piecePosition) {
	for (size_t heightIndex = 0; heightIndex < newPiece.getHeight(); ++heightIndex) {
		for (size_t widthIndex = 0; widthIndex < newPiece.getWidth(); ++widthIndex) {
			if (newPiece(widthIndex, heightIndex)) {
				size_t nextInfoPos = getNextBlock(this->currentBlock);
				size_t boardPos = (piecePosition.first + widthIndex) + (piecePosition.second + heightIndex) * this->width;

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

	std::vector<size_t> fullRows = this->listFullRows();
	size_t numFullRows = fullRows.size();
	this->removeRows(fullRows);

	indexBuffer.resetBuffer(this->height * this->width, &this->blockMatrix[0].blockIndex);

	this->validateBoard();

	return numFullRows;
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
					ERR << "Missing block, containerIdx: " << vecIdx << ", blockId: " << this->blockMatrix[this->isPopulated[vecIdx]].blockIndex << END;
				}

				if (this->blockMatrix[this->isPopulated[vecIdx]].blockIndex != vecIdx) {
					ERR << "Index mismatch, containerIdx: " << vecIdx << ", dataIdx: " << this->isPopulated[vecIdx] << ", act: " << (int) this->blockMatrix[this->isPopulated[vecIdx]].blockIndex << END;
				}
			}
		}
	}
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
			//INFO << "The row: " << row << " is full" << END;
		}
	}

	//INFO << "Got the listed rows!!!" << END;
	return listRows;
}

void GridWindow::removeRows(const std::vector<size_t> &rows) {
	//INFO << "Removing the blocks" << END;

	for (size_t row : rows) {
		for (size_t col = 0; col < this->width; ++col) {
			this->blockMatrix[this->isPopulated[row * this->width + col]].renderBlock = 0;

			this->isPopulated[row * this->width + col] = -1;
		}
	}

	//INFO << "Removed the blocks" << END;

	for (size_t row = 0, lostRows = 0; row < this->height; ++row) {
		//INFO << "Row: " << row << END;

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

void GridWindow::render(void) {
	CHECKERRORS();
	this->getShaderManager().setUniformBool("uUniformSettings", false);
	CHECKERRORS();
	this->getShaderManager().setUniformInt("uNumRotations", 0);
	CHECKERRORS();
	this->getShaderManager().setUniformInt("uBlockMod", this->width);
	CHECKERRORS();
	indexBuffer.manageRender(1, 2, 3, 4, 5, 6);
	CHECKERRORS();
}

void GridWindow::setGridDimsInShader(void) {
	this->getShaderManager().bindVector2i("uWindowDims", this->width, this->height);
}

GridWindow::~GridWindow(void) {
	INFO << "Killing grid window" << END;
}

