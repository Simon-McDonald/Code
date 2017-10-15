/*
 * WorldMap.cpp
 *
 *  Created on: Feb 1, 2017
 *      Author: Simon
 */

#include <algorithm>
#include <queue>

#include "WorldMap.hpp"

unsigned mapSizeToScale (WorldMap::MapSize size) {
	unsigned scaleFactor = 1;

	switch (size) {
	case WorldMap::MapSize::Small:
		scaleFactor = 1;
		break;
	case WorldMap::MapSize::Medium:
		scaleFactor = 2;
		break;
	case WorldMap::MapSize::Large:
		scaleFactor = 3;
		break;
	}

	return scaleFactor;
}

WorldMap::WorldMap(MapSize size, MapShape shape) :
		height(6 * mapSizeToScale(size)), width(6 * mapSizeToScale(size)),
		map(MapTile('o'), height, width), isCopy(false) {
}

WorldMap::WorldMap(size_t height, size_t width) : height(height), width(width), map(MapTile('o'), height, width), isCopy(true) {

}

WorldMap WorldMap::generateCopy(void) {
	WorldMap newMap{this->height, this->width};
	return newMap;
}

void WorldMap::PrintTextDisplay(void) {
	INFO << "Display the Map!" << END;

	for (unsigned hIdx = 0, hLim = 3 * this->height + 1; hIdx < hLim; ++hIdx) {
		APPEND << "+";
		for (unsigned wIdx = 0; wIdx < this->width; ++wIdx) {
			matrixIdx<2> mapPos{wIdx, hIdx / 3};
			bool pointsUp = (mapPos.getSize() + mapPos.getSizei(1) + 1) % 2;
			char symbol = this->map[mapPos].getBla();

			switch ((hIdx % 3)) {
			case 0:
				APPEND << "+++";
				break;
			case 1:
				if (pointsUp) {
					APPEND << "+" << symbol << "+";
				} else {
					APPEND << symbol << symbol << symbol;
				}
				break;
			case 2:
				if (!pointsUp) {
					APPEND << "+" << symbol << "+";
				} else {
					APPEND << symbol << symbol << symbol;
				}
				break;
			}
		}
		APPEND << "+" << END;
	}
}

MapTile& WorldMap::at(matrixIdx<2> &tileIdx) {
	return this->map[tileIdx];
}

void WorldMap::clearFlags(void) {
	this->map.apply([](MapTile &tile) {tile.clear();});
}

std::vector<matrixIdx<2>> WorldMap::getNeighbours(matrixIdx<2> &tileIndex) {
	std::vector<matrixIdx<2>> indexList;

	if (tileIndex.getSize() > 0) {
		indexList.emplace_back(tileIndex.getSize() - 1, tileIndex.getSizei(1));
	}
	if (tileIndex.getSize() < this->width - 1) {
		indexList.emplace_back(tileIndex.getSize() + 1, tileIndex.getSizei(1));
	}

	bool pointsUp = (tileIndex.getSize() + tileIndex.getSizei(1) + 1) % 2;
	if (pointsUp) {
		if (tileIndex.getSizei(1) < this->height - 1) {
			indexList.emplace_back(tileIndex.getSize(), tileIndex.getSizei(1) + 1);
		}
	} else {
		if (tileIndex.getSizei(1) > 0) {
			indexList.emplace_back(tileIndex.getSize(), tileIndex.getSizei(1) - 1);
		}
	}

	return indexList;
}

std::vector<std::pair<matrixIdx<2>, unsigned>> WorldMap::plotPath(const matrixIdx<2> &startIdx, const matrixIdx<2> &endIdx) {
	matrix<matrixIdx<2>, 2> prevSide{{-1u, -1u}, this->map.getSize()};
	matrix<unsigned, 2> costs{-1u, this->map.getSize()};
	std::queue<matrixIdx<2>> working;

	working.push(startIdx);
	costs[startIdx] = 0;

	while (working.size()) {
		auto currIdx = working.front();
		working.pop();

		unsigned baseCost = costs[currIdx];

		std::vector<matrixIdx<2>> neighbours = this->getNeighbours(currIdx);

		for (auto tileIndex : neighbours) {
			std::cout << tileIndex << ", ";

			unsigned newCost = baseCost + this->map[tileIndex].getCost();

			if (newCost < costs[tileIndex])
			{
				costs[tileIndex] = newCost;
				prevSide[tileIndex] = currIdx;
				working.push(tileIndex);
			}
		}
		std::cout << std::endl;
	}

	std::vector<std::pair<matrixIdx<2>, unsigned>> path;

	if (costs[endIdx] < 1000000) {
		matrixIdx<2> nextEdge = endIdx;
		while (nextEdge != startIdx) {
			path.emplace_back(nextEdge, costs[nextEdge]);
			nextEdge = prevSide[nextEdge];
		}
		path.emplace_back(nextEdge, 0);
	}

	std::reverse(path.begin(), path.end());

	return path;
}

void WorldMap::tagTilesWithinDistance(matrixIdx<2> startPosition, size_t distance) {
	matrix<size_t, 2> costs{-1u, this->map.getSize()};
	std::queue<matrixIdx<2>> working;

	this->clearFlags();
	working.push(startPosition);
	costs[startPosition] = 0;

	while (working.size()) {
		auto currIdx = working.front();
		working.pop();

		size_t baseCost = costs[currIdx];

		std::vector<matrixIdx<2>> neighbours = this->getNeighbours(currIdx);

		for (auto tileIndex : neighbours) {
			std::cout << tileIndex << ", ";

			unsigned newCost = baseCost + this->map[tileIndex].getCost();

			if ((newCost < costs[tileIndex]) && (newCost <= distance))
			{
				costs[tileIndex] = newCost;
				this->map[tileIndex].flag();
				working.push(tileIndex);
			}
		}
		std::cout << std::endl;
	}


}

