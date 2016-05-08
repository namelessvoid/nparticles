/**
 * Basic vertex shader inputs provided by NParticles.
 *
 * This header contains the basic vertex shader inputs provided by the NParticles
 * engine. These are.
 */

#version 430

#extension GL_ARB_shading_language_include : enable

#include </np/vertex-inputs.glsl>
#include </np/uniforms.glsl>

#include </gravity/inputs.glsl>

flat out vec4 v_color;

// Color subroutines
subroutine vec4 ColorFunc();

// Use the velocity vector as color.
subroutine (ColorFunc) vec4 velocityVector()
{
    // Distinguish velocity for euler and verlet.
    vec3 velocity = properties[gl_InstanceID].xyz;
    if(usingVerlet)
        velocity = (positions[gl_InstanceID].position - velocity) * 100;

    return vec4(velocity, 1.0);
}

// Draw fast particles red and slow ones blue.
subroutine (ColorFunc) vec4 velocitySpeed()
{
    // Distinguish velocity for euler and verlet.
    vec3 velocity = properties[gl_InstanceID].xyz;
    if(usingVerlet)
        velocity = (positions[gl_InstanceID].position - velocity) * 500;

    return mix(vec4(0, 0, 1, 1), vec4(1, 0, 0, 1), length(velocity) / 500);
}

// Particles near the origin (0/0/0) are painted green wheres particles
// far away are painted red.
subroutine (ColorFunc) vec4 distance()
{
    return mix(vec4(0, 1, 0, 1), vec4(1, 0, 0, 1), length(positions[gl_InstanceID].position) / 100);
}

// Paints particles yellow.
subroutine (ColorFunc) vec4 yellowSubroutine()
{
    return vec4(1, 1, 0, 1);
}

subroutine uniform ColorFunc colorFunction;

/**
 * Vertex shader of the gravity simulation.
 *
 * It performs basic vertex transformation and defines the color of the
 * rendered sprite.
 *
 * It also sets the point size to decrease the farther away a particle is
 * from the camera.
 */
void main()
{
    // Color
    v_color = colorFunction();

    // Vertex transformation
    vec3 pos = np_in_position + positions[gl_InstanceID].position;

    gl_Position = np_viewProjectionMatrix * vec4(pos, 1);

    // Sprite sizes
    gl_PointSize = positions[gl_InstanceID].mass / length(gl_Position);
}
