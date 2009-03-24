namespace isl {


ISL_INLINE
Point2D<int> 
Rectangle::origin() const
{
  int x = this->params_.x;
  int y = this->params_.y;
  return( Point2D<int>(x, y) );
}

ISL_INLINE
int
Rectangle::x() const
{
  return( this->params_.x );
}


ISL_INLINE
int
Rectangle::y() const
{
  return( this->params_.y );
}

ISL_INLINE
int
Rectangle::width() const
{
  return( this->params_.width );
}


ISL_INLINE
int
Rectangle::height() const
{
  return( this->params_.height );
}


ISL_INLINE
Point2D<double> 
Rectangle::center() const
{
  Point2D<double> p(this->origin().x(), this->origin().y());
  p.x() += 0.5 * this->width();
  p.y() += 0.5 * this->height();
  return( p );
}

ISL_INLINE
bool
Rectangle::is_empty() const
{
  return width() == 0 || height() == 0;
}

}
