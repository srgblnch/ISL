namespace isl {

ISL_INLINE
int
PatternAnalyzer::calibrator_height() const
{
  return(this->calibrator_size_.height);
}

ISL_INLINE
int
PatternAnalyzer::calibrator_width() const
{
  return(this->calibrator_size_.width);
}

ISL_INLINE
int
PatternAnalyzer::nb_pt() const
{
  return(this->world_coord_->rows);
}

ISL_INLINE
const CvMat*
PatternAnalyzer::world_coord() const
{
  return(this->world_coord_);
}

ISL_INLINE
const CvMat*
PatternAnalyzer::image_coord() const
{
  return(this->image_coord_);
}



ISL_INLINE
const Image&
PatternAnalyzer::preprocessed_image() const
{
  return(*this->preproc_image_);
}

ISL_INLINE
BlobList&
PatternAnalyzer::blob_list()
{
  return(this->blob_list_);
}

}
