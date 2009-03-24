#include <isl/statistics/PrincipalAxis.h>
#include "InternalErrorHandler.h"

#if !defined (__ISL_INLINE__)
# include <isl/statistics/PrincipalAxis.i>
#endif


namespace isl {

PrincipalAxis::PrincipalAxis(double mx, double my, double cxx, double cxy, double cyy)
{
  CV_FUNCNAME( "PrincipalAxis::PrincipalAxis" );
  __BEGIN__;

  CV_CALL( this->covariance_    = cvCreateMat( 2, 2, CV_64FC1 ) );
  CV_CALL( this->eigen_vectors_ = cvCreateMat( 2, 2, CV_64FC1 ) );
  CV_CALL( this->eigen_values_  = cvCreateMat( 2, 1, CV_64FC1 ) );

  CV_CALL( cvZero(this->covariance_   ) );
  CV_CALL( cvZero(this->eigen_vectors_) );
  CV_CALL( cvZero(this->eigen_values_ ) );

  this->compute(mx, my, cxx, cxy, cyy);

  __END__;

  if( cvGetErrStatus() < 0 )
  {
    __ISL_DISABLE_ERROR__;
    this->release();
    __ISL_RESTORE_ERROR__;
  }
  __ISL_CHECK_ERROR__;
}

PrincipalAxis::~PrincipalAxis()
{
  this->release();
}

void PrincipalAxis::release()
{
  cvReleaseMat( &this->covariance_ );
  cvReleaseMat( &this->eigen_vectors_ );
  cvReleaseMat( &this->eigen_values_ );
}

void
PrincipalAxis::compute(double mx, double my, double cxx, double cxy, double cyy)
{
   CV_FUNCNAME( "PrincipalAxis::compute" );
  __BEGIN__;
  
  // compute the center
  this->center_.x = mx;
  this->center_.y = my;

  // fill in covariance matrix
  cvmSet( this->covariance_, 0, 0, cxx );
  cvmSet( this->covariance_, 0, 1, cxy );
  cvmSet( this->covariance_, 1, 0, cxy );
  cvmSet( this->covariance_, 1, 1, cyy );

  // compute the eigen analysis
  CV_CALL( cvSVD( this->covariance_, this->eigen_values_, this->eigen_vectors_) );

  // make the necessary arrangement to have well-oriented eigen vectors
  // we want the coordinates of the first axis to be (+,+) or (+,-)
  // and the second vector to be oriented in the trigonometric direction
  CvPoint2D64f first_axis;
  first_axis.x  = cvmGet(this->eigen_vectors_, 0, 0);
  first_axis.y  = cvmGet(this->eigen_vectors_, 1, 0);

  if (first_axis.x < 0)
  {
    cvmSet(this->eigen_vectors_, 0, 0, - first_axis.x);
    cvmSet(this->eigen_vectors_, 1, 0, - first_axis.y);
  }

  // the second eigen vector is orthogonal to the first (covariance is symetric)
  cvmSet(this->eigen_vectors_, 0, 1, - first_axis.y);
  cvmSet(this->eigen_vectors_, 1, 1, first_axis.x);


  // update angle
  this->angle_ = atan2(cvmGet(this->eigen_vectors_, 1, 0), cvmGet(this->eigen_vectors_, 0, 0));

  __END__;
  __ISL_CHECK_ERROR__;
}


}
