#include "Camera.h"

Camera::Camera()
	:mPosition({ 0.0f,0.0f,0.0f }),
	mTarget({ 0.0f,0.0f,0.0f }),
	mUp({ 0.0f,1.0f,0.0f }),
	mYaw(0.0f),
	mPitch(0.0f)
{
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(mPosition, mTarget, mUp);
}

OrbitCamera::OrbitCamera() :mRadius(10.0f)
{
}

void OrbitCamera::rotate(float yaw, float pitch)
{
	mYaw = glm::radians(yaw);
	mPitch = glm::radians(pitch);

	mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
	updateCameraVectors();
}

void OrbitCamera::setLookAt(const glm::vec3& target)
{
	mTarget = target;
}

void OrbitCamera::setRadius(float radius)
{
	mRadius = glm::clamp(radius, 2.0f, 80.0f);
}

void OrbitCamera::updateCameraVectors()
{
	mPosition.x = mTarget.x + mRadius * cosf(mPitch) * sinf(mYaw);
	mPosition.y = mTarget.y + mRadius * sinf(mPitch);
	mPosition.z = mTarget.z + mRadius * cosf(mPitch) * cosf(mYaw);
}
