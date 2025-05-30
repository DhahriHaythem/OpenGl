#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

class Camera
{
public:
	glm::mat4 getViewMatrix() const;
	virtual void rotate(float yaw, float pitch) {};//in degrees
protected:
	Camera();
	glm::vec3 mPosition;
	glm::vec3 mTarget;
	glm::vec3 mUp;
	//Eulers angles in radians
	float mYaw, mPitch;
};


class OrbitCamera: public Camera
{
public:
	OrbitCamera();
	virtual void rotate(float yaw, float pitch);//in degrees	
	void setLookAt(const glm::vec3& target);
	void setRadius(float radius);
private:
	void updateCameraVectors();
	float mRadius;
};



