//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#ifndef __INCLUDE_PRINCIPALAXIS_H__
#define __INCLUDE_PRINCIPALAXIS_H__

#include <isl/ISLConfig.h>

namespace isl {

/**
 *  \addtogroup statistics
 *  @{
 */

/**
 *  \brief Class computing the principal axis of an image, a scatter of point
 *         or anything that can be represented by its moments
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL PrincipalAxis
{

  public:
    /**
     *  \brief Constructor
     *
     *  \exception Exception
     */
    PrincipalAxis(double mx, double my, double cxx, double cxy, double cyy);
    
    /**
     *  \brief Destructor
     */
    ~PrincipalAxis(void);

    /**
     *  \brief Computes the principal axis from moments
     *  \param[in] mx the 1st order moment in the X direction
     *  \param[in] my the 1st order moment in the Y direction
     *  \param[in] cxx the 2nd order moment in the X direction
     *  \param[in] cxy the 2nd order moment (cross correlation in X and Y directions)
     *  \param[in] cyy the 2nd order moment in the Y direction
     *  \exception Exception
     */
    void
      compute(double mx, double my, double cxx, double cxy, double cyy);

    /**
     *  \brief Accessor for the first principal axis
     */
    void
      get_first(double &x, double &y, double &std_dev) const;

    /**
     *  \brief Accessor for the second principal axis (orthogonal to the first one, oriented in the trigonometric direction)
     */
    void
      get_second(double &x, double &y, double &std_dev) const;

    /**
     *  \brief Accessor for the center [mx, my]
     */
    void
      get_center(double &x, double &y) const;

    /**
     *  \brief Accessor for the angle between the X direction and the first principal axis
     */
    double
      get_angle(void) const;

  private:
    void
      release(void);

    CvMat* covariance_;
    CvMat* eigen_vectors_;
    CvMat* eigen_values_;
    CvPoint2D64f center_;
    double angle_;
};
/** @} */

}

#if defined (__ISL_INLINE__)
# include <isl/statistics/PrincipalAxis.i>
#endif // __ISL_INLINE__


#endif
