namespace isl {


ISL_INLINE
void
PrincipalAxis::get_first(double &x, double &y, double &std_dev) const
{
  x       = cvmGet(this->eigen_vectors_, 0, 0);
  y       = cvmGet(this->eigen_vectors_, 1, 0);
  std_dev = sqrt(cvmGet(this->eigen_values_,  0, 0));
}


ISL_INLINE
void
PrincipalAxis::get_second(double &x, double &y, double &std_dev) const
{
  x       = cvmGet(this->eigen_vectors_, 0, 1);
  y       = cvmGet(this->eigen_vectors_, 1, 1);
  std_dev = sqrt(cvmGet(this->eigen_values_,  1, 0));
}

ISL_INLINE
void
PrincipalAxis::get_center(double &x, double &y) const
{
  x = this->center_.x;
  y = this->center_.y;
}


ISL_INLINE
double  
PrincipalAxis::get_angle() const
{
  return(angle_);
}

ISL_INLINE
const Axis*
PrincipalAxis::getMajor() const
{
  return major_;
}

ISL_INLINE
const Axis*
PrincipalAxis::getMinor() const
{
  return minor_;
}



}
