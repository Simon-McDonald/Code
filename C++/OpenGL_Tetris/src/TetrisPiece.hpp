/*
 * TetrisPiece.hpp
 *
 *  Created on: Apr 19, 2017
 *      Author: Simon
 */

#ifndef TETRISPIECE_HPP_
#define TETRISPIECE_HPP_

#include <cassert>
#include <stdexcept>
#include <vector>
#include <random>
#include <array>
#include "DataBuffer.hpp"
#include "CheckErrors.h"
#include "ResourceManager.h"
#include <UtilityManager.hpp>
#include <WorldManager.hpp>

/* Class to define a generic 'template' for a tetris piece. This 'template' defines the size, profile and behaviour of
 * a piece that can be used in the game. The piece definitions are read from the configuration file. The 'template'
 * works by defining a height and width for a piece corresponding to a height by width box that the piece will occupy
 * a portion of. The positions that the piece occupies are determined by polling the piece with offsets from the top
 * left corner of the piece.
 *
 * 		+-+-+-+-+
 * 	  2 |x|x|x|x|
 * 		+-+-+-+-+
 * 	  1 | |x| | |
 * 		+-+-+-+-+
 * 	  0 | |x| | |
 * 		+-+-+-+-+
 * 		 0 1 2 3
 */
class TetrisPieceTemplate : protected UtilityManager, protected WorldManager {
public:
//	using outlineType = std::array<int, 25>;
	using outlineType = std::vector<int>;

	static size_t calculateHeight(const std::string &templateString) {
		if (!templateString.size()) {
			return 0;
		}

		size_t heightCount = 0;

		for (size_t currentIndex = 0;
				currentIndex != std::string::npos;
				currentIndex = templateString.find_first_of(';', currentIndex + 1) ) {
			++heightCount;
		}

		return heightCount;
	}

	static size_t calculateWidth(const std::string &templateString) {
		if (!templateString.size()) {
			return 0;
		}

		size_t tempWidth = 0;

		size_t numCommas = 0;
		size_t numSemiColon = 0;

		for (size_t strIdx = 0; strIdx != templateString.size(); ++strIdx) {
			//INFO << "GOT CHAR " << templateString[strIdx] << END;

			switch (templateString[strIdx]) {
			case ',':
				++numCommas;
				break;
			case ';':
				++numSemiColon;

				if (!tempWidth) {
					tempWidth = numCommas;
				}

				if ((numCommas % numSemiColon) || (numCommas % tempWidth)) {
					std::string errorString = "Comma/Semi-colon mismatch in '" + templateString + "'";
					throw std::invalid_argument(errorString);
					//error
				}

				break;
			}
		}

		if (!tempWidth) {
			tempWidth = numCommas;
		}

		++tempWidth;

		return tempWidth;
	}

	static outlineType determinePieceProfile(const std::string &templateString, size_t width, size_t height) {
		outlineType textureIndices;
		textureIndices.resize(height * width, -1);
		for (auto &bla : textureIndices) {
			bla = -1;
		}

		size_t heightIdx = height - 1;
		size_t widthIdx = 0;

		size_t currentIdx = 0;
		size_t nextBreak = 0;
		size_t nextComma = templateString.find(',', currentIdx);
		size_t nextSemi = templateString.find_first_of(';', currentIdx);

		while(currentIdx < templateString.length()) {
			//INFO << "NextComma: " << nextComma << ", nextSemi: " << nextSemi << ", in string '" << templateString << "'" << END;

			if (nextComma < nextSemi) {
				nextBreak = nextComma;
				nextComma = templateString.find_first_of(',', nextBreak + 1);

				if (nextComma == std::string::npos) {
					nextComma = templateString.length();
				}
			} else {
				nextBreak = nextSemi;
				nextSemi = templateString.find_first_of(';', nextSemi + 1);

				if (nextSemi == std::string::npos) {
					nextSemi = templateString.length();
				}
			}

			if (nextBreak == std::string::npos) {
				nextBreak = templateString.length();
			}

			//INFO << "CurrentIdx: " << currentIdx << ", nextBreak: " << nextBreak << END;
			std::string nextNum = templateString.substr(currentIdx, nextBreak - currentIdx);
			currentIdx = nextBreak + 1;

			//INFO << "Try got value of '" << nextNum << "' from '" << templateString << "'" << END;

			if (nextNum.compare("x")) {
				textureIndices[heightIdx * width + widthIdx] = stoi(nextNum);
				//INFO << "------Add block: (" << (heightIdx * width + widthIdx) << "," << textureIndices[heightIdx * width + widthIdx] << ") - from " << nextNum << END;
				//INFO << "heightIdx: " << heightIdx << " - width: " << width << " - widthIdx: " << widthIdx << END;
				//INFO << "Height: " << height << END;
			}

			++widthIdx;
			heightIdx -= widthIdx / width;
			widthIdx %= width;
		}

		//INFO << "The vector derived from '" << templateString << "', (w,h)=(" << width << "," << height << "), values: ";
		//for (auto bla : textureIndices) {
		//	APPEND << bla << ", ";
		//}
		//APPEND << END;

		return textureIndices;
	}

