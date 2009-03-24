#include <isl/blob/Criteria.h>
#include <isl/blob/Blob.h>


#if !defined (__ISL_INLINE__)
# include <isl/blob/Criteria.i>
#endif


namespace isl {

double
Area::operator()(Blob &blob) const
{ 
  return blob.area();
}

double
Perimeter::operator()(Blob &blob) const
{ 
  return blob.perimeter();
}

double
ContourNbPoint::operator()(Blob &blob) const
{ 
  return static_cast<double>(blob.edges_->total);
}

double
MinX::operator()(Blob &blob) const
{ 
  return static_cast<double>(blob.bounding_rect().origin().x());
}

double
MinY::operator()(Blob &blob) const
{ 
  return static_cast<double>(blob.bounding_rect().origin().y());
}

double
MaxX::operator()(Blob &blob) const
{ 
  const Rectangle& rect = blob.bounding_rect();
  return static_cast<double>(rect.origin().x() + rect.width());
}

double
MaxY::operator()(Blob &blob) const
{ 
  const Rectangle& rect = blob.bounding_rect();
  return static_cast<double>(rect.origin().y() + rect.height());
}

}
