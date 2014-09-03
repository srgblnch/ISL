//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#include <isl/statistics/AccurateCentroid.h>
#include "InternalErrorHandler.h"
#include <isl/Image.h>
#include <vector>
#include <string>
#include <sstream>

const double ISL_PI = 3.1415926535897932384626433832795f;

//- #define ACCURATE_CENTROID_DEBUG

#ifdef ACCURATE_CENTROID_DEBUG

  static int i = 0;
  static std::stringstream s;
  static std::string basename;

  void save_mat(const std::string& filename, CvMat* im)
  {
    CvMat* normalized = cvCloneMat(im);
    CvMat* uchar_im = cvCreateMat(im->height, im->width, CV_8U);
    cvNormalize(im, normalized, 255, 0, CV_MINMAX);
    cvConvert(normalized, uchar_im);
    cvSaveImage(filename.c_str(), uchar_im);
    cvReleaseMat(&normalized);
    cvReleaseMat(&uchar_im);
  }

#endif

namespace isl {


AccurateCentroid::AccurateCentroid()
{
  this->init_kernels();
}

AccurateCentroid::~AccurateCentroid()
{
}

void
AccurateCentroid::init_kernels()
{
  const int n = ISL_ACC_CENTR_KSIZE;
  const double sigma = ISL_ACC_CENTR_KSIGMA;
  float* gd  = this->gaussian_derivative_buf_;
  float* gdd = this->gaussian_laplacian_buf_;
  int i;

  double sigma2 = sigma * sigma;
  double scale_exp = - 0.5 / (sigma2);
  double scale_der = 1 / ( ::sqrt(2 * ISL_PI) * sigma2 * sigma );
  double scale_lap = 1 / ( ::sqrt(2 * ISL_PI) * sigma2 * sigma2 * sigma );

  for( i = 0; i <= n / 2; i++ )
  {
    double expo    = ::exp(scale_exp * i * i);
    gd[n / 2 + i]  = static_cast<float>( -i * expo * scale_der );
    gdd[n / 2 + i] = static_cast<float>( (i * i - sigma2) * expo * scale_lap );
  }
  for( i = 0; i <= n / 2; i++ )
  {
    gd[n / 2 - i]  = - gd[n / 2 + i];
    gdd[n / 2 - i] = gdd[n / 2 + i];
  }

  this->smooth_filt_buf_[0] = 0.25f;
  this->smooth_filt_buf_[1] = 0.5f;
  this->smooth_filt_buf_[2] = 0.25f;

  this->smooth_filt_        = cvMat(3, 1, CV_32F, this->smooth_filt_buf_);
  this->gaussian_derived_   = cvMat(1, n, CV_32F, gd);
  this->gaussian_laplacian_ = cvMat(1, n, CV_32F, gdd);
}


void
AccurateCentroid::process_rows(IplImage* _src, Line2D& _line)
{
  CvMat* src = 0;
  CvMat* derived = 0;
  CvMat* laplacian = 0;
  CvMat* temp = 0;
  CvMat* row_max = 0; // N * 1 vector containing the max of each row of abs(derived)

  std::vector<CvPoint2D32f> centers;
  std::vector<double> centers_weight;

	//- variables for line fitting
  double x, y, x2, y2, xy, w, t;
  double dx2, dy2, dxy;
  std::vector<CvPoint2D32f>::iterator c_it;
  std::vector<double>::iterator       w_it;

  CV_FUNCNAME( "AccurateCentroid::process_rows" );
  __BEGIN__;

  CvSize size = cvGetSize(_src);
  CV_CALL( src       = cvCreateMat(size.height, size.width, CV_32F) );
  CV_CALL( derived   = cvCreateMat(size.height, size.width, CV_32F) );
  CV_CALL( laplacian = cvCreateMat(size.height, size.width, CV_32F) );

  CV_CALL( cvConvert(_src,   src ) );
//  CV_CALL( cvSmooth(_src,   src,   CV_GAUSSIAN ) );
/*
#ifdef ACCURATE_CENTROID_DEBUG
  save_mat(basename + "_smooth.pgm", src);
#endif
*/

  CV_CALL( cvFilter2D(src,      derived,   &this->gaussian_derived_) );
/*
#ifdef ACCURATE_CENTROID_DEBUG
  save_mat(basename + "_deriv.pgm", derived);
#endif
*/
  CV_CALL( cvFilter2D(derived,   derived,   &this->smooth_filt_) );
#ifdef ACCURATE_CENTROID_DEBUG
  save_mat(basename + "_deriv_filt.pgm", derived);
#endif

  CV_CALL( cvFilter2D(src,      laplacian, &this->gaussian_laplacian_) );
/*
#ifdef ACCURATE_CENTROID_DEBUG
  save_mat(basename + "_lapla.pgm", laplacian);
#endif
*/
  CV_CALL( cvFilter2D(laplacian, laplacian, &this->smooth_filt_) );
#ifdef ACCURATE_CENTROID_DEBUG
  save_mat(basename + "_lapla_filt.pgm", laplacian);
#endif


  CV_CALL( temp = cvCloneMat(derived) );
  CV_CALL( row_max = cvCreateMat(size.height, 1, CV_32F) );
  CV_CALL( cvAbs(temp, temp) );
  CV_CALL( cvReduce(temp, row_max, 1, CV_REDUCE_MAX) );

  //- do not consider the row if its weight is < than 20% of the max
  double threshold;
  CV_CALL( cvMinMaxLoc(row_max, 0, &threshold) );
  threshold *= 0.2;


  //- find zero-crossings of the laplacian
  int i, j;
  for (i = 0; i < size.height; i++)
  {
    double weight = cvmGet(row_max, i, 0);

    if (weight < threshold)
      continue;

    int h2l_pos, l2h_pos;
    double h2l_value, l2h_value;
    double prev, cur;

    h2l_pos = l2h_pos = 0;
    h2l_value = l2h_value = 0;
    cur = cvmGet(laplacian, i, 0);
    for (j = 1; j < size.width; j++)
    {
      prev = cur;
      cur = cvmGet(laplacian, i, j);
      if (prev < 0 && cur > 0 && cur - prev > l2h_value)
      {
        //- low-to-high zero-crossing
        l2h_value = cur - prev;
        l2h_pos = j;
      }
      else if (prev > 0 && cur < 0 && prev - cur > h2l_value)
      {
        //- high-to-low zero-crossing
        h2l_value = prev - cur;
        h2l_pos = j;
      }
    }

    //- if found, refine the position by linear interpolation to get sub-pixel 
    //- accuracy on the zero crossing position
    if (h2l_pos != 0 && l2h_pos != 0 && l2h_pos < size.width / 2 && h2l_pos > size.width / 2)
    {
      double h2l_pos_interp, l2h_pos_interp;
      double a,b;
    
      a = - cvmGet(laplacian, i, l2h_pos - 1);
      b = cvmGet(laplacian, i, l2h_pos);
      l2h_pos_interp = l2h_pos - 1 + a / (b + a);

      a = cvmGet(laplacian, i, h2l_pos - 1);
      b = - cvmGet(laplacian, i, h2l_pos);
      h2l_pos_interp = h2l_pos - 1 + a / (b + a);

      CvPoint2D32f center_f;
      center_f.x = static_cast<float>( (l2h_pos_interp + h2l_pos_interp) / 2 );
      center_f.y = static_cast<float>( i );

      centers.push_back(center_f);
      centers_weight.push_back(weight * weight);
    }
  }

  //- fit a line to 'centers' point set, with weight 'centers_weight'
  x = y = x2 = y2 = xy = w = 0;
  for (c_it = centers.begin(), w_it = centers_weight.begin();
       c_it != centers.end();
       c_it++, w_it++)
  {
    CvPoint2D32f p = *c_it;
    double cur_w = *w_it;

    x  += cur_w * p.x;
    y  += cur_w * p.y;
    x2 += cur_w * p.x * p.x;
    y2 += cur_w * p.y * p.y;
    xy += cur_w * p.x * p.y;
    w  += cur_w;
  }

  x /= w;
  y /= w;
  x2 /= w;
  y2 /= w;
  xy /= w;

  dx2 = x2 - x * x;
  dy2 = y2 - y * y;
  dxy = xy - x * y;

  t = ::atan2( 2 * dxy, dx2 - dy2 ) / 2;
  _line.vx = ::cos( t );
  _line.vy = ::sin( t );

  _line.x0 = x;
  _line.y0 = y;

  __END__;

  cvReleaseMat(&src);
  cvReleaseMat(&derived);
  cvReleaseMat(&laplacian);
  cvReleaseMat(&temp);
  cvReleaseMat(&row_max);

  __ISL_CHECK_ERROR__;
}

void
AccurateCentroid::process_cols(IplImage* _src, Line2D& _line)
{
  IplImage* src_t = 0;
  Line2D line_t;

  CV_FUNCNAME( "AccurateCentroid::process_cols" );
  __BEGIN__;

  //- transpose the matrix and process its rows
  CvSize size = cvSize(_src->height, _src->width);
  CV_CALL( src_t = cvCreateImage(size, _src->depth, 1) );
  CV_CALL( cvTranspose(_src, src_t) );

  ISL_CALL( this->process_rows(src_t, _line) );

  //- transpose the resulting line parameters
  line_t.vx = _line.vy;
  line_t.vy = _line.vx;
  line_t.x0 = _line.y0;
  line_t.y0 = _line.x0;
  _line = line_t;

  __END__;
  cvReleaseImage(&src_t);
  __ISL_CHECK_ERROR__;
}



Point2D<double>
AccurateCentroid::compute(const Image& _im)
{
  IplImage* src = _im.get_ipl_image();

  Point2D<double> centroid(0,0);
  Line2D vert_line;
  Line2D hori_line;

  CV_FUNCNAME( "AccurateCentroid::compute" );
  __BEGIN__;

  
#ifdef ACCURATE_CENTROID_DEBUG
  {
    std::stringstream s0;
    s0 << "centroid/sub";
    if (i < 10) s0 << "0";
    s0 << i << "_x_";
    basename = s0.str();
  }
#endif

  ISL_CALL( this->process_rows(src, vert_line) );
  
#ifdef ACCURATE_CENTROID_DEBUG
  {
    std::stringstream s1;
    s1 << "centroid/sub";
    if (i < 10) s1 << "0";
    s1 << i << "_y_" ;
    basename = s1.str();
  }
#endif
  
  ISL_CALL( this->process_cols(src, hori_line) );

#ifdef ACCURATE_CENTROID_DEBUG
  i++;
#endif


  //- intersect the two lines by solving a 2x2 system
  double A_buf[4];
  double B_buf[2];
  double X_buf[2];
  CvMat A, B, X;

  A_buf[0] = vert_line.vx;
  A_buf[1] = - hori_line.vx;
  A_buf[2] = vert_line.vy;
  A_buf[3] = - hori_line.vy;

  B_buf[0] = hori_line.x0 - vert_line.x0;
  B_buf[1] = hori_line.y0 - vert_line.y0;

  A = cvMat(2, 2, CV_64F, A_buf);
  B = cvMat(2, 1, CV_64F, B_buf);
  X = cvMat(2, 1, CV_64F, X_buf);

  CV_CALL( cvSolve(&A, &B, &X) );

  centroid.x() = vert_line.x0 + X_buf[0] * vert_line.vx;
  centroid.y() = vert_line.y0 + X_buf[0] * vert_line.vy;

  __END__;
  __ISL_CHECK_ERROR__;
  return(centroid);
}


}
