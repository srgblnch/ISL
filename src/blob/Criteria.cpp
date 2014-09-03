//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
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
