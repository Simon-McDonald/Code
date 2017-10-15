/*
 * WorldMap.hpp
 *
 *  Created on: Feb 1, 2017
 *      Author: Simon
 */

#ifndef WORLDMAP_HPP_
#define WORLDMAP_HPP_

#include <vector>

#include "matrix.hpp"
#include "UtilityManager.hpp"

/*class TileIndex {
public:
	TileIndex (void) : x(-1), y(-1) {};
	TileIndex (unsigned x, unsigned y) : x(x), y(y) {};

	unsigned X(void) const {
		return this->x;
	}

	unsigned Y(void) const {
		return this->y;
	}

	friend std::ostream& operator<<(std::ostream &os, TileIndex &tile) {
		os << "(" << tile.x << ", " << tile.y << ")" << std::endl;
		return os;
	}

private:
	unsigned x, y;
};*/

class MapTile : protected UtilityManager {
public:
	MapTile(char a) : bla(a), markerFlag(false) {};
	MapTile() : MapTile(' ') {};
	void InitialiseTile (char id) {
		this->bla = id;
	}
	char getBla() const { return bla; };

	void flag(void) {
		this->markerFlag = true;
	}
	void clear(void) {
		this->markerFlag = false;
	}

	bool isFlagged(void) const {
		return this->markerFlag;
	}

	unsigned getCost(void) const {
		return 2;
	}

	friend std::ostream& operator<<(std::ostream& os, const MapTile &tile) {
		os << tile.bla;
		return os;
	}
private:
	char bla;
	bool markerFlag;
};

class WorldMap : protected UtilityManager {
public:
	enum class MapSize {
		Small,
		Medium,
		Large
	};

	enum class MapShape {
		Circular,
		Square,
		Hexagonal
	};

	WorldMap(MapSize size, MapShape shape);

	WorldMap generateCopy(void);

	/*WorldMap(const WorldMap &orig) = delete;
	WorldMap(WorldMap &&orig) = delete;
	WorldMap& operator=(const WorldMap &cpy) = delete;
	WorldMap&& operator=(WorldMap &&orig) = delete;*/

	void PrintTextDisplay(void);

	std::vector<std::pair<matrixIdx<2>, unsigned>> plotPath(const matrixIdx<2> &startIdx, const matrixIdx<2> &endIdx);
	void tagTilesWithinDistance(matrixIdx<2> startPosition, size_t distance);

protected:
	MapTile& at(matrixIdx<2> &tileIdx);
	void clearFlags(void);
	std::vector<matrixIdx<2>> getNeighbours(matrixIdx<2> &tileIndex);

	const size_t height;
	const size_t width;
	matrix<MapTile, 2> map;

	const bool isCopy;

	WorldMap(size_t height, size_t width);
};

#endif /* WORLDMAP_HPP_ */