	static size_t getNumBlocks(outlineType &textureIndices) {
		size_t numBlocks = 0;

		for (auto val : textureIndices) {
			if (val >= 0) {
				++numBlocks;
			}
		}

		return numBlocks;
	}

	static std::vector<size_t> getPieceList(size_t width, size_t height, size_t numBlocks, outlineType pieceProfile) {
		std::vector<size_t> pieceList(numBlocks);
		size_t currIdx = 0;

		for (size_t heightIdx = 0; heightIdx < height; ++heightIdx) {
			for (size_t widthIdx = 0; widthIdx < width; ++widthIdx) {
				size_t blockIdx = widthIdx + heightIdx * width;

				if (pieceProfile[blockIdx] != -1) {
					pieceList[currIdx++] = blockIdx;
				}
			}
		}

		return pieceList;
	}

	/* Define buffer space at the edges of the array in order of sides: Left, Bottom, Right, Top */
	static std::vector<size_t> getEdgeBuffers(size_t width, size_t height) {
		std::vector<size_t> edgeBuff(5);

		if (width > height) {
			edgeBuff[0] = width;
			edgeBuff[2] = (width - height) / 2;
			edgeBuff[4] = width - height - edgeBuff[1];
		} else if (height > width) {
			edgeBuff[0] = height;
			edgeBuff[1] = (height - width) / 2;
			edgeBuff[3] = height - width - edgeBuff[0];
		} else {
			edgeBuff[0] = height;
		}

		return edgeBuff;
	}

	//static DataBufferTemplate<GL_UNSIGNED_BYTE, 1, 1> generateOGLTextureBuffer(size_t width, size_t height, size_t numBlocks, std::vector<size_t> &edgeBuffer, outlineType &pieceProfile) {
	static DataBuffer<GLubyte, 1, 1> generateOGLTextureBuffer(size_t width, size_t height, size_t numBlocks, std::vector<size_t> &edgeBuffer, outlineType &pieceProfile) {
		GLubyte indexData[numBlocks][2];
		size_t arrayIdx = 0;

		for (size_t heightIdx = 0; heightIdx < height; ++heightIdx) {
			for (size_t widthIdx = 0; widthIdx < width; ++widthIdx) {
				int texIdx = pieceProfile[heightIdx * width + widthIdx];
				unsigned blockIdx = widthIdx + edgeBuffer.at(1) + (heightIdx + edgeBuffer.at(2)) * width;

				if (texIdx != -1) {
					indexData[arrayIdx][0] = blockIdx;
					indexData[arrayIdx++][1] = texIdx;
				}
			}
		}

		//return DataBufferTemplate<GL_UNSIGNED_BYTE, 1, 1>(numBlocks, &indexData[0][0]);
		return std::move(DataBuffer<GLubyte, 1, 1>(numBlocks, &indexData[0][0]));
	}

