namespace isl {

ISL_INLINE
int  
Image::width() const
{
  return this->cv_image_->width;
}

ISL_INLINE
int  
Image::height() const
{
  return this->cv_image_->height;
}

ISL_INLINE
int  
Image::row_byte_step() const
{
  return this->cv_image_->widthStep;
}

ISL_INLINE
const void*  
Image::data() const
{
  return this->cv_image_->imageData;
}

ISL_INLINE
void*  
Image::data()
{
  return this->cv_image_->imageData;
}

ISL_INLINE
IplImage*  
Image::get_ipl_image() const
{
  return (this->cv_image_);
}

ISL_INLINE
int  
Image::depth() const
{
  return (this->cv_image_->depth);
}

ISL_INLINE
int  
Image::bit_per_pixel() const
{
  return (this->cv_image_->depth & ~IPL_DEPTH_SIGN);
}

ISL_INLINE
double  
Image::value(int row, int col) const
{
  return( cvGetReal2D(this->cv_image_, col, row) );
}

ISL_INLINE
void
Image::min_max(double& min_value, double& max_value) const
{
  this->min_max_location(min_value, max_value, *((Point2D<int>*)0), *((Point2D<int>*)0));
}

} // namespace isl
