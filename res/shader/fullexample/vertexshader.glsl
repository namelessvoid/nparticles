/**
 * Basic vertex shader inputs provided by NParticles.
 *
 * This header contains the basic vertex shader inputs provided by the NParticles
 * engine. These are.
 */

#version 430
#extension GL_ARB_shading_language_include : require

/**
 * Input variable storing the positin of a vertex.
 *
 * TODO: This is deprecated and should be accomplished using
 *       the engine built-in shader library!
 */
in vec3 np_in_position;

/**
 * Uniform variable storing the view projection matrix.
 *
 * TODO: This is deprecated and should be accomplished using
 *       the engine built-in shader library!
 */
uniform mat4 np_viewProjectionMatrix;

/**
 * The color of the vertex.
 */
in vec4 in_color;

/**
 * The rotation matrix used to rotate the vertex.
 */
layout (binding = 10) uniform RotationMatrixBlock
{
    mat4 rotationMatrix;
};

#include </fullexample/buffers.glsl>

out vec4 v_color;

/**
 * Vertex shader passing through the input color, performing translation of the
 * input vertex, applying a rotation and view projectin.
 */
void main()
{
    v_color = in_color;

    vec3 pos = np_in_position + position[gl_InstanceID].xyz;

    gl_Position = np_viewProjectionMatrix * rotationMatrix * vec4(pos, 1.0);
}
