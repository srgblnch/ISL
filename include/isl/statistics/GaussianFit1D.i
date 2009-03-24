namespace isl {

ISL_INLINE
double  
GaussianFit1D::magnitude() const
{
  return(cvmGet(this->result_, 0, 0));
}

ISL_INLINE
double  
GaussianFit1D::mean() const
{
  return(cvmGet(this->result_, 1, 0));
}

ISL_INLINE
double  
GaussianFit1D::variance() const
{
  return(cvmGet(this->result_, 2, 0));
}

ISL_INLINE
double  
GaussianFit1D::standard_deviation() const
{
  return(::sqrt(cvmGet(this->result_, 2, 0)));
}

ISL_INLINE
double  
GaussianFit1D::background() const
{
  return (this->fixed_bg == false) ? cvmGet(this->result_, 3, 0) : this->fixed_bg_value;  
}

}
