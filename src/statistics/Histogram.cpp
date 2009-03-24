#include <isl/statistics/Histogram.h>
#include "InternalErrorHandler.h"
#include <isl/Image.h>

#if !defined (__ISL_INLINE__)
# include <isl/statistics/Histogram.i>
#endif


namespace isl {

Histogram::Histogram(const Image& image, int nb_bins, float low_thresh, float high_thresh)
: cv_hist_(0),
  nb_bins_(0)
{
  this->compute(image, nb_bins, low_thresh, high_thresh);
}

Histogram::~Histogram()
{
  cvReleaseHist(&this->cv_hist_);
}


void 
Histogram::compute(const Image& image, int nb_bins, float low_thresh, float high_thresh)
{
  IplImage* im = 0;
  CV_FUNCNAME( "Histogram::compute" );
  __BEGIN__;

  float  _range[] = {low_thresh, high_thresh};
  float* range = _range;

  CV_CALL( this->cv_hist_ = cvCreateHist(1, &nb_bins, CV_HIST_ARRAY, &range, 1) );

  im = image.get_ipl_image();
  CV_CALL( cvCalcHist(&im, this->cv_hist_, 0, 0) );
  
  this->nb_bins_ = nb_bins; 

  __END__;
  __ISL_CHECK_ERROR__;
}



}
