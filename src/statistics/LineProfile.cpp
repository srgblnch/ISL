//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#include <isl/statistics/LineProfile.h>
#include "InternalErrorHandler.h"
#include <isl/Image.h>
#include <iomanip>


#if !defined (__ISL_INLINE__)
# include <isl/statistics/LineProfile.i>
#endif

namespace isl {

CvPoint2D64f operator +(const CvPoint2D64f& p, const CvPoint2D64f& q)
{
  return cvPoint2D64f(p.x + q.x, p.y + q.y);
}

CvPoint2D64f operator -(const CvPoint2D64f& p, const CvPoint2D64f& q)
{
  return cvPoint2D64f(p.x - q.x, p.y - q.y);
}

CvPoint2D64f operator *(const CvPoint2D64f& p, double s)
{
  return cvPoint2D64f(p.x * s, p.y * s);
}

CvPoint2D64f operator /(const CvPoint2D64f& p, double s)
{
  return cvPoint2D64f(p.x / s, p.y / s);
}

LineProfile::LineProfile()
  : profile_(0)
{
}

LineProfile::~LineProfile()
{
  this->clean();
}

void
LineProfile::clean()
{
  if (this->profile_)
    cvReleaseMat(&this->profile_);
}

Image*
LineProfile::compute(const Image& image, Point2D<int> origin, Point2D<int> end, int thickness)
{
  this->clean();
  CvMat*  profile = 0;
  IplImage* src = image.get_ipl_image();
  
  CvPoint pi = cvPoint(origin.x(), origin.y());
  CvPoint qi = cvPoint(end.x(), end.y());
  bool vertical   = (pi.x == qi.x);
  bool horizontal = (pi.y == qi.y);
  CvPoint2D64f p,q,u,v, p1, p2, p3, p4;
  Image* dst = 0;
  CvMat* map_matrix = 0;
  double t,d;
  int di;

  CV_FUNCNAME( "LineProfile::compute" );
  __BEGIN__;

  if (vertical && horizontal)
    CV_ERROR(CV_StsBadArg, "Origin and End of profile are the same");

  //- use the center of pixels
  p = cvPoint2D64f(pi.x, pi.y);
  q = cvPoint2D64f(qi.x, qi.y);
  t = thickness;

  if ( q.x == p.x )
  {
    //- vertical
    if ( q.y > p.y )
    {
      u.x = 0;
      u.y = 1;
      v.x = -1;
      v.y = 0;
    }
    else
    {
      u.x = 0;
      u.y = -1;
      v.x = 1;
      v.y = 0;
    }
  }
  else
  {
    double c =  ( q.y - p.y ) / ( q.x - p.x );
    double s = ::sqrt( 1 / (1 + c * c) );

    if ( q.x > p.x )
    {
      u.x = s;
      u.y = s * c;
      v.x = - c * s;
      v.y = s;
    }
    else
    {
      u.x = - s;
      u.y = - s * c;
      v.x = c * s;
      v.y = - s;
    }
  }
  d  = ::sqrt( (p.x - q.x) *  (p.x - q.x) + (p.y - q.y) *  (p.y - q.y) );
  di = cvCeil( d );

  p1 = p - u * 0.5 - v * (0.5 * t);
  p2 = p + u * di + u * 0.5 - v * (0.5 * t);
  p3 = p + u * di + u * 0.5 + v * (0.5 * t);
  p4 = p - u * 0.5 + v * (0.5 * t);

  CvPoint2D32f src_pt [3];
  src_pt[0] = cvPoint2D32f( p1.x, p1.y );
  src_pt[1] = cvPoint2D32f( p2.x, p2.y );
  src_pt[2] = cvPoint2D32f( p3.x, p3.y );

  CvPoint2D32f dst_pt [3];
  dst_pt[0] = cvPoint2D32f( -0.5, -0.5 );
  dst_pt[1] = cvPoint2D32f( di + 1 - 0.5, -0.5 );
  dst_pt[2] = cvPoint2D32f( di + 1 - 0.5, t - 0.5 );

#ifdef _IBA_USE_FLOAT64_ 
    CV_CALL( map_matrix = cvCreateMat(2, 3, CV_64F) ); 
#else 
    CV_CALL( map_matrix = cvCreateMat(2, 3, CV_32F) ); 
#endif 

  //- compute affine transform from DST to SRC (optimization in cvWarpAffine)
  CV_CALL( cvGetAffineTransform(dst_pt, src_pt, map_matrix) );

#ifdef _IBA_USE_FLOAT64_ 
  ISL_CALL( dst = new Image(di + 1, thickness, isl::ISL_STORAGE_DOUBLE) ); 
#else 
  ISL_CALL( dst = new Image(di + 1, thickness, isl::ISL_STORAGE_FLOAT) );
#endif
  
  CV_CALL( cvWarpAffine(src,
                        dst->get_ipl_image(),
                        map_matrix,
                        CV_WARP_FILL_OUTLIERS + CV_WARP_INVERSE_MAP + CV_INTER_AREA,
                        cvScalarAll(0)) );
  
  CV_CALL( profile = cvCreateMat(1, di + 1, CV_64F) );

  CV_CALL( cvReduce( dst->get_ipl_image(), profile, 0, CV_REDUCE_AVG ) );

  __END__;

  cvReleaseMat( &profile_ );
  profile_ = profile;
  cvReleaseMat( &map_matrix );

  __ISL_CHECK_ERROR__;

  return dst;
}

}
