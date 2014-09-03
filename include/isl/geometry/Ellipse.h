//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#ifndef __INCLUDE_ELLIPSE_H__
#define __INCLUDE_ELLIPSE_H__

#include <isl/ISLConfig.h>
#include <isl/geometry/Point.h>

namespace isl {

class Image;
/**
 *  \addtogroup geometry
 *  @{
 */

/**
 *  \brief Class used to hold an ellipse's parameters and draw it on an image
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL Ellipse
{
  friend class BeamBox;
  friend class BlobEllipse;

  public:
    /**
     *  \brief Constructor
     */
    Ellipse();

    /**
     *  \brief Destructor
     */
    ~Ellipse();

    /**
     *  \brief Accessor for major axis
     */
    double
      major_axis(void) const;

    /**
     *  \brief Accessor for minor axis
     */
    double
      minor_axis(void) const;

    /**
     *  \brief Accessor for ellipse tilt (angle in radians between major axis and X-axis)
     */
    double
      tilt(void) const;

    /**
     *  \brief Accessor for the ellipse center
     */
    Point2D<double>
      center() const;

    /**
     *  \brief Draws the ellipse on an image
     *  \param[in] img the image on which the ellipse is drawn
     *  \param[in] intensity the intensity of the ellipse
     *  \param[in] thickness the line thickness
     *  \param[in] line_type the line type (see \ref line_type)
     *  \exception Exception
     */
    void
      draw(Image& img, double intensity = 255.0f, int thickness = 1, int line_type = ISL_LINETYPE_4) const;

    /**
     *  \brief Draws a cross representing the ellipse center
     *  \param[in] img the image on which the center is drawn
     *  \param[in] nb_pixels the half-size of the cross in pixel
     *  \param[in] intensity the intensity of the ellipse
     *  \param[in] thickness the line thickness
     *  \param[in] line_type the line type (see \ref line_type)
     *  \exception Exception
     */
    void
      draw_center(Image& img, int nb_pixels, double intensity = 0.0f, int thickness = 1, int line_type = ISL_LINETYPE_4) const;

  private:
    CvBox2D params_;
};

/** @} */



}

#if defined (__ISL_INLINE__)
# include <isl/geometry/Ellipse.i>
#endif // __ISL_INLINE__


#endif
