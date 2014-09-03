//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#include <isl/blob/BlobMeasure.h>
#include <isl/blob/Blob.h>
#include "InternalErrorHandler.h"

namespace isl {


void
BlobPerimeter::compute()
{
  this->measure_ = ::fabs(cvArcLength(this->blob_.edges_, CV_WHOLE_SEQ, 1));
}

void
BlobArea::compute()
{
  this->measure_ = ::fabs(cvContourArea(this->blob_.edges_, CV_WHOLE_SEQ));
}

void
BlobEllipse::compute()
{
  if( this->blob_.edges_ != NULL && this->blob_.edges_->total > 6)
  {
    this->measure_.params_ = cvFitEllipse2( this->blob_.edges_ );
    this->measure_.params_.angle *= static_cast<float>(CV_PI) / 180;  //- convert angle value into radians
  }
}

void
BlobBoundingRect::compute()
{
  this->measure_.params_ = cvBoundingRect( this->blob_.edges_, 1 );
}

}
