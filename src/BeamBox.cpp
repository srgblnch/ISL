//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#include <isl/BeamBox.h>
#include "InternalErrorHandler.h"
#include <isl/Image.h>
#include <isl/blob/BlobList.h>
#include <algorithm>

#if !defined (__ISL_INLINE__)
# include <isl/BeamBox.i>
#endif

namespace isl {

BeamBox::BeamBox()
  : main_blob_(0)
{
}

BeamBox::BeamBox(const Image& image, double magnification_factor_x, double magnification_factor_y)
  : main_blob_(0)
{
  this->compute(image, magnification_factor_x, magnification_factor_y);
}

BeamBox::~BeamBox()
{
  SAFE_DELETE(this->main_blob_);
}

bool
BeamBox::compute(const Image& image, double magnification_factor_x, double magnification_factor_y)
{
  bool found = false;
  BlobList bl;
  Rectangle bounds;

  CV_FUNCNAME( "BeamBox::compute" );
  __BEGIN__;
  
  //- make a copy of the input image since BlobList::detect modify the source image
  Image image_for_blobs(image);
  ISL_CALL( bl.detect(image_for_blobs) );

  if (bl.size() == 0)
    CV_ERROR(CV_StsNoConv, "No blob found");

  //- keep only blobs with more than 10 points describing the contour
  ISL_CALL( bl.filter(ContourNbPoint(), Greater(10)) );

  if (bl.size() == 0)
    CV_ERROR(CV_StsNoConv, "No blob found");
  
  //- retrieve the blob with greater area
  if (bl.size() > 1)
  {
    std::partial_sort(bl.begin(),
                      bl.end(),
                      bl.end(),
                      BlobComparator(Area(), Greater()));
  }

  SAFE_DELETE(this->main_blob_);
  this->main_blob_ = new Blob(bl[0]);


  //- bounding box computation
  bounds = this->main_blob_->bounding_rect();
  this->params_.x       = cvRound( bounds.center().x() - magnification_factor_x * bounds.width()  / 2 );
  this->params_.y       = cvRound( bounds.center().y() - magnification_factor_y * bounds.height() / 2 );
  this->params_.width   = cvRound( bounds.width()  * magnification_factor_x );
  this->params_.height  = cvRound( bounds.height() * magnification_factor_y );

  __END__;
  __ISL_CHECK_ERROR__;
  return found;
}

void
BeamBox::draw(Image& img, double intensity, int thickness)
{
  CV_FUNCNAME( "BeamBox::draw_box" );
  __BEGIN__;

  CvRect& roi = this->params_;

  CV_CALL(cvLine(img.get_ipl_image() ,
                 cvPoint(roi.x, roi.y),
                 cvPoint(roi.x + roi.width, roi.y),
                 cvRealScalar(intensity),
                 thickness));
  CV_CALL(cvLine(img.get_ipl_image() ,
                 cvPoint(roi.x + roi.width, roi.y),
                 cvPoint(roi.x + roi.width, roi.y + roi.height),
                 cvRealScalar(intensity),
                 thickness));
  CV_CALL(cvLine(img.get_ipl_image() ,
                 cvPoint(roi.x + roi.width, roi.y + roi.height),
                 cvPoint(roi.x, roi.y + roi.height),
                 cvRealScalar(intensity),
                 thickness));
  CV_CALL(cvLine(img.get_ipl_image() ,
                 cvPoint(roi.x, roi.y + roi.height),
                 cvPoint(roi.x, roi.y),
                 cvRealScalar(intensity),
                 thickness));
  
  __END__;
  __ISL_CHECK_ERROR__;
}


}
