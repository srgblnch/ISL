namespace isl {

ISL_INLINE
double  
GaussianFit2D::magnitude() const
{
  return(cvmGet(this->result_, 0, 0));
}


ISL_INLINE
Point2D<double>  
GaussianFit2D::mean() const
{
  double x = cvmGet(this->result_, 1, 0);
  double y = cvmGet(this->result_, 2, 0);
  return( Point2D<double>(x,y) );
}


ISL_INLINE
void  
GaussianFit2D::covariance(double& c_xx, double& c_xy, double& c_yy) const
{
  c_xx = cvmGet(this->result_, 3, 0);
  c_xy = cvmGet(this->result_, 4, 0);
  c_yy = cvmGet(this->result_, 5, 0);
}


ISL_INLINE
double  
GaussianFit2D::background() const
{
  return(cvmGet(this->result_, 6, 0));
}

}
