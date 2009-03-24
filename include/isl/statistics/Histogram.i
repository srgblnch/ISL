namespace isl {

ISL_INLINE
int
Histogram::nb_bins() const
{
  return this->nb_bins_;
}

ISL_INLINE
float
Histogram::high_thresh() const
{
  return this->cv_hist_->thresh[0][1];
}

ISL_INLINE
const float*
Histogram::bins() const
{
  return cvGetHistValue_1D( this->cv_hist_, 0 );
}

}
