//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#include <isl/statistics/GaussianFit2D.h>
#include "InternalErrorHandler.h"

#include <isl/Image.h>
#include <isl/statistics/Projections.h>
#include <isl/statistics/GaussianFit1D.h>

#if !defined (__ISL_INLINE__)
# include <isl/statistics/GaussianFit2D.i>
#endif

namespace isl {

GaussianFit2D::GaussianFit2D()
: x_(0),
  y_(0),
  xc_(0),
  yc_(0),
  xc2_(0),
  yc2_(0),
  xcyc_(0),
  quad_part_(0),
  exp_part_(0)
{
}

GaussianFit2D::~GaussianFit2D()
{
}

void
GaussianFit2D::initial_guess(const Image& image,
                             double& magnitude,
                             double& mean_x,
                             double& mean_y,
                             double& c_xx,
                             double& c_xy,
                             double& c_yy,
                             double& background)
{
  CV_FUNCNAME( "GaussianFit2D::initial_guess" );
  __BEGIN__;
  
  //-----------------------
  //- BACKGROUND
  //-----------------------
  {
    //- analyze the 4 bands of kBG_FINDING_N pixels on the edges of the image
    const int kBG_FINDING_N = 5;

    if (image.height() > 2 * kBG_FINDING_N && image.width() > 2 * kBG_FINDING_N)
    {
      double mean_band[4];
      CvMat band;
      
      CvScalar s;

      //- upper band
      {
        CV_CALL( cvGetSubRect(image.get_ipl_image(), 
                              &band,
                              cvRect(0,
                                     0, 
                                     image.width(), 
                                     kBG_FINDING_N)) );
        CV_CALL( s = cvAvg(&band) );
        mean_band[0] = s.val[0];
      }

      //- lower band
      {
        CV_CALL( cvGetSubRect(image.get_ipl_image(), 
                              &band,
                              cvRect(0,
                                     image.height() - 1 - kBG_FINDING_N,
                                     image.width(),
                                     kBG_FINDING_N)) );
        CV_CALL( s = cvAvg(&band) );
        mean_band[1] = s.val[0];
      }
      
      //- left band
      {
        CV_CALL( cvGetSubRect(image.get_ipl_image(), 
                              &band,
                              cvRect(0,
                                     0,
                                     kBG_FINDING_N,
                                     image.height())) );
        CV_CALL( s = cvAvg(&band) );
        mean_band[2] = s.val[0];
      }
      
      //- right band
      {
        CV_CALL( cvGetSubRect(image.get_ipl_image(), 
                              &band,
                              cvRect(image.width() - 1 - kBG_FINDING_N,
                                     0,
                                     kBG_FINDING_N,
                                     image.height())) );
        CV_CALL( s = cvAvg(&band) );
        mean_band[3] = s.val[0];
      }

      background = ( mean_band[0] + mean_band[1] + mean_band[2] + mean_band[3] ) / 4;
    }
    else
    {
      //- take the minimal pixel value in the image
      CV_CALL( cvMinMaxLoc(image.get_ipl_image(), &background, 0) );
    }

  }
  
  
  //-----------------------
  //- CENTER
  //-----------------------
  double max_val;
  {
    CvPoint max_loc;
    
    CV_CALL( cvMinMaxLoc(image.get_ipl_image(), 0, &max_val, 0, &max_loc) );

    mean_x = max_loc.x;
    mean_y = max_loc.y;
  }
  
  
  //-----------------------
  //- SIGMA
  //-----------------------
  {
    Projections p(image);
    GaussianFit1D g;
    
    g.compute(p.get_x_projection(), p.size_x());
    c_xx = g.variance();
    
    g.compute(p.get_y_projection(), p.size_y());
    c_yy = g.variance();

    c_xy = 0;
  }
 

  
  //-----------------------
  //- MAGNITUDE
  //-----------------------
  {
    if (max_val - background > 0)
      magnitude = max_val - background;
    else
      magnitude = max_val;
  }

  __END__;
  __ISL_CHECK_ERROR__;
}

void
GaussianFit2D::compute(const Image& image,
                       double magnitude,
                       double mean_x,
                       double mean_y,
                       double c_xx,
                       double c_xy,
                       double c_yy,
                       double background)
{

  CvMat* init_guess = 0;
  CvMat* seed_x = 0;
  int w,h,i,j,jw,n;
  
  CV_FUNCNAME( "GaussianFit2D::compute" );
  __BEGIN__;

  if (magnitude <= DBL_EPSILON || c_xx <= DBL_EPSILON || c_yy <= DBL_EPSILON)
  {
    //- the data cannot be fitted
    CV_ERROR( CV_StsBadArg, "Data cannot be fitted");
  }
  
  //- compute the mesh grid of pixel coordinates
  w = image.width();
  h = image.height();
  n = w * h;
  jw = 0;

  CV_CALL( this->x_ = cvCreateMat(n, 1, CV_64FC1) );
  CV_CALL( seed_x = cvCreateMat(w, 1, CV_64FC1) );
  for (i = 0; i < w; i++)
    cvmSet(seed_x, i, 0, i );
  CV_CALL( cvRepeat(seed_x, this->x_) );
  
  CV_CALL( this->y_ = cvCreateMat(n, 1, CV_64FC1) );
  CvMat tmp_hdr;
  
  for (j = 0; j < h; j++)
  {
    CV_CALL( cvGetRows(this->y_, &tmp_hdr, jw, jw + w) );
    CV_CALL( cvSet(&tmp_hdr, cvRealScalar(j)) );
    jw += w;
  }

  CV_CALL( this->xc_        = cvCreateMat(n, 1, CV_64FC1) );
  CV_CALL( this->yc_        = cvCreateMat(n, 1, CV_64FC1) );
  CV_CALL( this->xc2_       = cvCreateMat(n, 1, CV_64FC1) );
  CV_CALL( this->yc2_       = cvCreateMat(n, 1, CV_64FC1) );
  CV_CALL( this->xcyc_      = cvCreateMat(n, 1, CV_64FC1) );
  CV_CALL( this->quad_part_ = cvCreateMat(n, 1, CV_64FC1) );
  CV_CALL( this->exp_part_  = cvCreateMat(n, 1, CV_64FC1) );

  //- get the input data
  CV_CALL( cvReshape(image.get_ipl_image(), &this->observ_, 0, n) );

  CV_CALL( init_guess = cvCreateMat(7, 1, CV_64F) );

  cvmSet(init_guess, 0, 0, magnitude);
  cvmSet(init_guess, 1, 0, mean_x);
  cvmSet(init_guess, 2, 0, mean_y);
  cvmSet(init_guess, 3, 0, c_xx);
  cvmSet(init_guess, 4, 0, c_xy);
  cvmSet(init_guess, 5, 0, c_yy);
  cvmSet(init_guess, 6, 0, background);

  ISL_CALL( this->init(n, init_guess) );


  ISL_CALL( this->run(ISL_LMOPTIM_SCALE_JAC) );


  __END__;

  cvReleaseMat(&seed_x);
  cvReleaseMat(&init_guess);
  cvReleaseMat(&this->x_);
  cvReleaseMat(&this->y_);
  cvReleaseMat(&this->xc_);
  cvReleaseMat(&this->yc_);
  cvReleaseMat(&this->xc2_);
  cvReleaseMat(&this->yc2_);
  cvReleaseMat(&this->xcyc_);
  cvReleaseMat(&this->quad_part_);
  cvReleaseMat(&this->exp_part_);

  __ISL_CHECK_ERROR__;
}

void
GaussianFit2D::compute(const Image& image)
{
  double magnitude, mean_x, mean_y, c_xx, c_xy, c_yy, background;

  CV_FUNCNAME( "GaussianFit2D::compute" );
  __BEGIN__;
  
  ISL_CALL( this->initial_guess(image, magnitude, mean_x, mean_y, c_xx, c_xy, c_yy, background) );

  ISL_CALL( this->compute(image, magnitude, mean_x, mean_y, c_xx, c_xy, c_yy, background) );

  __END__;
  __ISL_CHECK_ERROR__;
}







void
GaussianFit2D::errfunc(CvMat* _params, CvMat* _err_func) const
{
  CvMat* x         = this->x_;
  CvMat* y         = this->y_;
  CvMat* xc        = this->xc_;
  CvMat* yc        = this->yc_;
  CvMat* xc2       = this->xc2_;
  CvMat* yc2       = this->yc2_;
  CvMat* xcyc      = this->xcyc_;
  CvMat* quad_part = this->quad_part_;
  CvMat* exp_part  = this->exp_part_;

  double xc2_coeff = 0;
  double yc2_coeff = 0;
  double xcyc_coeff = 0;
 

  CV_FUNCNAME( "gaussian_function" );
  __BEGIN__;
 
  double magnitude = cvmGet(_params, 0, 0);
  double mean_x    = cvmGet(_params, 1, 0);
  double mean_y    = cvmGet(_params, 2, 0);
  double c_xx      = cvmGet(_params, 3, 0);
  double c_xy      = cvmGet(_params, 4, 0);
  double c_yy      = cvmGet(_params, 5, 0);
  double bg        = cvmGet(_params, 6, 0);

  double det_covar = c_xx * c_yy - c_xy * c_xy;
  double det_inv_scal = - 0.5 / det_covar;

  CV_CALL( cvCopy(x, xc) );
  CV_CALL( cvCopy(y, yc) );

  //- xc
  CV_CALL( cvSubS(xc, cvRealScalar(mean_x), xc) );

  //- yc
  CV_CALL( cvSubS(yc, cvRealScalar(mean_y), yc) );

  xc2_coeff = c_yy * det_inv_scal;
  yc2_coeff = c_xx * det_inv_scal;
  xcyc_coeff = - 2 * c_xy * det_inv_scal;
 
  //- xc ^ 2
  CV_CALL( cvMul(xc, xc, xc2) );

  //- yc ^ 2
  CV_CALL( cvMul(yc, yc, yc2) );

  //- xc * yc
  CV_CALL( cvMul(xc, yc, xcyc) );

  //- quad_part = xc2_coeff * xc^2 + yc2_coeff * yc^2 + xcyc_coeff * xc * yc
  CV_CALL( cvAddWeighted(xc2, xc2_coeff, yc2, yc2_coeff, 0, quad_part) );
  CV_CALL( cvAddWeighted(quad_part, 1, xcyc, xcyc_coeff, 0, quad_part) );
  
  //- exp(quad_part)
  CV_CALL( cvExp(quad_part, exp_part) );

  //- A * exp_part
  CV_CALL( cvScale(exp_part, _err_func, magnitude) );

  //- A * exp_part + background
  CV_CALL( cvAddS(_err_func, cvRealScalar(bg), _err_func) );

  CV_CALL( cvSub(&this->observ_, _err_func, _err_func) );
  
  __END__;
  __ISL_CHECK_ERROR__;
}

void
GaussianFit2D::errfunc_and_jac(CvMat* _params, CvMat* _err_func, CvMat* _jac) const
{
  CvMat* x         = this->x_;
  CvMat* y         = this->y_;
  CvMat* xc        = this->xc_;
  CvMat* yc        = this->yc_;
  CvMat* xc2       = this->xc2_;
  CvMat* yc2       = this->yc2_;
  CvMat* xcyc      = this->xcyc_;
  CvMat* quad_part = this->quad_part_;
  CvMat* exp_part  = this->exp_part_;

  double xc2_coeff = 0;
  double yc2_coeff = 0;
  double xcyc_coeff = 0;

  CV_FUNCNAME( "gaussian_function_and_jacobian" );
  __BEGIN__;
 
  double magnitude = cvmGet(_params, 0, 0);
  double mean_x    = cvmGet(_params, 1, 0);
  double mean_y    = cvmGet(_params, 2, 0);
  double c_xx      = cvmGet(_params, 3, 0);
  double c_xy      = cvmGet(_params, 4, 0);
  double c_yy      = cvmGet(_params, 5, 0);
  double bg        = cvmGet(_params, 6, 0);

  double det_covar = c_xx * c_yy - c_xy * c_xy;
  double det_inv_scal = - 0.5 / det_covar;

  CV_CALL( cvCopy(x, xc) );
  CV_CALL( cvCopy(y, yc) );

  //- xc
  CV_CALL( cvSubS(xc, cvRealScalar(mean_x), xc) );

  //- yc
  CV_CALL( cvSubS(yc, cvRealScalar(mean_y), yc) );

  xc2_coeff = c_yy * det_inv_scal;
  yc2_coeff = c_xx * det_inv_scal;
  xcyc_coeff = - 2 * c_xy * det_inv_scal;
 
  //- xc ^ 2
  CV_CALL( cvMul(xc, xc, xc2) );

  //- yc ^ 2
  CV_CALL( cvMul(yc, yc, yc2) );

  //- xc * yc
  CV_CALL( cvMul(xc, yc, xcyc) );

  //- quad_part = xc2_coeff * xc^2 + yc2_coeff * yc^2 + xcyc_coeff * xc * yc
  CV_CALL( cvAddWeighted(xc2, xc2_coeff, yc2, yc2_coeff, 0, quad_part) );
  CV_CALL( cvAddWeighted(quad_part, 1, xcyc, xcyc_coeff, 0, quad_part) );
  
  //- exp(quad_part)
  CV_CALL( cvExp(quad_part, exp_part) );



  //-
  //- Fill the Function
  //-
  CV_CALL( cvScale(exp_part, _err_func, magnitude) );

  CV_CALL( cvAddS(_err_func, cvRealScalar(bg), _err_func) );

  CV_CALL( cvSub(&this->observ_, _err_func, _err_func) );

  //-
  //- Fill the Jacobian
  //-

  //- dfA = df / dA
  CvMat dfA;
  CV_CALL( cvGetCol(_jac, &dfA, 0) );
  CV_CALL( cvCopy(exp_part, &dfA) );


  //- dfxm = df / dxm
  CvMat dfxm;
  CV_CALL( cvGetCol(_jac, &dfxm, 1) );
  CV_CALL( cvAddWeighted(xc, - 2 * xc2_coeff * magnitude, yc, - xcyc_coeff * magnitude, 0, &dfxm) );
  CV_CALL( cvMul(&dfxm, exp_part, &dfxm) );
  
  //- dfym = df / dym
  CvMat dfym;
  CV_CALL( cvGetCol(_jac, &dfym, 2) );
  CV_CALL( cvAddWeighted(yc, - 2 * yc2_coeff * magnitude, xc, - xcyc_coeff * magnitude, 0, &dfym) );
  CV_CALL( cvMul(&dfym, exp_part, &dfym) );


  //- dfcxx = df / dcxx
  CvMat dfcxx;
  CV_CALL( cvGetCol(_jac, &dfcxx, 3) );
  CV_CALL( cvAddWeighted(quad_part, 2 * c_yy * magnitude * det_inv_scal, yc2, det_inv_scal * magnitude, 0, &dfcxx) );
  CV_CALL( cvMul(&dfcxx, exp_part, &dfcxx) );

  //- dfcxy = df / dcxy
  CvMat dfcxy;
  CV_CALL( cvGetCol(_jac, &dfcxy, 4) );
  CV_CALL( cvAddWeighted(quad_part, - 4 * c_xy * magnitude * det_inv_scal, xcyc, - 2 * det_inv_scal * magnitude, 0, &dfcxy) );
  CV_CALL( cvMul(&dfcxy, exp_part, &dfcxy) );

  
  //- dfcyy = df / dcyy
  CvMat dfcyy;
  CV_CALL( cvGetCol(_jac, &dfcyy, 5) );
  CV_CALL( cvAddWeighted(quad_part, 2 * c_xx * magnitude * det_inv_scal, xc2, det_inv_scal * magnitude, 0, &dfcyy) );
  CV_CALL( cvMul(&dfcyy, exp_part, &dfcyy) );
  
  //- dfb = df / db
  CvMat dfb;
  CV_CALL( cvGetCol(_jac, &dfb, 6) );
  CV_CALL( cvZero(&dfb) );
  CV_CALL( cvAddS(&dfb, cvRealScalar(1.f), &dfb) );

  __END__;
  __ISL_CHECK_ERROR__;
}

void
GaussianFit2D::jac(CvMat* _params, CvMat* _jac) const
{
  CvMat* x         = this->x_;
  CvMat* y         = this->y_;
  CvMat* xc        = this->xc_;
  CvMat* yc        = this->yc_;
  CvMat* xc2       = this->xc2_;
  CvMat* yc2       = this->yc2_;
  CvMat* xcyc      = this->xcyc_;
  CvMat* quad_part = this->quad_part_;
  CvMat* exp_part  = this->exp_part_;

  double xc2_coeff = 0;
  double yc2_coeff = 0;
  double xcyc_coeff = 0;
 
  CV_FUNCNAME( "gaussian_jacobian" );
  __BEGIN__;
 
  double magnitude = cvmGet(_params, 0, 0);
  double mean_x    = cvmGet(_params, 1, 0);
  double mean_y    = cvmGet(_params, 2, 0);
  double c_xx      = cvmGet(_params, 3, 0);
  double c_xy      = cvmGet(_params, 4, 0);
  double c_yy      = cvmGet(_params, 5, 0);

  double det_covar = c_xx * c_yy - c_xy * c_xy;
  double det_inv_scal = - 0.5 / det_covar;

  CV_CALL( cvCopy(x, xc) );
  CV_CALL( cvCopy(y, yc) );

  //- xc
  CV_CALL( cvSubS(xc, cvRealScalar(mean_x), xc) );

  //- yc
  CV_CALL( cvSubS(yc, cvRealScalar(mean_y), yc) );

  xc2_coeff = c_yy * det_inv_scal;
  yc2_coeff = c_xx * det_inv_scal;
  xcyc_coeff = - 2 * c_xy * det_inv_scal;
 
  //- xc ^ 2
  CV_CALL( cvMul(xc, xc, xc2) );

  //- yc ^ 2
  CV_CALL( cvMul(yc, yc, yc2) );

  //- xc * yc
  CV_CALL( cvMul(xc, yc, xcyc) );

  //- quad_part = xc2_coeff * xc^2 + yc2_coeff * yc^2 + xcyc_coeff * xc * yc
  CV_CALL( cvAddWeighted(xc2, xc2_coeff, yc2, yc2_coeff, 0, quad_part) );
  CV_CALL( cvAddWeighted(quad_part, 1, xcyc, xcyc_coeff, 0, quad_part) );
  
  //- exp(quad_part)
  CV_CALL( cvExp(quad_part, exp_part) );


  //-
  //- Fill the Jacobian
  //-

  //- dfA = df / dA
  CvMat dfA;
  CV_CALL( cvGetCol(_jac, &dfA, 0) );
  CV_CALL( cvCopy(exp_part, &dfA) );


  //- dfxm = df / dxm
  CvMat dfxm;
  CV_CALL( cvGetCol(_jac, &dfxm, 1) );
  CV_CALL( cvAddWeighted(xc, - 2 * xc2_coeff * magnitude, yc, - xcyc_coeff * magnitude, 0, &dfxm) );
  CV_CALL( cvMul(&dfxm, exp_part, &dfxm) );
  
  //- dfym = df / dym
  CvMat dfym;
  CV_CALL( cvGetCol(_jac, &dfym, 2) );
  CV_CALL( cvAddWeighted(yc, - 2 * yc2_coeff * magnitude, xc, - xcyc_coeff * magnitude, 0, &dfym) );
  CV_CALL( cvMul(&dfym, exp_part, &dfym) );


  //- dfcxx = df / dcxx
  CvMat dfcxx;
  CV_CALL( cvGetCol(_jac, &dfcxx, 3) );
  CV_CALL( cvAddWeighted(quad_part, 2 * c_yy * magnitude * det_inv_scal, yc2, det_inv_scal * magnitude, 0, &dfcxx) );
  CV_CALL( cvMul(&dfcxx, exp_part, &dfcxx) );

  //- dfcxy = df / dcxy
  CvMat dfcxy;
  CV_CALL( cvGetCol(_jac, &dfcxy, 4) );
  CV_CALL( cvAddWeighted(quad_part, - 4 * c_xy * magnitude * det_inv_scal, xcyc, - 2 * det_inv_scal * magnitude, 0, &dfcxy) );
  CV_CALL( cvMul(&dfcxy, exp_part, &dfcxy) );

  
  //- dfcyy = df / dcyy
  CvMat dfcyy;
  CV_CALL( cvGetCol(_jac, &dfcyy, 5) );
  CV_CALL( cvAddWeighted(quad_part, 2 * c_xx * magnitude * det_inv_scal, xc2, det_inv_scal * magnitude, 0, &dfcyy) );
  CV_CALL( cvMul(&dfcyy, exp_part, &dfcyy) );
  
  //- dfb = df / db
  CvMat dfb;
  CV_CALL( cvGetCol(_jac, &dfb, 6) );
  CV_CALL( cvSet(&dfb, cvRealScalar(1.f)) );

  __END__;
  __ISL_CHECK_ERROR__;
}








}
