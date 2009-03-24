#include <isl/geometry/Rectangle.h>
#include "InternalErrorHandler.h"
#include <isl/Image.h>

#if !defined (__ISL_INLINE__)
# include <isl/geometry/Rectangle.i>
#endif

namespace isl {


Rectangle::Rectangle()
{
  this->params_ = cvRect(0, 0, 0, 0);
}

Rectangle::Rectangle(int x, int y, int width, int height)
{
  this->params_ = cvRect(x, y, width, height);
}

Rectangle::~Rectangle()
{

}

Rectangle Rectangle::translation(int x0, int y0) const
{
  Rectangle ret(*this);
  ret.params_.x += x0;
  ret.params_.y += y0;
  return ret;
}

Rectangle& Rectangle::translate(int x0, int y0)
{
  *this = this->translation( x0, y0 );
  return *this;
}

Rectangle Rectangle::intersection(const Rectangle& rect) const
{
  if (this == &rect)
    return Rectangle(*this);
  
  
  int left = x() > rect.x() ? x() : rect.x();
	int top = y() > rect.y() ? y() : rect.y();
	int lhs = x() + width();
	int rhs = rect.x() + rect.width();
	int right = lhs < rhs ? lhs : rhs;
	lhs = y() + height();
	rhs = rect.y() + rect.height();
	int bottom = lhs < rhs ? lhs : rhs;
	return Rectangle (right < left ? 0 : left,
		                bottom < top ? 0 : top,
		                right < left ? 0 : right - left,
		                bottom < top ? 0 : bottom - top);
}

Rectangle& Rectangle::intersect(const Rectangle& other)
{
  *this = this->intersection( other );
  return *this;
}

}
