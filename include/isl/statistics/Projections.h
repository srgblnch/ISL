//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#ifndef __INCLUDE_PROJECTIONS_H__
#define __INCLUDE_PROJECTIONS_H__

#include <isl/ISLConfig.h>

namespace isl {

class Image;
/**
 *  \addtogroup statistics Statistical Estimation
 *  @{
 */


/**
 *  \brief Class computing the projections of an image, 
 *         and identifying the maximal value and its position
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL Projections
{
  public:
    /**
     *  \brief Constructor
     */
    Projections(const Image& image);

    /**
     *  \brief Destructor
     */
    ~Projections();

    /**
     *  \brief Constructor
     */
    Projections(const Projections& projections);

    /**
     *  \brief Assignment operator
     */
    Projections& operator = (const Projections& projections);


    /**
     *  \brief Computes the profiles
     *  \param[in] image the input image
     *  \exception Exception
     */
    void
      compute(const Image& image);
 
    /**
     *  \brief Accessor for the X profile values
     */
    const double*
      get_x_projection();

    /**
     *  \brief Number of X profile values
     */
    int
      size_x();

    /**
     *  \brief Index of the maximal value in the X profile
     */
    int
      max_pos_x();

    /**
     *  \brief Maximal value in the X profile
     */
    double
      max_val_x();

    /**
     *  \brief Accessor for the Y profile values
     */
    const double*
      get_y_projection();

    /**
     *  \brief Number of Y profile values
     */
    int
      size_y();

    /**
     *  \brief Index of the maximal value in the Y profile
     */
    int
      max_pos_y();

    /**
     *  \brief Maximal value in the Y profile
     */
    double
      max_val_y();

  private:
    void
      clean();

    CvMat* x_projection_;
    int    x_max_pos_;
    double x_max_val_;
    CvMat* y_projection_;
    int    y_max_pos_;
    double y_max_val_;
};
/** @} */

}

#if defined (__ISL_INLINE__)
# include <isl/statistics/Projections.i>
#endif // __ISL_INLINE__

#endif
