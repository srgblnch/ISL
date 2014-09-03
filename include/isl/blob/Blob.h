//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#ifndef __INCLUDE_BLOB_H__
#define __INCLUDE_BLOB_H__

#include <isl/blob/BlobMeasure.h>

namespace isl {

/**
 *  \addtogroup blob Blob Analysis
 *  @{
 */

/**
 *  \brief Class used to describe a blob
 *         (Binary Large OBject : it represents a particle in a binary image)
 *
 *  A blob is internally described only by its contour points (an ordered sequence of
 *  pixel coordinates).
 *  From this description, several characteristics can be calculated : perimeter, area,
 *  ellipse approximation, bounding rectangle.
 *
 *  This class has no default constructor. It is intended to be created by a BlobList
 *  from an Image, then it can be duplicated through the copy constructor.
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL Blob
{
  friend class BlobList;
  friend class BlobPerimeter;
  friend class BlobArea;
  friend class BlobEllipse;
  friend class BlobBoundingRect;
  friend class ContourNbPoint;
  


  public:
    /**
     *  \brief Default constructor
     *
     *  \exception Exception
     */
	  Blob( );

    /**
     *  \brief Copy constructor
     *
     *  \exception Exception
     */
	  Blob( const Blob &src );
    
    /**
     *  \brief Assignment operator
     *
     *  \exception Exception
     */
	  Blob& operator=(const Blob &src );
    
    /**
     *  \brief Destructor
     */
    ~Blob();

    /**
     *  \brief Returns the perimeter of the blob in pixel unit
     *
     *  \exception Exception
     */
    const double&
      perimeter();

    /**
     *  \brief Returns the area of the blob in pixel unit
     *
     *  \exception Exception
     */
    const double& 
      area();

    /**
     *  \brief Returns the blob's bounding rectangle
     *
     *  \exception Exception
     */
    const Rectangle& 
      bounding_rect();

    /**
     *  \brief Returns the approximation of the blob's contour by an ellipse
     *
     *  \exception Exception
     */
    const Ellipse& 
      ellipse();

    /**
     *  \brief Removes from the contour the points that are not inside 
     *         the rectangle described by the parameters
     *
     *  \exception Exception
     */
    void
      filter(int x_min, int y_min, int x_max, int y_max);

    /**
     *  \brief Draws the blob contour on an image
     *
     *  \exception Exception
     */
    void
      draw(Image& im, double intensity, int thickness = 1, int line_type = ISL_LINETYPE_4) const;

  private:
    Blob(const CvSeq* edges);

    void alloc();
    void release();

    void copy_edges(const CvSeq* edges);

	  CvMemStorage* storage_;
	  CvSeq* edges_;

    BlobPerimeter perimeter_;
    BlobArea area_;
    BlobEllipse ellipse_;
    BlobBoundingRect bounding_rect_;
};
/** @} */

}

#if defined (__ISL_INLINE__)
# include <isl/blob/Blob.i>
#endif // __ISL_INLINE__


#endif
