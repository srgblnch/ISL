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
