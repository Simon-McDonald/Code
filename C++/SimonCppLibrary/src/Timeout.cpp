/*
 * Timeout.cpp
 *
 *  Created on: Dec 24, 2017
 *      Author: Simon
 */

#include "Timeout.hpp"

Timeout::Timeout(double timeout_ms) : timeoutDuration_ms(timeout_ms), currentTime_ms(0.0) {}

void Timeout::reset(void) {
	currentTime_ms = 0.0;
}

bool Timeout::update(double delta_ms) {
	this->currentTime_ms += delta_ms;
	return this->hasExpired();
}

bool Timeout::hasExpired(void) {
	return (this->currentTime_ms >= timeoutDuration_ms);
}

void Timeout::modDuration(double change_ms) {
	this->timeoutDuration_ms += change_ms;
}
