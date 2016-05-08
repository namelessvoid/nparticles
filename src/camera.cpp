/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#include "camera.hpp"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <iostream>

namespace nparticles
{

Camera::Camera()
    : mUpVector(0, 1, 0),
      mDirectionVector(0, 0, -1),
      mStrafeVector(1, 0, 0),
      mPosition(0, 0, 0),
      mProjectionMatrix(glm::mat4())
{
}

Camera::~Camera()
{
}

void Camera::setPerspective(float fieldOfView, float screenWidth, float screenHeight, float nearClippingPlane, float farClippingPlane)
{
    mProjectionMatrix = glm::perspective<float>(fieldOfView, screenWidth / screenHeight, nearClippingPlane, farClippingPlane);
}

void Camera::setMovementSpeed(float speed, Camera::directions direction)
{
    if(direction == NP_DIR_BACKWARD || direction == NP_DIR_STRAFE_LEFT || direction == NP_DIR_DOWN)
        speed *= -1;

    switch(direction)
    {
    case NP_DIR_STRAFE_LEFT: // fallthrough
    case NP_DIR_STRAFE_RIGHT:
        mSpeed.x = speed;
        break;
    case NP_DIR_DOWN: // fallthrough
    case NP_DIR_UP:
        mSpeed.y = speed;
        break;
    case NP_DIR_BACKWARD: // fallthrough
    case NP_DIR_FOREWARD:
        mSpeed.z = speed;
        break;
    }
}

void Camera::rotate(float angle, Camera::rotations rotation)
{
    glm::vec3 rotationAxis;

    switch(rotation)
    {
    case NP_ROT_YAW:
        rotationAxis = mUpVector;
        break;
    case NP_ROT_PITCH:
        rotationAxis = mStrafeVector;
        break;
    case NP_ROT_ROLL:
        rotationAxis = mDirectionVector;
        break;
    }

    mUpVector = glm::rotate(mUpVector, angle, rotationAxis);
    mStrafeVector = glm::rotate(mStrafeVector, angle, rotationAxis);
    mDirectionVector = glm::rotate(mDirectionVector, angle, rotationAxis);
}

void Camera::move(glm::vec3 distance)
{
    mPosition += distance.x * mStrafeVector;
    mPosition += distance.y * mUpVector;
    mPosition += distance.z * mDirectionVector;
}

void Camera::updatePosition()
{
    mPosition += mSpeed.x * mStrafeVector;
    mPosition += mSpeed.y * mUpVector;
    mPosition += mSpeed.z * mDirectionVector;
}

glm::mat4 Camera::getViewProjectionMatrix()
{
    return mProjectionMatrix * glm::lookAt(mPosition, mPosition + mDirectionVector, mUpVector);
}

glm::mat3 Camera::getNormalMatrix()
{
    return glm::inverseTranspose(glm::mat3(getViewProjectionMatrix()));
}

} // namespace nparticles