	/* Instantiate a generic piece template with string corresponding to template outline. Commas indicate next column,
	 * semi-colons indicate next row, i.e. above defined with 0,1,2,3;x,4,x,x;x,5,x,x */
	TetrisPieceTemplate(const std::string &templateString, const size_t textureNum) :
		height(calculateHeight(templateString)),
		width(calculateWidth(templateString)),
		pieceProfile(determinePieceProfile(templateString, width, height)),
		numBlocks(getNumBlocks(pieceProfile)),
		pieceList(getPieceList(width, height, numBlocks, pieceProfile)),
		edgeBuffers(getEdgeBuffers(width, height)),
		textureIndexBuffer(generateOGLTextureBuffer(width, height, numBlocks, edgeBuffers, pieceProfile)),
		//layoutDefinition(ResourceManager::loadTexture(textureString))
		textureNumber(textureNum)
	{
		INFO << "Created TetrisPieceTemplate with (w,h) = (" << this->width << "," << this->height << ") numblocks: " << this->numBlocks << END;
	}

	TetrisPieceTemplate(const TetrisPieceTemplate &orig) = delete;
	TetrisPieceTemplate& operator=(const TetrisPieceTemplate &orig) = delete;

	TetrisPieceTemplate(TetrisPieceTemplate &&orig) noexcept : height(orig.height), width(orig.width),
			pieceProfile(orig.pieceProfile), numBlocks(orig.numBlocks), pieceList(orig.pieceList),
			edgeBuffers(orig.edgeBuffers), textureIndexBuffer(std::move(orig.textureIndexBuffer)),
			textureNumber(orig.textureNumber) {}

	TetrisPieceTemplate& operator=(TetrisPieceTemplate &&orig) noexcept {
		this->height = orig.height;
		this->width = orig.width;
		this->pieceProfile = orig.pieceProfile;
		this->numBlocks = orig.numBlocks;
		this->pieceList = orig.pieceList;
		this->edgeBuffers = orig.edgeBuffers;
		this->textureIndexBuffer = std::move(orig.textureIndexBuffer);
		this->textureNumber = orig.textureNumber;

		return *this;
	}

	void RenderPiece(void) {
		CHECKERRORS();

		this->getShaderManager().setUniformBool("uUniformSettings", true);
		this->getShaderManager().setUniformInt("uTexArrayIdx", this->textureNumber);
		//this->getShaderManager().setUniformInt("uTextureMod", 2);
		this->getShaderManager().setUniformInt("uBlockMod", this->width);

		textureIndexBuffer.manageRender(1, 2);
		CHECKERRORS();
	}

	size_t getHeight(void) const {
		return this->edgeBuffers.at(0);
	}

	size_t getWidth(void) const {
		return this->edgeBuffers.at(0);
	}

	bool operator()(size_t col, size_t row) {
		return this->check(col, row) >= 0;
	}

	int check(size_t col, size_t row) {
		if (col < this->edgeBuffers.at(1) || row < this->edgeBuffers.at(2)) {
			return -1;
		}

		if (col >= (this->edgeBuffers.at(1) + this->width) || row >= (this->edgeBuffers.at(2) + this->height)) {
			return -1;
		}

		return pieceProfile.at((row - this->edgeBuffers.at(2)) * this->width + (col - this->edgeBuffers.at(1)));
	}

	std::pair<size_t, size_t> at(size_t idx) {
		size_t piecePos = pieceList.at(idx);

		return std::make_pair<size_t, size_t>((piecePos % this->width) + this->edgeBuffers.at(1), (piecePos / this->width) + this->edgeBuffers.at(2));
	}

	size_t getNumPieces(void) {
		 return this->numBlocks;
	}

	~TetrisPieceTemplate() {}

	size_t getNumBlocks(void) {
		return this->numBlocks;
	}

	GLuint getTextureNumber(void) {
		return this->textureNumber;
	}

private:
	size_t height;
	size_t width;
	outlineType pieceProfile;
	size_t numBlocks;
	std::vector<size_t> pieceList;
	std::vector<size_t> edgeBuffers;

	DataBuffer<GLubyte, 1, 1> textureIndexBuffer;
	GLuint textureNumber;
};

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
	TetrisPiece& movePieceLeft(void) noexcept;
	TetrisPiece& movePieceRight(void) noexcept;
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
