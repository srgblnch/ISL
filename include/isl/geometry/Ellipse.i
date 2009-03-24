namespace isl {

ISL_INLINE
double  
Ellipse::major_axis(void) const
{
  return(this->params_.size.width);
}

ISL_INLINE
double  
Ellipse::minor_axis(void) const
{
  return(this->params_.size.height);
}

ISL_INLINE
double  
Ellipse::tilt(void) const
{
  return(this->params_.angle);
}

ISL_INLINE
Point2D<double>  
Ellipse::center() const
{
  return(Point2D<double>(this->params_.center.x, this->params_.center.y));
}





}
