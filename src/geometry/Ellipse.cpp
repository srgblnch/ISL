//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#include <isl/geometry/Ellipse.h>
#include "InternalErrorHandler.h"
#include <isl/Image.h>

#if !defined (__ISL_INLINE__)
# include <isl/geometry/Ellipse.i>
#endif

namespace isl {

Ellipse::Ellipse()
{
  ::memset(&this->params_, 0, sizeof(Ellipse));
}


Ellipse::~Ellipse()
{
}

void
Ellipse::draw(Image& img, double intensity, int thickness, int line_type) const
{
  CV_FUNCNAME( "Ellipse::draw" );
  __BEGIN__;
 
  int shift = 0;
  double fixpoint_factor = static_cast<double>(1 << shift);
  CvPoint center = cvPoint( cvRound(this->params_.center.x * fixpoint_factor), cvRound(this->params_.center.y * fixpoint_factor) );
  CvSize  axes   = cvSize(cvRound(this->params_.size.width * fixpoint_factor * 0.5) , cvRound(this->params_.size.height * fixpoint_factor * 0.5) );
  double angle   = static_cast<double>( - this->params_.angle ) * 180 / CV_PI;
  
  CV_CALL( cvEllipse(img.get_ipl_image(),
                     center,
                     axes,
                     angle,
                     0,
                     360,
                     cvRealScalar(intensity),
                     thickness,
                     line_type) );
  
  __END__;
  __ISL_CHECK_ERROR__;
}


void
Ellipse::draw_center(Image& img, int nb_pixels, double intensity, int thickness, int line_type) const
{
  CV_FUNCNAME( "BeamBox::draw_box_center" );
  __BEGIN__;

  CvPoint center = cvPoint(cvRound(this->params_.center.x), cvRound(this->params_.center.y));

  CV_CALL(cvLine(img.get_ipl_image() ,
                 cvPoint(center.x - nb_pixels, center.y),
                 cvPoint(center.x - 1,         center.y),
                 cvRealScalar(intensity),
                 thickness,
                 line_type));

  CV_CALL(cvLine(img.get_ipl_image() ,
                 cvPoint(center.x + 1,         center.y),
                 cvPoint(center.x + nb_pixels, center.y),
                 cvRealScalar(intensity),
                 thickness,
                 line_type));

  CV_CALL(cvLine(img.get_ipl_image() ,
                 cvPoint(center.x, center.y - nb_pixels),
                 cvPoint(center.x, center.y - 1),
                 cvRealScalar(intensity),
                 thickness,
                 line_type));

  CV_CALL(cvLine(img.get_ipl_image() ,
                 cvPoint(center.x, center.y + 1),
                 cvPoint(center.x, center.y + nb_pixels),
                 cvRealScalar(intensity),
                 thickness,
                 line_type));

  __END__;
  __ISL_CHECK_ERROR__;
}

}
