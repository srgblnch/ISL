//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#ifndef __INCLUDE_ACCURATECENTROID_H__
#define __INCLUDE_ACCURATECENTROID_H__

#include <isl/ISLConfig.h>
#include <isl/geometry/Point.h>

namespace isl {

const int    ISL_ACC_CENTR_KSIZE  = 31;
const double ISL_ACC_CENTR_KSIGMA = 3;

class Image;

/**
 *  \addtogroup statistics
 *  @{
 */

/**
 *  \brief Finds the centroid of a circular dot in an image with sub-pixel accuracy
 *         according to the method described in Reg G. Willson's thesis
 *
 *  This method is inspired from Appendix C (Sub-Pixel Centroid Measurement) 
 *  of Reg G. Willson's thesis 'Modeling and Calibration of Automated Zoom Lenses'.
 *  We use here only the case when blur width is less than the width of the
 *  dot's cross section, which means the method is based on zero-crossings of the
 *  second derivative of the dot's cross section.
 *  Please refer to the thesis to have all the details of the method.
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL AccurateCentroid
{
  public:
    /**
     *  \brief Constructor
     */
    AccurateCentroid(void);
    
    /**
     *  \brief Destructor
     */
    ~AccurateCentroid();

    /**
     *  \brief Main function for running the algorithm
     *  \param[in] image the input image to process
     *  \returns the coordinate of the estimated centroid inside the image
     *  \warning The input image should contain only one dot
     *
     *  \exception Exception
     */
    Point2D<double>
      compute(const Image& image);

  private:
    typedef struct _Line2D
    {
      double vx, vy, x0, y0;
    } Line2D;


    /**
     *  \brief Finds the centroid of each row of the input image and fits a line
     *         to them
     *
     *  \exception Exception
     */
    void
    process_rows(IplImage*, Line2D&);

    /**
     *  \brief Finds the centroid of each columns of the input image and fits a line
     *         to them
     *
     *  \exception Exception
     */
    void
    process_cols(IplImage*, Line2D&);

    /**
     *  \brief Init the gaussian kernels used to filter the image
     *
     *  \exception Exception
     */
    void
    init_kernels();

    float smooth_filt_buf_ [3];
    float gaussian_derivative_buf_[ISL_ACC_CENTR_KSIZE];
    float gaussian_laplacian_buf_ [ISL_ACC_CENTR_KSIZE];
    CvMat smooth_filt_;
    CvMat gaussian_derived_;
    CvMat gaussian_laplacian_;
};
/** @} */

}

#endif
