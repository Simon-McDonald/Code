/*
 * StopWatch.h
 *
 *  Created on: Oct 18, 2015
 *      Author: Simon
 */

#ifndef STOPWATCH_H_
#define STOPWATCH_H_

#include <chrono>

class StopWatch {

public:
	StopWatch();

	void Start();
	float Mark();
	void Stop();
	void Resume();

	float Duration();
	int Counts();
	float Increment();

private:
    bool running;
    int counts;
    float pausedDuration;

    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point intermediateTime;
    std::chrono::high_resolution_clock::time_point endTime;

};

#endif /* STOPWATCH_H_ */
