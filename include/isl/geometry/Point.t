namespace isl {

template <typename T>
Point2D<T>::Point2D(const Point2D<T>& _p)
{
  this->x_ = _p.x_;
  this->y_ = _p.y_;
}

template <typename T>
Point2D<T>::Point2D(const T& _x, const T& _y)
{
  this->x_ = _x;
  this->y_ = _y;
}


template <typename T>
Point2D<T>::~Point2D()
{
}

template <typename T>
Point2D<T>& Point2D<T>::operator+=(const Point2D<T>& _p)
{
  this->x_ += _p.x_;
  this->y_ += _p.y_;
  return(*this);
}

template <typename T>
Point2D<T>& Point2D<T>::operator-=(const Point2D<T>& _p)
{
  this->x_ -= _p.x_;
  this->y_ -= _p.y_;
  return(*this);
}

template <typename T>
Point2D<T>& Point2D<T>::operator=(const Point2D<T>& _p)
{
  this->x_ = _p.x_;
  this->y_ = _p.y_;
  return(*this);
}


template <typename T>
double Point2D<T>::operator*(const Point2D<T>& _p) const
{
  return(this->x_ * _p.x_ + this->y_ * _p.y_);
}

template <typename T>
double Point2D<T>::norm_L1() const
{
  return(::fabs(double(this->x_)) + ::fabs(double(this->y_)));
}

template <typename T>
double Point2D<T>::norm_Linf() const
{
  return(MAX(::fabs(double(this->x_)), ::fabs(double(this->y_))));
}

template <typename T>
double Point2D<T>::norm_L2() const
{
  return(::sqrt(double(this->x_ * this->x_ + this->y_ * this->y_)) );
}

template <typename T>
double Point2D<T>::norm_L2_square() const
{
  return( this->x_ * this->x_ + this->y_ * this->y_ );
}

template <typename T>
double Point2D<T>::dist_L1(const Point2D<T>& _p) const
{
  Point2D<T> diff( *this );
  diff -= _p;
  return(diff.norm_L1());
}

template <typename T>
double Point2D<T>::dist_Linf(const Point2D<T>& _p) const
{
  Point2D<T> diff( *this );
  diff -= _p;
  return(diff.norm_Linf());
}

template <typename T>
double Point2D<T>::dist_L2(const Point2D<T>& _p) const
{
  Point2D<T> diff( *this );
  diff -= _p;
  return(diff.norm_L2());
}

template <typename T>
double Point2D<T>::dist_L2_square(const Point2D<T>& _p) const
{
  Point2D<T> diff( *this );
  diff -= _p;
  return(diff.norm_L2_square());
}


template <typename T>
T& Point2D<T>::x()
{
  return(this->x_);
}

template <typename T>
T& Point2D<T>::y()
{
  return(this->y_);
}

template <typename T>
T Point2D<T>::x() const 
{
  return(this->x_);
}

template <typename T>
T Point2D<T>::y() const 
{
  return(this->y_);
}







template <typename T>
Point2D<T>
operator +(const Point2D<T>& _p, const Point2D<T>& _q)
{
  Point2D<T> r(_p);
  return( r += _q );
}

template <typename T>
Point2D<T>
operator -(const Point2D<T>& _p, const Point2D<T>& _q)
{
  Point2D<T> r(_p);
  return( r -= _q );
}

template <typename T>
bool
operator ==(const Point2D<T>& _p, const Point2D<T>& _q)
{
  return( _p.x() == _q.x() && _p.y() == _q.y() );
}

template <typename T>
bool
operator !=(const Point2D<T>& _p, const Point2D<T>& _q)
{
  return( _p.x() != _q.x() || _p.y() != _q.y() );
}










}
