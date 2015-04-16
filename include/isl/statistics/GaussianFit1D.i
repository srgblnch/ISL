//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
namespace isl {

ISL_INLINE
double  
GaussianFit1D::magnitude() const
{
  double value = 0;
#ifdef _IBA_ENABLE_SUPER_GAUSSIAN_FLAG_
  if (super_gaussian_fit_enable)
  {
	value = super_Gaussian_Fitter.getMagnitude();
  }
  else
#endif  
  {
	value = cvmGet(this->result_, 0, 0);
  }
  return(value);
}

ISL_INLINE
double  
GaussianFit1D::mean() const
{
  double value = 0;
  
#ifdef _IBA_ENABLE_SUPER_GAUSSIAN_FLAG_  
  if (super_gaussian_fit_enable)
  {
	value = super_Gaussian_Fitter.getCenter();
  }
  else
#endif 
  {
	value = cvmGet(this->result_, 1, 0);
  }
  return(value);
}

ISL_INLINE
double  
GaussianFit1D::variance() const
{
  double value = 0;
  
#ifdef _IBA_ENABLE_SUPER_GAUSSIAN_FLAG_  
  if (super_gaussian_fit_enable)
  {
	value = super_Gaussian_Fitter.getVariance();
  }
  else
#endif  
  {
	value = cvmGet(this->result_, 2, 0);
  }
  return(value);
}

ISL_INLINE
double  
GaussianFit1D::standard_deviation() const
{
  double value = 0;

#ifdef _IBA_ENABLE_SUPER_GAUSSIAN_FLAG_  
  if (super_gaussian_fit_enable)
  {
	value = super_Gaussian_Fitter.getSigma();
  }
  else
#endif  
  {
	value = ::sqrt(cvmGet(this->result_, 2, 0));
  }
  return(value);
}

ISL_INLINE
double  
GaussianFit1D::background() const
{
  return 0;//(this->fixed_bg == false) ? cvmGet(this->result_, 3, 0) : this->fixed_bg_value;  
}

ISL_INLINE
double
GaussianFit1D::order() const
{
  double value = 0;

#ifdef _IBA_ENABLE_SUPER_GAUSSIAN_FLAG_  
  if (super_gaussian_fit_enable)
  {
	value = super_Gaussian_Fitter.getOrder();
  }
  else
#endif  
  {
	value = 2; //gaussian order
  }
  return(value);
}

ISL_INLINE
void
GaussianFit1D::enableSuperGaussianFit(bool enable_value)
{
  super_gaussian_fit_enable = enable_value;
}

ISL_INLINE	
bool
GaussianFit1D::isSuperGaussianFitEnable() const
{
  return super_gaussian_fit_enable;
}

ISL_INLINE
bool
GaussianFit1D::has_converged() const
{
  bool value = 0;
#ifdef _IBA_ENABLE_SUPER_GAUSSIAN_FLAG_  
  if (super_gaussian_fit_enable)
  {
	value = super_Gaussian_Fitter.hasConverged();
  }
  else
#endif  
  {
	value = LMOptim::has_converged();
  }
  return(value);
}
    
ISL_INLINE
void
GaussianFit1D::nb_iter(int value)
{ 
#ifdef _IBA_ENABLE_SUPER_GAUSSIAN_FLAG_   
  if (super_gaussian_fit_enable)
  {
	super_Gaussian_Fitter.setIterationNumber(value);
  }
  else
#endif  
  {
	LMOptim::nb_iter(value);
  }
}

ISL_INLINE
int
GaussianFit1D::nb_iter() const
{
  int value = 0;
#ifdef _IBA_ENABLE_SUPER_GAUSSIAN_FLAG_    
  if (super_gaussian_fit_enable)
  {
	value = super_Gaussian_Fitter.getIterationNumber();
  }
  else
#endif  
  {
	value = LMOptim::nb_iter();
  }
  return(value);
}
    
ISL_INLINE
void
GaussianFit1D::epsilon(double value)
{  
#ifdef _IBA_ENABLE_SUPER_GAUSSIAN_FLAG_  
  if (super_gaussian_fit_enable)
  {
	super_Gaussian_Fitter.setEpsilon(value);
  }
  else
#endif  
  {
    LMOptim::epsilon(value);
  }
}
    
ISL_INLINE
double
GaussianFit1D::epsilon() const
{
  double value = 0;
#ifdef _IBA_ENABLE_SUPER_GAUSSIAN_FLAG_    
  if (super_gaussian_fit_enable)
  {
	value = super_Gaussian_Fitter.getEpsilon();
  }
  else
#endif  
  {
	value = LMOptim::epsilon();
  }
  return(value);
}
    
ISL_INLINE
double
GaussianFit1D::chi2() const
{
  double value = 0;
  
#ifdef _IBA_ENABLE_SUPER_GAUSSIAN_FLAG_    
  if (super_gaussian_fit_enable)
  {
	value = super_Gaussian_Fitter.getChi2();
  }
  else
#endif  
  {
	value = LMOptim::chi2();
  }
  return(value);
}

}