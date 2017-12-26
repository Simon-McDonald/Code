/*
 * Timeout.hpp
 *
 *  Created on: Dec 24, 2017
 *      Author: Simon
 */

#ifndef SRC_TIMEOUT_HPP_
#define SRC_TIMEOUT_HPP_

class Timeout {
public:
	Timeout(double timeout_ms);

	void reset(void);
	bool update(double delta_ms);
	bool hasExpired(void);
	void modDuration(double change_ms);

private:
	double timeoutDuration_ms;
	double currentTime_ms;
};

#endif /* SRC_TIMEOUT_HPP_ */
