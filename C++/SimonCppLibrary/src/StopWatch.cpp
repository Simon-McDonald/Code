/*
 * StopWatch.h
 *
 *  Created on: Oct 18, 2015
 *      Author: Simon
 */

#include "StopWatch.h"

StopWatch::StopWatch() : running(true), counts(0), pausedDuration(0.0f) {
	this->startTime = std::chrono::high_resolution_clock::now();
	this->intermediateTime = this->startTime;
	this->endTime = this->startTime;
}

void StopWatch::Start() {
	this->running = true;
	this->counts = 0;
	this->pausedDuration = 0.0f;

	this->startTime = std::chrono::high_resolution_clock::now();
	this->intermediateTime = this->startTime;
	this->endTime = this->startTime;
    this->running = true;
}

float StopWatch::Mark() {
	this->intermediateTime = this->endTime;
	this->endTime = std::chrono::high_resolution_clock::now();
	this->counts++;
	return (std::chrono::duration_cast<std::chrono::duration<float>>(this->endTime - this->intermediateTime).count());
}

void StopWatch::Stop() {
	this->running = false;
	this->intermediateTime = this->endTime;
	this->endTime = std::chrono::high_resolution_clock::now();
}

void StopWatch::Resume() {
	this->running = true;
	this->intermediateTime = std::chrono::high_resolution_clock::now();
	this->pausedDuration += std::chrono::duration_cast<std::chrono::duration<float>>(this->intermediateTime - this->endTime).count();
}

float StopWatch::Duration() {
	return std::chrono::duration_cast<std::chrono::duration<float>>(this->endTime - this->startTime).count() - this->pausedDuration;
}

int StopWatch::Counts() {
	return this->counts;
}

float StopWatch::Increment() {
	return std::chrono::duration_cast<std::chrono::duration<float>>(this->endTime - this->intermediateTime).count();
}
