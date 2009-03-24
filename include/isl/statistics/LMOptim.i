namespace isl {

ISL_INLINE
bool  
LMOptim::has_converged() const
{
  return !(this->max_iter_ == this->nb_iter_);
}

ISL_INLINE
void  
LMOptim::nb_iter(int value)
{
  this->max_iter_ = value;
}

ISL_INLINE
int  
LMOptim::nb_iter() const
{
  return(this->nb_iter_);
}

ISL_INLINE
void  
LMOptim::eps_fwd_diff(double value)
{
  this->esp_fwd_dif_ = value;
}

ISL_INLINE
void  
LMOptim::epsilon(double value)
{
  this->eps_bd_ = value;
}

ISL_INLINE
double  
LMOptim::epsilon() const
{
  return(this->eps_);
}

ISL_INLINE
double  
LMOptim::chi2() const
{
  return(this->chi2_);
}

ISL_INLINE
void
LMOptim::parameters_cov(double* covariance_matrix) const
{
  ::memcpy(covariance_matrix,
           this->result_covariance_->data.db,
		   nb_params_ * nb_params_ * sizeof(double));
}

ISL_INLINE
const CvMat*  
LMOptim::result() const
{
  return(this->result_);
}


}
