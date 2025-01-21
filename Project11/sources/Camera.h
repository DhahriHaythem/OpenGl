#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

const float DEF_FOV = 45.0f;

class Camera
{
public:
	glm::mat4 getViewMatrix() const;
	virtual void setposition(glm::vec3& position) {};
	virtual void rotate(float yaw, float pitch) {};//in degrees
	virtual void move(const glm::vec3& offsetPos) {};

	const glm::vec3& getLook() const;
	const glm::vec3& getRight() const;
	const glm::vec3& getUp() const;

	float getFOV() const{ return mFOV; }
	void setFOV(float fov) { mFOV = fov; } //degrees

protected:
	Camera();
	virtual void updateCameraVectors() {};

	glm::vec3 mPosition;
	glm::vec3 mTarget;
	glm::vec3 mUp;
	glm::vec3 mLook;
	glm::vec3 mRight;
	const glm::vec3 WORLD_UP;
	//Eulers angles in radians
	float mYaw, mPitch;

	float mFOV; //degrees
};

class FPSCamera: public Camera
{
public:
	FPSCamera(glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f),float yaw = glm::pi<float>(),float pitch =0.0f);

	virtual void setposition(glm::vec3& position);
	virtual void rotate(float yaw, float pitch);//in degrees
	virtual void move(const glm::vec3& offsetPos);
private:
	virtual void updateCameraVectors();
};


class OrbitCamera: public Camera
{
public:
	OrbitCamera();
	virtual void rotate(float yaw, float pitch);//in degrees	
	void setLookAt(const glm::vec3& target);
	void setRadius(float radius);
private:
	virtual void updateCameraVectors();
	float mRadius;
};



