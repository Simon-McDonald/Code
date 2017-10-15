
#ifndef WORLDPOSITION_H_
#define WORLDPOSITION_H_

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class WorldPosition {

public:
	WorldPosition();
	WorldPosition(float, float, float);

	glm::vec4 getCartesianCoords();

	void setPosition(WorldPosition newPosition);
	void setPosition(glm::vec4 newPosition);

	float getLatitude();
	float getLongitude();
	float getRadius();

private:
	float lat;
	float lon;
	float rad;

};

#endif /* WORLDPOSITION_H_ */
