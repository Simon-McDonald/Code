
#include <iostream>

#include <Logger.hpp>

#include "UtilityManager.hpp"
#include "WorldMap.hpp"

#include "matrix.hpp"

Logger& getLogger() {
	static Logger logger;
	return logger;
}



int main() {
	Config config("config/config.txt");
	Logger logger;
	UtilityManager::Initialise(&config, &logger);


	WorldMap map {WorldMap::MapSize::Small, WorldMap::MapShape::Square};
	map.PrintTextDisplay();

	matrixIdx<2> start{1, 1};
	matrixIdx<2> end{3, 3};

	auto path = map.plotPath(start, matrixIdx<2>{5, 5});

	for (auto pos : path) {
		//pos.first.print(std::cout);
		std::cout << pos.first << std::endl;
	}



	return 0;
}
