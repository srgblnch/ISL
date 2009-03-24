namespace isl {

ISL_INLINE
double
Engine::x_mag_factor() const
{
  return(this->x_mag_factor_);
}

ISL_INLINE
double
Engine::y_mag_factor() const
{
  return(this->y_mag_factor_);
}

ISL_INLINE
double
Engine::model_error() const
{
  return(this->model_error_);
}

}
