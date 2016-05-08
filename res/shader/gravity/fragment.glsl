/**
 * Basic vertex shader inputs provided by NParticles.
 *
 * This header contains the basic vertex shader inputs provided by the NParticles
 * engine. These are.
 */

#version 430

#extension GL_ARB_shading_language_include : enable

#include </np/point-sprite-utils.glsl>

flat in vec4 v_color;

out vec4 f_color;

subroutine void SpriteType();

/**
 * Draw the particle as a circle.
 */
subroutine (SpriteType) void circle()
{
    npCreateCircularPointSprite();
}

/**
 * Draw the particle as a square.
 */
subroutine (SpriteType) void quad()
{
}

subroutine uniform SpriteType createSpriteShape;

/**
 * Fragment shader applying the color calculated by the vertex shader.
 * It also decides if the particle is rendered as square or circle (subroutines!).
 */
void main()
{
    createSpriteShape();
    f_color = v_color;
}
