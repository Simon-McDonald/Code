/*
 * TetrisPieceTexture.hpp
 *
 *  Created on: Jul 9, 2017
 *      Author: Simon
 */

#ifndef TETRISPIECETEXTURE_HPP_
#define TETRISPIECETEXTURE_HPP_

class TetrisPieceTexture : protected UtilityManager {
public:
	struct textureDefinition {
		std::string textureName;
		size_t gridHeight;
		size_t gridWidth;
	};

	TetrisPiece(std::vector<std::pair<std::string, std::pair<size_t, size_t>>> textureList)




};

#endif /* TETRISPIECETEXTURE_HPP_ */
