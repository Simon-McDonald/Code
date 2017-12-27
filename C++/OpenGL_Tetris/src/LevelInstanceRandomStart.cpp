/*
 * LevelInstanceRandomStart.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Simon
 */

#include "LevelInstanceRandomStart.hpp"

LevelInstanceRandomStart::LevelInstanceRandomStart(size_t rowsToFill) : LevelInstance(),
    generator((unsigned int) time(0)), randomGen(0.0, 1.0) {

    size_t numRows = rowsToFill > this->window.getHeight() ? this->window.getHeight() : rowsToFill;
    std::vector<GLubyte> randomBlocks;

    for (GLubyte row = 0; row < numRows; ++row) {
        std::vector<GLubyte> tempRandomRow = this->generateRandomBlocksRow(
            this->window.getWidth(), row * this->window.getWidth());

        randomBlocks.insert(randomBlocks.end(), tempRandomRow.begin(), tempRandomRow.end());
    }

    this->window.insertIntoBottom(numRows, {150, 150, 150}, randomBlocks);
}

std::vector<GLubyte> LevelInstanceRandomStart::generateRandomBlocksRow(GLubyte width, GLubyte offset) {
    std::vector<GLubyte> blockIndexVec;

    for (GLubyte index = 0; index < width; ++index) {
        float randomNum = this->randomGen(this->generator);

        if (randomNum > 0.5) {
            blockIndexVec.push_back(index + offset);
        }
    }

    if (blockIndexVec.size() == width) {
        size_t randomIndex = (size_t) (this->randomGen(this->generator) * width);

        blockIndexVec.erase(blockIndexVec.begin() + randomIndex);
    }

    return blockIndexVec;
}
