//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#include "Engine.h"

#if !defined (__ISL_INLINE__)
# include "Engine.i"
#endif


namespace isl {

Engine::Engine()
: x_mag_factor_(0),
  y_mag_factor_(0),
  model_error_(0)
{
}

Engine::~Engine()
{
}


}
