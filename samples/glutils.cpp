/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

/**
 * This is a very basic application to test
 * if glutils::getGlTypeInfo() was implemented correctly.
 *
 * TODO: Use a real test framework!
 */
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "glutils.hpp"

using namespace nparticles::glutils;

class MyClass
{};

int main(int argc, char* argv [])
{
    // bools
    bool       b1 = false;
    glm::bvec2 b2 = {false, false};
    glm::bvec3 b3 = {false, false, false};
    glm::bvec4 b4 = {false, false, false, false};

    assert(GlTypeInfo(GL_BOOL, 1) == getGlTypeInfo(b1));
    assert(GlTypeInfo(GL_BOOL, 2) == getGlTypeInfo(b2));
    assert(GlTypeInfo(GL_BOOL, 3) == getGlTypeInfo(b3));
    assert(GlTypeInfo(GL_BOOL, 4) == getGlTypeInfo(b4));

    // ints
    int        i1 = 1;
    glm::ivec2 i2 = {1, 2};
    glm::ivec3 i3 = {1, 2, 3};
    glm::ivec4 i4 = {1, 2, 3, 4};

    assert(GlTypeInfo(GL_INT, 1) == getGlTypeInfo(i1));
    assert(GlTypeInfo(GL_INT, 2) == getGlTypeInfo(i2));
    assert(GlTypeInfo(GL_INT, 3) == getGlTypeInfo(i3));
    assert(GlTypeInfo(GL_INT, 4) == getGlTypeInfo(i4));

    // unsigned ints
    unsigned int ui1 = 0;
    GLuint       ui2 = 2;

    assert(GlTypeInfo(GL_UNSIGNED_INT, 1) == getGlTypeInfo(ui1));
    assert(GlTypeInfo(GL_UNSIGNED_INT, 1) == getGlTypeInfo(ui2));

    // floats
    float     f1 = false;
    glm::vec2 f2 = {false, false};
    glm::vec3 f3 = {false, false, false};
    glm::vec4 f4 = {false, false, false, false};

    assert(GlTypeInfo(GL_FLOAT, 1) == getGlTypeInfo(f1));
    assert(GlTypeInfo(GL_FLOAT, 2) == getGlTypeInfo(f2));
    assert(GlTypeInfo(GL_FLOAT, 3) == getGlTypeInfo(f3));
    assert(GlTypeInfo(GL_FLOAT, 4) == getGlTypeInfo(f4));

    MyClass mc;
    assert(GlTypeInfo(GL_INVALID_VALUE, -1) == getGlTypeInfo(mc));
    std::cout << "All testst passed.\n";

    return 0;
}
