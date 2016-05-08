/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_POINT_SPRITE_UITLS
#define NP_POINT_SPRITE_UTILS

/**
 * Used to discard fragments to get a circular point sprite.
 *
 * This method can be used to create a circular point sprite.
 * It discards all fragments that are outside of the radius of the sprite's
 * circle.
 *
 * @note Fragment shader only!
 */
void npCreateCircularPointSprite()
{
    vec2 temp = gl_PointCoord - vec2(0.5);
    float distanceToCenter = dot(temp, temp);

    if(distanceToCenter > 0.25)
            discard;
}

#endif // NP_POINT_SPRITE_UTILS
