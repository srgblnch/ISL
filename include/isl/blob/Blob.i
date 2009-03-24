namespace isl {

ISL_INLINE
const double& 
Blob::perimeter(void)
{
  return(this->perimeter_.get());
}

ISL_INLINE
const double&  
Blob::area(void)
{
  return(this->area_.get());
}


ISL_INLINE
const Rectangle&  
Blob::bounding_rect(void)
{
  return(this->bounding_rect_.get());
}

ISL_INLINE
const Ellipse&  
Blob::ellipse(void)
{
  return(this->ellipse_.get());
}


}
