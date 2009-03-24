namespace isl {

ISL_INLINE
const double*
Projections::get_x_projection()
{
  return(this->x_projection_->data.db);
}

ISL_INLINE
const double*
Projections::get_y_projection()
{
  return(this->y_projection_->data.db);
}

ISL_INLINE
int
Projections::size_x()
{
  return(this->x_projection_->cols);
}

ISL_INLINE
int
Projections::size_y()
{
  return(this->y_projection_->rows);
}

ISL_INLINE
int
Projections::max_pos_x()
{
  return(this->x_max_pos_);
}

ISL_INLINE
double
Projections::max_val_x()
{
  return(this->x_max_val_);
}

ISL_INLINE
int
Projections::max_pos_y()
{
  return(this->y_max_pos_);
}

ISL_INLINE
double
Projections::max_val_y()
{
  return(this->y_max_val_);
}





}
