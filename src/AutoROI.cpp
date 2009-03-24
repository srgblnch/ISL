#include <isl/AutoROI.h>
#include "InternalErrorHandler.h"

#include <isl/Image.h>
#include <isl/statistics/Projections.h>
#include <isl/statistics/GaussianFit1D.h>

#if !defined (__ISL_INLINE__)
# include <isl/AutoROI.i>
#endif

namespace isl {

AutoROI::AutoROI()
{
}

AutoROI::AutoROI(const Image& image, double magnification_factor_x, double magnification_factor_y)
{
  this->compute(image, magnification_factor_x, magnification_factor_y);
}

AutoROI::~AutoROI()
{
}

void
AutoROI::compute(const Image& image, double magnification_factor_x, double magnification_factor_y)
{
  CV_FUNCNAME( "AutoROI::compute" );
  __BEGIN__;

  CvRect& roi = this->params_;

  Projections projections(image);

  GaussianFit1D gaussian_fit;
  
  {
    ISL_CALL( gaussian_fit.compute(projections.get_x_projection(),
                                   projections.size_x()) );
    
    if ( !gaussian_fit.has_converged() )
      CV_ERROR(CV_StsNoConv, "Gaussian Fit of X profile did not converge");

    double m = gaussian_fit.mean();
    double s = gaussian_fit.standard_deviation();

    roi.x = cvFloor(m - s * magnification_factor_x);
    roi.width = cvCeil(2 * s * magnification_factor_x);
  }

  {
    ISL_CALL( gaussian_fit.compute(projections.get_y_projection(),
                                   projections.size_y()) );
    
    if ( !gaussian_fit.has_converged() )
      CV_ERROR(CV_StsNoConv, "Gaussian Fit of Y profile did not converge");
    
    double m = gaussian_fit.mean();
    double s = gaussian_fit.standard_deviation();

    roi.y = cvFloor(m - s * magnification_factor_y);
    roi.height = cvCeil(2 * s * magnification_factor_y);
  }

  this->intersect( Rectangle(0,0,image.width(),image.height()) );

  __END__;
  __ISL_CHECK_ERROR__;
}

}
