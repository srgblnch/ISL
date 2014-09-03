//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#ifndef __INCLUDE_AUTO_ROI_H__
#define __INCLUDE_AUTO_ROI_H__

#include <isl/geometry/Rectangle.h>

namespace isl {

class Image;
class Blob;

/**
 *  \brief Class which extracts the main blob from an image
 *         and approximate the contour by an ellipse
 *
 *  The BeamBox class can process a 8-bit unsigned image where non-zero
 *  pixels are treated as 1's.
 *  It finds the blobs present in the image, keeps only blobs with more than
 *  10 points describing the contour of the blob, then find the one with greater area,
 *  which is considered to be the beam.
 *  The bounding rectangle of the blob is computed, and its ellipse approximation
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL AutoROI : public Rectangle
{
  public:

    /**
     *  \brief Constructor
     */
    AutoROI();
    
    /**
     *  \brief Constructor
     */
    AutoROI(const Image& image, double magnification_factor_x, double magnification_factor_y);
    
    /**
     *  \brief Destructor
     */
    ~AutoROI();

  private:
    /**
     *  \brief Computes an automatic ROI on the image
     *  \param[in] image the input image
     *  \param[in] magnification_factor the factor by which the bounding rectangle is scaled
     *  \exception Exception
     *  \returns a boolean indicating if the beam box was found
     *  \warning The image must be 8-bit unsigned and already preprocessed (binarized)
     */
    void
      compute(const Image& image, double magnification_factor_x, double magnification_factor_y);
};


}

#if defined (__ISL_INLINE__)
# include <isl/AutoROI.i>
#endif // __ISL_INLINE__


#endif
