#include <isl/statistics/Moments.h>
#include "InternalErrorHandler.h"
#include <isl/Image.h>

#if !defined (__ISL_INLINE__)
# include <isl/statistics/Moments.i>
#endif


namespace isl {

Moments2D::Moments2D(const Image& image)
{
  this->compute(image);
}

Moments2D::Moments2D(const Moments2D& m)
{
  *this = m;
}

Moments2D& Moments2D::operator=(const Moments2D& m)
{
  this->cv_moments_ = m.cv_moments_;
  return *this;
}

Moments2D::~Moments2D()
{
}

void 
Moments2D::compute(const Image& image)
{
  CV_FUNCNAME( "Moments2D::compute" );
  __BEGIN__;

  CV_CALL( cvMoments(image.get_ipl_image(), &this->cv_moments_, 0) );

  __END__;

  __ISL_CHECK_ERROR__;
}





Moments1D::Moments1D(const double* vector, size_t vector_size)
{
  this->compute(vector, vector_size);
}

Moments1D::Moments1D(const Moments1D& m)
{
  *this = m;
}

Moments1D& Moments1D::operator=(const Moments1D& m)
{
  this->cv_moments_ = m.cv_moments_;
  return *this;
}

Moments1D::~Moments1D()
{
}

void 
Moments1D::compute(const double* vector, size_t vector_size)
{
  CV_FUNCNAME( "Moments1D::compute" );
  __BEGIN__;

  CvMat stub;
  CV_CALL( cvInitMatHeader(&stub, 1, static_cast<int>(vector_size), CV_64FC1, const_cast<double*>(vector)) );
  CV_CALL( cvMoments(&stub, &this->cv_moments_, 0) );

  __END__;

  __ISL_CHECK_ERROR__;
}



}
