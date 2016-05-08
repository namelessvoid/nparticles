/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#include "action.hpp"

namespace nparticles
{

Action::Action(ComputeProgram& computeProgram)
    : mComputeProgram(computeProgram)
{
}

Action::~Action()
{
}

} // namespace nparticles
