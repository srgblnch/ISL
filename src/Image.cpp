#include <isl/Image.h>
#include "InternalErrorHandler.h"
#include <isl/geometry/Rectangle.h>

#if !defined (__ISL_INLINE__)
# include <isl/Image.i>
#endif

namespace isl {

Image::Image(int width, int height, int st)
: cv_image_(NULL)
{
  this->create(width, height, st);
}

Image::Image(const char* filename)
: cv_image_(NULL)
{
  this->load(filename);
}


Image::Image(const Image& img)
: cv_image_(NULL)
{
  this->copy(*img.cv_image_);
}

Image::~Image()
{
  this->release();
}

Image&
Image::operator = (const Image& img)
{
  CV_FUNCNAME( "Image::operator =" );

  __BEGIN__;

  if (this == &img)
    EXIT;
  
  ISL_CALL( this->copy(*img.cv_image_) );

  __END__;
  __ISL_CHECK_ERROR__;
  return(*this);
}

Image&
Image::operator += (const Image& img)
{
  CV_FUNCNAME( "Image::operator +=" );
  __BEGIN__;

  CV_CALL( cvAdd(this->cv_image_, img.cv_image_, this->cv_image_) );

  __END__;
  __ISL_CHECK_ERROR__;
  return(*this);
}

Image&
Image::operator -= (const Image& img)
{
  CV_FUNCNAME( "Image::operator -=" );
  __BEGIN__;

  CV_CALL( cvSub(this->cv_image_, img.cv_image_, this->cv_image_) );

  __END__;
  __ISL_CHECK_ERROR__;
  return(*this);
}

Image&
Image::operator += (double val)
{
  CV_FUNCNAME( "Image::operator +=" );
  __BEGIN__;

  CV_CALL( cvAddS(this->cv_image_, cvScalar(val), this->cv_image_) );

  __END__;
  __ISL_CHECK_ERROR__;
  return(*this);
}

Image&
Image::operator -= (double val)
{
  CV_FUNCNAME( "Image::operator -=" );
  __BEGIN__;

  CV_CALL( cvSubS(this->cv_image_, cvScalar(val), this->cv_image_) );

  __END__;
  __ISL_CHECK_ERROR__;
  return(*this);
}

Image&
Image::set_zero()
{
  CV_FUNCNAME( "Image::set_zero" );
  __BEGIN__;

  CV_CALL( cvSetZero(this->cv_image_) );

  __END__;
  __ISL_CHECK_ERROR__;
  return(*this);
}

Image&
Image::set(double value)
{
  CV_FUNCNAME( "Image::set" );
  __BEGIN__;

  CV_CALL( cvSet(this->cv_image_, cvScalar(value)) );

  __END__;
  __ISL_CHECK_ERROR__;
  return(*this);
}

Image&
Image::power(double p)
{
  CV_FUNCNAME( "Image::power" );
  __BEGIN__;

  CV_CALL( cvPow(this->cv_image_, this->cv_image_, p) );

  __END__;
  __ISL_CHECK_ERROR__;
  return(*this);
}

Image&
Image::gamma_correction(double gamma, double max_pixel_value)
{
  CV_FUNCNAME( "Image::gamma_correction" );
  __BEGIN__;

  if (gamma != 1.0f)
  {
    CV_CALL( cvScale(this->cv_image_, this->cv_image_, 1 / max_pixel_value) );
    CV_CALL( cvPow  (this->cv_image_, this->cv_image_, 1 / gamma) );
    CV_CALL( cvScale(this->cv_image_, this->cv_image_, max_pixel_value) );
  }

  __END__;
  __ISL_CHECK_ERROR__;
  return(*this);
}

void
Image::min_max_location(double& min_value, double& max_value, Point2D<int>& min_loc, Point2D<int>& max_loc) const
{
  CV_FUNCNAME( "Image::min_max_location" );
  __BEGIN__;

  CV_CALL( cvMinMaxLoc( this->cv_image_,
                        &min_value,
                        &max_value,
                        reinterpret_cast<CvPoint*>(&min_loc),
                        reinterpret_cast<CvPoint*>(&max_loc) ) );

  __END__;
  __ISL_CHECK_ERROR__;
}

double
Image::estimate_background(int border_size)
{
  double background = 0;

  CV_FUNCNAME( "Image::estimate_background" );
  __BEGIN__;

  if (this->height() > 2 * border_size && this->width() > 2 * border_size)
  {
    double mean_band[4];
    CvMat band;
    CvScalar s;

    //- upper band
    {
      CV_CALL( cvGetSubRect(this->cv_image_, 
                            &band,
                            cvRect(0,
                                   0, 
                                   this->width(), 
                                   border_size)) );
      CV_CALL( s = cvAvg(&band) );
      mean_band[0] = s.val[0];
    }

    //- lower band
    {
      CV_CALL( cvGetSubRect(this->cv_image_, 
                            &band,
                            cvRect(0,
                                   this->height() - 1 - border_size,
                                   this->width(),
                                   border_size)) );
      CV_CALL( s = cvAvg(&band) );
      mean_band[1] = s.val[0];
    }
    
    //- left band
    {
      CV_CALL( cvGetSubRect(this->cv_image_, 
                            &band,
                            cvRect(0,
                                   0,
                                   border_size,
                                   this->height())) );
      CV_CALL( s = cvAvg(&band) );
      mean_band[2] = s.val[0];
    }
    
    //- right band
    {
      CV_CALL( cvGetSubRect(this->cv_image_, 
                            &band,
                            cvRect(this->width() - 1 - border_size,
                                   0,
                                   border_size,
                                   this->height())) );
      CV_CALL( s = cvAvg(&band) );
      mean_band[3] = s.val[0];
    }

    //background = ( mean_band[0] + mean_band[1] + mean_band[2] + mean_band[3] ) / 4;
    background = MIN(  MIN(mean_band[0],mean_band[1]) , MIN(mean_band[2],mean_band[3]) );
  }
  else
  {
    //- take the minimal pixel value in the image
    CV_CALL( cvMinMaxLoc(this->cv_image_, &background, 0) );
  }

  __END__;
  __ISL_CHECK_ERROR__;

  return background;
}

Image&
Image::rotate_flip(int operation)
{
  IplImage* tmp = 0;

  CV_FUNCNAME( "Image::rotate_flip" );
  __BEGIN__;
 
  switch (operation)
  {
  case ISL_ROTATE_90_FLIP_NONE:
    {
      //- Transpose then Flip H
      if (this->width() == this->height())
      {
        CV_CALL( cvTranspose(this->cv_image_, this->cv_image_) );
        CV_CALL( cvFlip(this->cv_image_, 0, 1) );
      }
      else
      {
        CV_CALL( tmp = cvCreateImage(cvSize(this->height(), this->width()), this->depth(), 1) );
        CV_CALL( cvTranspose(this->cv_image_, tmp) );
        CV_CALL( cvFlip(tmp, 0, 1) );
        ISL_CALL( this->take_ownership(tmp) );
      }
      break;
    }
  case ISL_ROTATE_180_FLIP_NONE:
    {
      //- Flip H + Flip V
      CV_CALL( cvFlip(this->cv_image_, 0, -1) );
      break;
    }
  case ISL_ROTATE_270_FLIP_NONE:
    {
      //- Transpose then Flip V
      if (this->width() == this->height())
      {
        CV_CALL( cvTranspose(this->cv_image_, this->cv_image_) );
        CV_CALL( cvFlip(this->cv_image_) );
      }
      else
      {
        CV_CALL( tmp = cvCreateImage(cvSize(this->height(), this->width()), this->depth(), 1) );
        CV_CALL( cvTranspose(this->cv_image_, tmp) );
        CV_CALL( cvFlip(tmp) );
        ISL_CALL( this->take_ownership(tmp) );
      }
      break;
    }
  case ISL_ROTATE_NONE_FLIP_H:
    {
      //- Flip H
      CV_CALL( cvFlip(this->cv_image_, 0, 1) );
      break;
    }
  case ISL_ROTATE_90_FLIP_H:
    {
      //- Transpose
      if (this->width() == this->height())
      {
        CV_CALL( cvTranspose(this->cv_image_, this->cv_image_) );
      }
      else
      {
        CV_CALL( tmp = cvCreateImage(cvSize(this->height(), this->width()), this->depth(), 1) );
        CV_CALL( cvTranspose(this->cv_image_, tmp) );
        ISL_CALL( this->take_ownership(tmp) );
      }
      break;
    }
  case ISL_ROTATE_180_FLIP_H:
    {
      //- Flip V
      CV_CALL( cvFlip(this->cv_image_) );
      break;
    }
  case ISL_ROTATE_270_FLIP_H:
    {
      //- Transpose then Flip H & V
      if (this->width() == this->height())
      {
        CV_CALL( cvTranspose(this->cv_image_, this->cv_image_) );
        CV_CALL( cvFlip(this->cv_image_, 0, -1) );
      }
      else
      {
        CV_CALL( tmp = cvCreateImage(cvSize(this->height(), this->width()), this->depth(), 1) );
        CV_CALL( cvTranspose(this->cv_image_, tmp) );
        CV_CALL( cvFlip(tmp, 0, -1) );
        ISL_CALL( this->take_ownership(tmp) );
      }
      break;
    }
  case ISL_ROTATE_NONE_FLIP_NONE:
  default:
    break;
  }

  __END__;
  __ISL_CHECK_ERROR__;
  return *this;
}

Image&
Image::threshold(double value, double max_value, int type)
{
  CV_FUNCNAME( "Image::threshold" );
  __BEGIN__;

  // Source array (single-channel, 8-bit of 32-bit floating point).

  //- In-place thresholding
  CV_CALL( cvThreshold(this->cv_image_, this->cv_image_, value, max_value, type) );
  
  __END__;
  __ISL_CHECK_ERROR__;
  return(*this);
}


Image&
Image::adaptive_threshold(double max_value, int method, int type , int block_size, double offset)
{
  CV_FUNCNAME( "Image::adaptive_threshold" );
  __BEGIN__;

  //- In-place thresholding
  CV_CALL( cvAdaptiveThreshold( this->cv_image_,
                                this->cv_image_,
                                max_value,
                                method,
                                type,
                                block_size,
                                offset) );
  
  __END__;
  __ISL_CHECK_ERROR__;

  return(*this);
}


Image&
Image::smooth_gaussian(int kernel_size_x, int kernel_size_y)
{
  CV_FUNCNAME( "Image::smooth_gaussian" );
  __BEGIN__;

  //- In-place smoothing
  CV_CALL( cvSmooth(this->cv_image_, this->cv_image_, CV_GAUSSIAN, kernel_size_x, kernel_size_y) );

  __END__;
  __ISL_CHECK_ERROR__;

  return(*this);
}

Image&
Image::equalize_hist()
{
  CV_FUNCNAME( "Image::equalize_hist" );
  __BEGIN__;

  CV_CALL( cvEqualizeHist(this->cv_image_, this->cv_image_) );

  __END__;
  __ISL_CHECK_ERROR__;

  return(*this);
}

Image&
Image::morphology(int op, int cols, int rows, int anchor_x, int anchor_y, int shape, int iterations)
{
  IplConvKernel* element = 0;
  IplImage* temp = 0;

  CV_FUNCNAME( "Image::morphology" );
  __BEGIN__;

  IplImage* image = this->cv_image_;
  CV_CALL( element = cvCreateStructuringElementEx( cols, rows, anchor_x, anchor_y, shape, 0 ); );

  if (op == ISL_MORPHO_ERODE)
  {
    CV_CALL( cvErode(image, image, element, iterations) );
  }
  else if (op == ISL_MORPHO_DILATE)
  {
    CV_CALL( cvDilate(image, image, element, iterations) );
  }
  else
  {
    if (op == CV_MOP_GRADIENT || op == CV_MOP_TOPHAT || op == CV_MOP_BLACKHAT)
    {
      CV_CALL( temp = cvCreateImage(cvGetSize(image), image->depth, 1) );
    }
    CV_CALL( cvMorphologyEx( image, image, temp, element, op, iterations) );

  }

  __END__;

  cvReleaseStructuringElement(&element);
  cvReleaseImage(&temp);

  __ISL_CHECK_ERROR__;

  return(*this);
}

Image&
Image::normalize(double a, double b, int norm_type)
{
  CV_FUNCNAME( "Image::convert" );
  __BEGIN__;

  CV_CALL( cvNormalize(this->get_ipl_image(), this->get_ipl_image(), a, b, norm_type) );

  __END__;
  __ISL_CHECK_ERROR__;
  return *this;
}



Image&
Image::convert(int type, double scale, double shift)
{
  CV_FUNCNAME( "Image::convert" );
  __BEGIN__;

  IplImage* converted_image = 0;
  CV_CALL( converted_image = cvCreateImage(cvSize(this->width(), this->height()), type, 1) );

  CV_CALL( cvConvertScale(this->cv_image_, converted_image, scale, shift) );

  this->release();
  this->cv_image_ = converted_image;

  __END__;
  __ISL_CHECK_ERROR__;

  return *this;
}


const Image&
Image::convert(Image& dst_image, double scale, double shift) const
{
  CV_FUNCNAME( "Image::convert" );
  __BEGIN__;

  CV_CALL( cvConvertScale(this->cv_image_, dst_image.cv_image_, scale, shift) );

  __END__;
  __ISL_CHECK_ERROR__;
  return *this;
}

Image*
Image::clone() const
{
  Image* cloned = 0;

  CV_FUNCNAME( "Image::clone" );
  __BEGIN__;

  ISL_CALL( cloned = new Image(*this) );

  __END__;
  __ISL_CHECK_ERROR__;
  return(cloned);
}

Image*
Image::get_roi(int& actual_origin_x, int& actual_origin_y, int& actual_width, int& actual_height) const
{
  CV_FUNCNAME( "Image::get_roi" );
  
  Image* roi_image = 0;

  __BEGIN__;

  int origin_x    = actual_origin_x;
  int origin_y    = actual_origin_y;
  int width       = actual_width;
  int height      = actual_height;

  actual_origin_x = 0;
  actual_origin_y = 0;
  actual_width    = 0;
  actual_height   = 0;
  

  //- clip roi to actual image  
  if (origin_x < 0)
  {
    width += origin_x;
    origin_x = 0;
  }
  if (origin_y < 0)
  {
    height += origin_y;
    origin_y = 0;
  }
  if (origin_x + width > this->width())
    width = this->width() - origin_x;
  if (origin_y + height > this->height())
    height = this->height() - origin_y;

  //- get matrix header corresponding to ROI
  CvMat submat;
  CvRect rect;
  CV_CALL( rect = cvRect(origin_x, origin_y, width, height) );
  CV_CALL( cvGetSubRect(this->cv_image_, &submat, rect) );
  
  //- clone the data and store them in a Image class
#if CV_MAJOR_VERSION >= 2
  ISL_CALL( roi_image = new Image(submat.width, submat.height, cvIplDepth(submat.type)) );
#else
  ISL_CALL( roi_image = new Image(submat.width, submat.height, cvCvToIplDepth(submat.type)) );
#endif
  CV_CALL( cvCopy(&submat, roi_image->cv_image_, NULL) );

  //- the ROI has been selected successfully, update the output values
  actual_origin_x = origin_x;
  actual_origin_y = origin_y;
  actual_width    = width;
  actual_height   = height;

  __END__;
  __ISL_CHECK_ERROR__;
  return(roi_image);

}


Image*
Image::get_roi(Rectangle& rect) const
{
  Image* roi_image = 0;

  CV_FUNCNAME( "Image::get_roi" );
  __BEGIN__;

  int actual_origin_x = rect.origin().x();
  int actual_origin_y = rect.origin().y();
  int actual_width    = rect.width();
  int actual_height   = rect.height();

  ISL_CALL( roi_image = this->get_roi(actual_origin_x, actual_origin_y, actual_width, actual_height) );

  rect = Rectangle(actual_origin_x, actual_origin_y, actual_width, actual_height);

  __END__;
  __ISL_CHECK_ERROR__;
  return(roi_image);
}

void
Image::save(const char* filename) const
{
  CV_FUNCNAME( "Image::save" );
  __BEGIN__;

  CV_CALL( cvSaveImage(filename, this->cv_image_) );

  __END__;
  __ISL_CHECK_ERROR__;
}

void
Image::load(const char* filename)
{
  CV_FUNCNAME( "Image::load" );
  __BEGIN__;

  this->release();
  CV_CALL( this->cv_image_ = cvLoadImage(filename, 0) );
  
  if (this->cv_image_ == 0)
  {
    CV_ERROR(CV_StsError, "Image loading failed");
  }

  __END__;
  __ISL_CHECK_ERROR__;
}



void
Image::create(int width, int height, int st)
{
  CV_FUNCNAME( "Image::create" );
  __BEGIN__;

  CV_CALL( this->cv_image_ = cvCreateImage( cvSize(width,height), st, 1 ) );

  __END__;
  __ISL_CHECK_ERROR__;
}

void
Image::release()
{
  cvReleaseImage(&this->cv_image_); // automatically sets the pointer to NULL
}

Image&
Image::copy( const IplImage& img )
{
  CV_FUNCNAME( "Image::copy" );
  __BEGIN__;

  if (&img == this->cv_image_)
    EXIT;

  ISL_CALL( this->release() );
  CV_CALL( this->cv_image_ = cvCloneImage(&img) );
  
  __END__;
  __ISL_CHECK_ERROR__;
  return *this;
}

Image&
Image::take_ownership( IplImage* img )
{
  CV_FUNCNAME( "Image::take_ownership" );
  __BEGIN__;

  if (img == this->cv_image_)
    EXIT;

  ISL_CALL( this->release() );
  this->cv_image_ = img;
  
  __END__;
  __ISL_CHECK_ERROR__;
  return *this;
}

const Image&
Image::serialize(void* output) const
{
  char* cur_output        = static_cast<char*>(output);
  const char* cur_input   = this->cv_image_->imageData;
  const int row_byte_step = this->row_byte_step();
  const size_t count      = (this->bit_per_pixel() >> 3) * this->width();

  for (int j = 0; j < this->height(); j++)
  {
    ::memcpy(cur_output, cur_input, count);
    cur_input  += row_byte_step;
    cur_output += count;
  }
  return *this;
}


Image&
Image::unserialize(const void* input)
{
  char* cur_output        = this->cv_image_->imageData;
  const char* cur_input   = static_cast<const char*>(input);
  const int row_byte_step = this->row_byte_step();
  const size_t count      = (this->bit_per_pixel() >> 3) * this->width();

  for (int j = 0; j < this->height(); j++)
  {
    ::memcpy(cur_output, cur_input, count);
    cur_input  += count;
    cur_output += row_byte_step;
  }
  return *this;
}

}
