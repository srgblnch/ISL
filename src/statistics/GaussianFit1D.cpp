//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#include <isl/statistics/GaussianFit1D.h>
#include "InternalErrorHandler.h"
#include <isl/Constants.h>
#include <isl/statistics/Moments.h>

#if !defined (__ISL_INLINE__)
# include <isl/statistics/GaussianFit1D.i>
#endif

namespace isl {


GaussianFit1D::GaussianFit1D()
: x_(0),
  fixed_bg(false)
{
}

GaussianFit1D::~GaussianFit1D()
{
}



void
GaussianFit1D::compute(const double* vector,
                       size_t vector_size,
                       double magnitude,
                       double mean,
                       double covariance,
                       double background)
{
  CvMat* init_guess = 0;
  CvMat* x = 0;
  void* data;

  int i;
  int s = static_cast<int>(vector_size);

  CV_FUNCNAME( "GaussianFit1D::compute" );
  __BEGIN__;

  if (magnitude <= DBL_EPSILON || covariance <= DBL_EPSILON)
  {
    //- the data cannot be fitted
    CV_ERROR( CV_StsBadArg, "Data cannot be fitted");
  }

  //- compute the abscissa axis as [0 ... n-1]
  CV_CALL( x = cvCreateMat(s, 1, CV_64FC1) );
  for (i = 0; i < s; i++)
    cvmSet(x, i, 0, i);
  
  this->x_ = x;

  data = static_cast<void*>(const_cast<double*>(vector));
  CV_CALL( cvInitMatHeader(&this->observ_, s, 1, CV_64FC1, data) );
  
  
  if (this->fixed_bg == false)
  {
    CV_CALL( init_guess = cvCreateMat(4, 1, CV_64F) );
    cvmSet(init_guess, 3, 0, background);
  }
  else
  {
    CV_CALL( init_guess = cvCreateMat(3, 1, CV_64F) );
  }
  cvmSet(init_guess, 0, 0, magnitude);
  cvmSet(init_guess, 1, 0, mean);
  cvmSet(init_guess, 2, 0, covariance);
  
  this->init(s, init_guess);
  
  this->run(ISL_LMOPTIM_SCALE_JAC);

  __END__;

  cvReleaseMat(&this->x_);
  cvReleaseMat(&init_guess);

  __ISL_CHECK_ERROR__;
}


void
GaussianFit1D::compute(const double* vector,
                       size_t vector_size)
{
  double magnitude, mean, covariance, background;


  CV_FUNCNAME( "GaussianFit1D::compute" );
  __BEGIN__;

  this->initial_guess(vector, vector_size, magnitude, mean, covariance, background);

  ISL_CALL( this->compute(vector, vector_size, magnitude, mean, covariance, background) );

  __END__;
  __ISL_CHECK_ERROR__;
}



void
GaussianFit1D::compute_fixed_bg(const double* vector,
                                size_t vector_size,
                                double _fixed_bg_value)
{
  double magnitude, mean, covariance;


  CV_FUNCNAME( "GaussianFit1D::compute" );
  __BEGIN__;
  
  this->fixed_bg = true;
  this->fixed_bg_value = _fixed_bg_value;

  this->initial_guess(vector, vector_size, magnitude, mean, covariance, _fixed_bg_value);

  ISL_CALL( this->compute(vector, vector_size, magnitude, mean, covariance, _fixed_bg_value) );

  __END__;
  __ISL_CHECK_ERROR__;
}



double
GaussianFit1D::get_fitted_value(int idx)
{
	double xc = static_cast<double>(idx) - this->mean();
	double value = this->magnitude() * ::exp( - 0.5 * xc * xc / this->variance() ) + this->background(); 
	return(value);
}

double
GaussianFit1D::get_fitted_error(int idx)
{
	return cvmGet(&this->observ_, idx, 0) - this->get_fitted_value(idx);
}

void GaussianFit1D::initial_guess(const double* vector,
                                  size_t vector_size,
                                  double& magnitude,
                                  double& mean,
                                  double& covariance,
                                  double& background)
{

  if (vector_size < 3)
  {
    magnitude = vector[0];
    mean = 0;
    covariance = 1;
    background = 0;
    return;
  }

  //-----------------------
  //- BACKGROUND
  //-----------------------
  if (this->fixed_bg == false)
  {
    const int kBG_FINDING_N = 5; // we will take the 5 first points
                                 // and the 5 last points

    if (vector_size < 2 * kBG_FINDING_N)
    {
      //- choose the min between the first and the last point
      background = MIN(vector[0], vector[vector_size - 1]);
    }
    else
    {
      //- compute the mean of the first kBG_FINDING_N points
      //- and the mean of the last kBG_FINDING_N points

      //- then choose the min as the background value

      double bg_begin, bg_end;
      bg_begin = bg_end = 0;
      for (int i = 0; i < kBG_FINDING_N; i++)
      {
        bg_begin += vector[i];
        bg_end   += vector[vector_size - 1 - i];
      }
      bg_begin /= kBG_FINDING_N;
      bg_end   /= kBG_FINDING_N;
      background = MIN(bg_begin, bg_end);
    }
  }


  //-----------------------
  //- CENTER
  //----------------------- 
  size_t maxima_pos = 0;
  double maxima_val = vector[0];
  for (size_t i = 1; i < vector_size; i++)
  {
    if (vector[i] > maxima_val)
    {
      maxima_pos = i;
      maxima_val = vector[i];
    }
  }
  mean = static_cast<double>(maxima_pos);

  //-----------------------
  //- SIGMA
  //----------------------- 
  double sigma;
  {
    double val = vector[maxima_pos];
    size_t pos = maxima_pos;

    //- background is necessarily inferior to the max value
    //- (just see how it is computed)
    double half_top_mag = 0.5 * (background + val);
  
    if (maxima_pos < vector_size / 2)
    {
      //- look in increasing direction
      while( pos < vector_size - 1 && val > half_top_mag)
      {
        val = vector[++pos];
      }
    }
    else
    {
      //- look in decreasing direction
      while( pos > 0 && val > half_top_mag)
      {
        val = vector[--pos];
      }
    }

    if (pos != maxima_pos)
    {
      sigma = ::fabs(static_cast<double>(pos) - static_cast<double>(maxima_pos));
    }
    else
    {
      //- try an arbitrary value that should be not so far from reality
      sigma = vector_size / 6;
    }

    covariance = sigma * sigma;
  }


  //-----------------------
  //- MAGNITUDE
  //----------------------- 
  {
    //- compute the area under the curve
    //- by the trapezoidal method
    double sum = 0;
  
    for (size_t i = 0; i < vector_size; i++)
    {
      sum += vector[i];
    }
    sum -= 0.5 * (vector[0] + vector[vector_size - 1]);

    //- remove the contribution of the background
    sum -= vector_size * background;


    if (sum > 0)
    {
      //- the magnitude and the area are linked by : 
      //-    magnitude = A / sqrt(2 * pi * sigma²)
      magnitude = sum / ::sqrt(2 * PI * covariance);
    }
    else
    {
      //- estimated background too high
      //- poor chances of convergence

      //- try with the maximal value
      magnitude = vector[maxima_pos];
    }
  }
}


void
GaussianFit1D::errfunc(CvMat* _params, CvMat* _err_func) const
{
  CvMat* xc = 0;
  CvMat* xc2 = 0;
  CvMat* quad_part = 0;
  CvMat* exp_part = 0;
  
  int n = this->nb_func_;

  CV_FUNCNAME( "GaussianFit1D::errfunc" );
  __BEGIN__;
 
  double magnitude  = cvmGet(_params, 0, 0);
  double mean       = cvmGet(_params, 1, 0);
  double covar      = cvmGet(_params, 2, 0);
  
  double background;
  if (this->fixed_bg == false)
    background = cvmGet(_params, 3, 0);
  else
    background = fixed_bg_value;


  CV_CALL( xc = cvCloneMat(this->x_) );

  //- xc
  CV_CALL( cvSubS(xc, cvRealScalar(mean), xc) );
 
  //- xc ^ 2
  CV_CALL( xc2 = cvCreateMat(n, 1, CV_64FC1) );
  CV_CALL( cvMul(xc, xc, xc2) );

  //- quad_part = - xc^2 / (2 * covar)
  CV_CALL( quad_part = cvCreateMat(n, 1, CV_64FC1) );
  CV_CALL( cvScale(xc2, quad_part, - 0.5 / covar ) );
  
  //- exp(quad_part)
  CV_CALL( exp_part = cvCreateMat(n, 1, CV_64FC1) );
  CV_CALL( cvExp(quad_part, exp_part) );

  //- A * exp_part
  CV_CALL( cvScale(exp_part, _err_func, magnitude) );

  //- A * exp_part + background
  CV_CALL( cvAddS(_err_func, cvRealScalar(background), _err_func) );

  CV_CALL( cvSub(&this->observ_, _err_func, _err_func) );

  __END__;

  cvReleaseMat(&xc);
  cvReleaseMat(&xc2);
  cvReleaseMat(&quad_part);
  cvReleaseMat(&exp_part);

  __ISL_CHECK_ERROR__;
}

void
GaussianFit1D::errfunc_and_jac(CvMat* _params, CvMat* _err_func, CvMat* _jac) const
{

  CvMat* xc = 0;
  CvMat* xc2 = 0;
  CvMat* quad_part = 0;
  CvMat* exp_part = 0;
  
  int n = this->nb_func_;

  CV_FUNCNAME( "GaussianFit1D::errfunc_and_jac" );
  __BEGIN__;

 
  double magnitude  = cvmGet(_params, 0, 0);
  double mean       = cvmGet(_params, 1, 0);
  double covar      = cvmGet(_params, 2, 0);
  
  double background;
  if (this->fixed_bg == false)
    background = cvmGet(_params, 3, 0);
  else
    background = fixed_bg_value;

  CV_CALL( xc = cvCloneMat(this->x_) );

  //- xc
  CV_CALL( cvSubS(xc, cvRealScalar(mean), xc) );
 
  //- xc ^ 2
  CV_CALL( xc2 = cvCreateMat(n, 1, CV_64FC1) );
  CV_CALL( cvMul(xc, xc, xc2) );

  //- quad_part = - xc^2 / (2 * covar)
  CV_CALL( quad_part = cvCreateMat(n, 1, CV_64FC1) );
  CV_CALL( cvScale(xc2, quad_part, - 0.5 / covar ) );
  
  //- exp(quad_part)
  CV_CALL( exp_part = cvCreateMat(n, 1, CV_64FC1) );
  CV_CALL( cvExp(quad_part, exp_part) );


  
  //-
  //- Fill the Function
  //-
  CV_CALL( cvScale(exp_part, _err_func, magnitude) );

  CV_CALL( cvAddS(_err_func, cvRealScalar(background), _err_func) );

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
  CV_CALL( cvMul(exp_part, xc, &dfxm, magnitude / covar) );

  //- dfc = df / dc
  CvMat dfc;
  CV_CALL( cvGetCol(_jac, &dfc, 2) );
  CV_CALL( cvMul(exp_part, xc2, &dfc, magnitude / (2 * covar * covar)) );

  if (this->fixed_bg == false)
  {
    //- dfb = df / db
    CvMat dfb;
    CV_CALL( cvGetCol(_jac, &dfb, 3) );
    CV_CALL( cvZero(&dfb) );
    CV_CALL( cvAddS(&dfb, cvRealScalar(1.f), &dfb) );
  }

  __END__;

  cvReleaseMat(&xc);
  cvReleaseMat(&xc2);
  cvReleaseMat(&quad_part);
  cvReleaseMat(&exp_part);

  __ISL_CHECK_ERROR__;
}


void
GaussianFit1D::jac(CvMat* _params, CvMat* _jac) const
{
  CvMat* xc = 0;
  CvMat* xc2 = 0;
  CvMat* quad_part = 0;
  CvMat* exp_part = 0;

  int n = this->nb_func_;

  CV_FUNCNAME( "GaussianFit1D::jac" );
  __BEGIN__;

 
  double magnitude  = cvmGet(_params, 0, 0);
  double mean       = cvmGet(_params, 1, 0);
  double covar      = cvmGet(_params, 2, 0);

  CV_CALL( xc = cvCloneMat(this->x_) );

  //- xc
  CV_CALL( cvSubS(xc, cvRealScalar(mean), xc) );
 
  //- xc ^ 2
  CV_CALL( xc2 = cvCreateMat(n, 1, CV_64FC1) );
  CV_CALL( cvMul(xc, xc, xc2) );

  //- quad_part = - xc^2 / (2 * covar)
  CV_CALL( quad_part = cvCreateMat(n, 1, CV_64FC1) );
  CV_CALL( cvScale(xc2, quad_part, - 0.5 / covar ) );
  
  //- exp(quad_part)
  CV_CALL( exp_part = cvCreateMat(n, 1, CV_64FC1) );
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
  CV_CALL( cvMul(exp_part, xc, &dfxm, magnitude / covar) );

  //- dfc = df / dc
  CvMat dfc;
  CV_CALL( cvGetCol(_jac, &dfc, 2) );
  CV_CALL( cvMul(exp_part, xc2, &dfc, magnitude / (2 * covar * covar)) );
  
  if (this->fixed_bg == false)
  {
    //- dfb = df / db
    CvMat dfb;
    CV_CALL( cvGetCol(_jac, &dfb, 3) );
    CV_CALL( cvZero(&dfb) );
    CV_CALL( cvAddS(&dfb, cvRealScalar(1.f), &dfb) );
  }

  __END__;

  cvReleaseMat(&xc);
  cvReleaseMat(&xc2);
  cvReleaseMat(&quad_part);
  cvReleaseMat(&exp_part);

  __ISL_CHECK_ERROR__;
}




}
