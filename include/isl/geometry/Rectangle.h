//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#ifndef __INCLUDE_RECTANGLE_H__
#define __INCLUDE_RECTANGLE_H__

#include <isl/ISLConfig.h>
#include <isl/geometry/Point.h>

namespace isl {

/**
 *  \addtogroup geometry
 *  @{
 */

/**
 *  \brief Class used to describe a rectangle with axises parallel
 *         to X and Y axises, and integer coordinates
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL Rectangle
{
  friend class BlobBoundingRect;

  public:
    /**
     *  \brief Constructor
     */
    Rectangle();
    
    /**
     *  \brief Constructor
     */
    Rectangle(int x, int y, int width, int height);

    /**
     *  \brief Destructor
     */
    ~Rectangle();

    /**
     *  \brief Accessor for the origin
     */
    Point2D<int>
      origin() const;

    /**
     *  \brief Accessor for the x origin
     */
    int
      x() const;

    /**
     *  \brief Accessor for the y origin
     */
    int
      y() const;

    /**
     *  \brief Accessor for the width in pixel
     */
    int
      width() const;

    /**
     *  \brief Accessor for the height in pixel
     */
    int
      height() const;

    /**
     *  \brief Accessor for the center of the rectangle
     */
    Point2D<double>
      center() const;

    /**
     *  \brief Tells if the rectangle has an empty area
     */
    bool is_empty() const;

    /**
     *  \brief Translate the rectangle by a specified offset
     */
    Rectangle& translate( int x0, int y0 );

    /**
     *  \brief Returns the current rectangle translated by a specified offset
     */
    Rectangle translation( int x0, int y0 ) const;

    /**
     *  \brief Intersects the rectangle with another
     *
     *  Clip the rectangle with another
     *
     *  \returns *this
     */
    Rectangle& intersect( const Rectangle& other );

    /**
     *  \brief Returns the intersected rectangle between 'this' and another
     *
     *  Returns the intersected rectangle if intersection is not empty,
     *  otherwise, return Rectangle(0,0,0,0)
     */
    Rectangle intersection( const Rectangle& other ) const;

  protected:
    CvRect params_;
};

/** @} */

}

#if defined (__ISL_INLINE__)
# include <isl/geometry/Rectangle.i>
#endif // __ISL_INLINE__


#endif
