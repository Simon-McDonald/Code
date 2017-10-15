/*
 * WorldPosition.c
 *
 *  Created on: Dec 29, 2015
 *      Author: Simon
 */

#include "WorldPosition.h"

WorldPosition::WorldPosition() : lat(0), lon(0), rad(0) {}

WorldPosition::WorldPosition(float latitude, float longitude, float radius) : lat(latitude), lon(longitude), rad(radius) {}

glm::vec4 WorldPosition::getCartesianCoords() {
	return glm::vec4(sin(this->getLatitude()) * sin(this->getLongitude()) * this->getRadius(),
					 cos(this->getLatitude()) * this->getRadius(),
					 sin(this->getLatitude()) * cos(this->getLongitude()) * this->getRadius(),
					 1);
}

void WorldPosition::setPosition(WorldPosition newPosition) {
	this->lat = newPosition.getLatitude();
	this->lon = newPosition.getLongitude();
	this->rad = newPosition.getRadius();
}

void WorldPosition::setPosition(glm::vec4 newPosition) {
	this->lon = atan2(newPosition[0], newPosition[2]);
	this->lat = acos(newPosition[1] / this->getRadius());
}

float WorldPosition::getLatitude() {
	return this->lat;
}

float WorldPosition::getLongitude() {
	return this->lon;
}

float WorldPosition::getRadius() {
	return this->rad;
}
