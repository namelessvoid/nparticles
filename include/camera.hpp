/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_CAMERA_HPP
#define NP_CAMERA_HPP

#include <glm/glm.hpp>

namespace nparticles
{

/**
 * The Camera class is a basic camera implementation.
 *
 * The vector based Camera is used to navigate through a scene.
 */
class Camera
{
public:
    /**
     * Enum rotations defines valid Camera rotations.
     *
     * This rotations are passed to rotate() to specify the axis of rotation.
     * The axes are relative to the strafe (x), up (y) and direction (z) vectors
     * of the Camera.
     */
    enum rotations
    {
        NP_ROT_PITCH, /**< rotation around strafe / x axis */
        NP_ROT_YAW,   /**< rotation around up / y axis */
        NP_ROT_ROLL   /**< rotation around direction / z axis */
    };

    /**
     * Enum directions defines valid Camera movements.
     *
     * This directions are passed to setMovementSpeed() to set the movement
     * in the given direction. The movement is relative to the strafe (x), up (y) and
     * direction (z) vectors of the Camera.
     */
    enum directions
    {
        NP_DIR_FOREWARD,      /**< Move foreward along the direction (z) axis */
        NP_DIR_BACKWARD,      /**< Move backards along the direction (z) axis */
        NP_DIR_STRAFE_RIGHT,  /**< Move to the right along the strafe (x) axis */
        NP_DIR_STRAFE_LEFT,   /**< Move to the left along the strafe (x) axis */
        NP_DIR_UP,            /**< Move up along the up (y) axis */
        NP_DIR_DOWN           /**< Move down along the up (y) axis */
    };

    /**
     * Camera constructor.
     */
    Camera();

    /**
     * Camera destructor.
     */
    ~Camera();

    /**
     * Set the perspective projection.
     *
     * Set the perspective projection matrix for projecting the scene.
     *
     * @param fieldOfView The field of view in radians.
     * @param screenWidth The width of the view plane.
     * @param screenHeight The height of the view plane.
     * @param nearClippingPlane Distance of the near clipping plane.
     * @param farClippingPlane Distance of the far clipping plane.
     */
    void setPerspective(float fieldOfView, float screenWidth, float screenHeight, float nearClippingPlane, float farClippingPlane);

    /**
     * Set movement speed in a given @p direction.
     *
     * This sets the movement speed in a given @p direction to guarantee uniform movement.
     *
     * @param speed The speed.
     * @param direction The direction which is one of available Camera::directions
     */
    void setMovementSpeed(float speed, directions direction);

    /**
     * Rotate the Camera around a given @p rotation axis.
     *
     * This rotates the camera aroud a given @p rotation axis.
     *
     * @param angle The angle (in radians) to rotate.
     * @param rotation One of the available Camera::rotations.
     */
    void rotate(float angle, rotations rotation);

    /**
     * Move the Camera in a direction.
     *
     * Move the Camera by given distances. The order of directions of the vector is NP_DIR_STRAFE_RIGHT, NP_DIR_UP, NP_DIR_FOREWARD.
     *
     * @param distance Vector which indicates the distances to move.
     */
    void move(glm::vec3 distance);

    /**
     * Updates the Camera position.
     *
     * This udpates the position of the Camera according the current movement speed set via setMovementSpeed().
     */
    void updatePosition();

    /**
     * Get the view projection matrix.
     *
     * Get view and projection matrix combined to a single matrix. This can be used for view projection transformations
     * in a shader.
     *
     * @return The combined view and projection matrix.
     */
    glm::mat4 getViewProjectionMatrix();

    /**
     * Get the normal matrix.
     *
     * This returns the normal matrix retrieved by the current transformation of the camera. This can be used for
     * normal transformations.
     *
     * @return The normal matrix.
     */
    glm::mat3 getNormalMatrix();

private:
    /**
     * The up vector.
     */
    glm::vec3 mUpVector;

    /**
     * The directon or "look at" vector.
     */
    glm::vec3 mDirectionVector;

    /**
     * The strafe vector pointing to the right.
     */
    glm::vec3 mStrafeVector;

    /**
     * Current speed in x, y and z direction.
     */
    glm::vec3 mSpeed;

    /**
     * The current camera postion.
     */
    glm::vec3 mPosition;

    /**
     * The perspective projection matrix calculated by intputs of setPerspective().
     */
    glm::mat4 mProjectionMatrix;
};

} // namespace nparticles

#endif // NP_CAMERA_HPP
