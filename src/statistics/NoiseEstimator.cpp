//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#include <isl/statistics/NoiseEstimator.h>

#if !defined (__ISL_INLINE__)
# include <isl/statistics/NoiseEstimator.i>
#endif

#include "InternalErrorHandler.h"
#include <isl/Image.h>

namespace isl {

NoiseEstimator::NoiseEstimator()
: mean_(0),
  n_(0)
{
  
}

NoiseEstimator::~NoiseEstimator()
{
  this->init();
}

void
NoiseEstimator::init()
{
  SAFE_DELETE(this->mean_);
  this->n_ = 0;
}

const Image&
NoiseEstimator::add_image(const Image& im)
{
  CV_FUNCNAME( "NoiseEstimator::add_image" );
  __BEGIN__;

  if (this->mean_ == 0)
  {
    ISL_CALL( this->mean_ = new Image(im) );
    this->n_ = 1;
  }
  else
  {
    double n = static_cast<double>(this->n_);
    IplImage* mean = this->mean_->get_ipl_image();
    IplImage* new_im = im.get_ipl_image();

    CV_CALL( cvAddWeighted(mean,   n / (n + 1),
                           new_im, 1 / (n + 1),
                           0,
                           mean) );
    this->n_++;
  }

  __END__;
  __ISL_CHECK_ERROR__;
  return( *this->mean_ );
}


}
