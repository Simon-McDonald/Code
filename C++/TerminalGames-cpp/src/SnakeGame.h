/*
 * SnakeGame.h
 *
 *  Created on: Mar 8, 2016
 *      Author: Simon
 */

#ifndef SNAKEGAME_H_
#define SNAKEGAME_H_

class SnakeGame {
public:
	SnakeGame (char x, char y);
	~SnakeGame ();

	static void RunSnakeGame ();

	static void start ();

	void setDisplay ();
	void updateDisplay ();

	void setTarget ();

	bool getUserInput ();
	bool moveSnake ();

	int nextIndex (int index);
	int prevIndex (int index);
	int offsetIndex (int index, int offset);

	static const int maxLength = 32;
	static const int startUpdateDuration_ms = 200;
	static const int deltaUpdateDuration_ms = 5;
	static const int startLength = 5;

private:
	unsigned int score;

	char targetX;
	char targetY;
	char dirX;
	char dirY;

	int headIndex;
	char snakeCoordX[maxLength];
	char snakeCoordY[maxLength];
};

#endif /* SNAKEGAME_H_ */
