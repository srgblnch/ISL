//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#include <isl/statistics/Projections.h>
#include "InternalErrorHandler.h"
#include <isl/Image.h>

#if !defined (__ISL_INLINE__)
# include <isl/statistics/Projections.i>
#endif

namespace isl {

Projections::Projections(const Image& image)
: x_projection_(0),
  x_max_pos_(0),
  x_max_val_(0),
  y_projection_(0),
  y_max_pos_(0),
  y_max_val_(0)
{
  this->compute(image);
}

Projections::Projections(const Projections& other)
{
  *this = other;
}

Projections& Projections::operator=(const Projections& other)
{
  if (this != &other)
  {
    this->x_projection_ = cvCloneMat(other.x_projection_);
    this->x_max_pos_ = other.x_max_pos_;
    this->x_max_val_ = other.x_max_val_;
    this->y_projection_ = cvCloneMat(other.y_projection_);
    this->y_max_pos_ = other.y_max_pos_;
    this->y_max_val_ = other.y_max_val_;
  }
  return *this;
}

Projections::~Projections()
{
  this->clean();
}

void
Projections::clean()
{
  if (this->x_projection_)
    cvReleaseMat(&this->x_projection_);
  if (this->y_projection_)
    cvReleaseMat(&this->y_projection_);

  this->x_max_pos_ = 0;
  this->x_max_val_ = 0;
  this->y_max_pos_ = 0;
  this->y_max_val_ = 0;
}




void
Projections::compute(const Image& image)
{
  CvMat*  x_projection = 0;
  CvPoint x_max_pos;
  double  x_max_val = 0;
  CvMat*  y_projection = 0;
  CvPoint y_max_pos;
  double  y_max_val = 0;

  IplImage* src = image.get_ipl_image();

  CV_FUNCNAME( "Projections::compute" );
  __BEGIN__;

  CV_CALL( x_projection = cvCreateMat(1, src->width, CV_64F) );
  CV_CALL( y_projection = cvCreateMat(src->height, 1, CV_64F) );

  CV_CALL( cvReduce(src, x_projection, 0, CV_REDUCE_AVG) );
  CV_CALL( cvReduce(src, y_projection, 1, CV_REDUCE_AVG) );

  CV_CALL( cvMinMaxLoc(x_projection, 0, &x_max_val, 0, &x_max_pos) );
  CV_CALL( cvMinMaxLoc(y_projection, 0, &y_max_val, 0, &y_max_pos) );

  __END__;

  this->x_projection_ = x_projection;
  this->x_max_pos_ = x_max_pos.x;
  this->x_max_val_ = x_max_val;
  this->y_projection_ = y_projection;
  this->y_max_pos_ = y_max_pos.y;
  this->y_max_val_ = y_max_val;

  __ISL_CHECK_ERROR__;
}


}
