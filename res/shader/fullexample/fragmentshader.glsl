/**
 * Basic vertex shader inputs provided by NParticles.
 *
 * This header contains the basic vertex shader inputs provided by the NParticles
 * engine. These are.
 */

#version 430

uniform vec3 color;

in vec4 v_color;

out vec4 fColor;

/**
 * Simple fragment shader applying the color calculated by the vertex shader.
 */
void main(void)
{
    fColor = mix(vec4(color, 1), v_color, 0.5);
}
