namespace isl {

ISL_INLINE
const double*
LineProfile::values()
{
  return(this->profile_->data.db);
}

ISL_INLINE
int
LineProfile::size()
{
  return(this->profile_->cols);
}

}
