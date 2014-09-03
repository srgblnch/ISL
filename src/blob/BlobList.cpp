//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#include <isl/blob/BlobList.h>
#include "InternalErrorHandler.h"
#include <isl/Image.h>

#include <algorithm>

namespace isl {

BlobList::BlobList()
{
}

BlobList::BlobList(Image& _image)
{
  CV_FUNCNAME( "BlobList::BlobList" );
  __BEGIN__;

  ISL_CALL( this->detect(_image) );

  __END__;
  __ISL_CHECK_ERROR__;
}

BlobList::~BlobList()
{
}

void
BlobList::draw(Image& _im, double _intensity, int _thickness, int _line_type) const
{
  CV_FUNCNAME( "BlobList::draw" );
  __BEGIN__;

  BlobList::const_iterator it;
  for (it = this->begin(); it != this->end(); it++)
  {
    ISL_CALL( (*it).draw(_im, _intensity, _thickness, _line_type) );
  }

  __END__;
  __ISL_CHECK_ERROR__;
}

void
BlobList::detect(Image& _image)
{
  //- storage for initial contour list
  CvMemStorage* stor = 0;
  CvSeq* cont = 0;

  CV_FUNCNAME( "BlobList::analyze" );
  __BEGIN__;


  //- remove any existing blob
  this->clear();

  //- memory allocation for contours
  CV_CALL( stor = cvCreateMemStorage(0) );
  CV_CALL( cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT | CV_SEQ_KIND_CURVE, sizeof(CvContour), sizeof(CvPoint) , stor) );

  //- contour finding algorithm from OpenCV
  CV_CALL( cvFindContours(_image.get_ipl_image(),
                          stor,
                          &cont,
                          sizeof(CvContour),
                          CV_RETR_LIST,
                          CV_CHAIN_APPROX_NONE,
                          cvPoint(0,0)) );

  //- extract each contour and push it into the deque
  while(cont != NULL)
  {
    Blob cur_blob(cont);
    this->push_back( cur_blob );
    cont = cont->h_next;
  }

  __END__;

  cvReleaseMemStorage(&stor);

  __ISL_CHECK_ERROR__;
}

void
BlobList::filter(Filter* _filter, BlobList& dst)
{
  CV_FUNCNAME( "BlobList::filter" );
  __BEGIN__;
    
  BlobList::iterator it;
  BlobList tmp_list;

  for (it = this->begin(); it != this->end(); ++it)
  {
    bool accepted;
    ISL_CALL( accepted = _filter->accept(*it) ); 
    if (accepted)
      tmp_list.push_back(*it);
  }
  dst.swap(tmp_list);

  __END__;
  __ISL_CHECK_ERROR__;
}

void
BlobList::filter(Filter* _filter)
{
  CV_FUNCNAME( "BlobList::filter" );
  __BEGIN__;
    
  ISL_CALL( this->filter(_filter, *this) );
  
  __END__;
  __ISL_CHECK_ERROR__;
}

void
BlobList::filter(Criteria* _crit, Operator* _op, BlobList& _dst)
{
  CV_FUNCNAME( "BlobList::filter" );
  __BEGIN__;
    
  ISL_CALL( this->filter(NumericalFilter(_crit, _op), _dst) ); 
  
  __END__;
  __ISL_CHECK_ERROR__;
}

void
BlobList::filter(Criteria* _crit, Operator* _op)
{
  CV_FUNCNAME( "BlobList::filter" );
  __BEGIN__;
    
  ISL_CALL( this->filter(_crit, _op, *this) );
  
  __END__;
  __ISL_CHECK_ERROR__;
}



}
