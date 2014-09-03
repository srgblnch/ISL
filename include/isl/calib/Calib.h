//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#ifndef __INCLUDE_CALIB_H__
#define __INCLUDE_CALIB_H__

#include <isl/ISLConfig.h>
#include <isl/geometry/Point.h>

namespace isl {

class Image;
class Engine;
class PatternAnalyzer;
template class ISL_DECL Point2D<int>;

/**
 *  \addtogroup calib Camera Calibration
 *  @{
 */


/**
 *  \brief Camera Calibration class
 *
 *  This is the main class for camera calibration which drives the different 
 *  algorithms implemented in the library.
 *
 *  For the moment, only perspective calibration is possible, for which the 
 *  transformation model is an homography.
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL Calib
{
  public:
    
    class ISL_DECL Config
    {
      public:


        Config();
        ~Config();

        /**
         *  The spacing between two adjacent calibration points along the X axis
         */
        double x_spacing;
        
        /**
         *  The spacing between two adjacent calibration points along the Y axis
         */
        double y_spacing;

        /**
         *  The calibration engine (\see calib_type)
         */
        int calib_type;

        /**
         *  The size of the smoothing kernel
         */
        int smoothing_kernel_width;
        
        /**
         *  The proportion of the total size of the image which is taken to 
         *  estimate the threshold level (\see Image::adaptive_threshold)
         */
        double adthresh_window_size_factor;
        
        /**
         *  The offset substrated from the mean value 
         */
        double adthresh_offset;

        /**
         *  The size of the structuring element when dilating the thresholded image
         */
        int dilatation_width;
        
        /**
         *  The shape of the structuring element when dilating the thresholded image
         */
        int dilatation_shape;

        /**
         *  Four points in the image delimitating a region of interest (ROI).
         *  Only the blobs found inside this ROI will be considered
         */
        Point2D<int> roi [4];
        
        /**
         *  The maximal deviation of a blob area from the median area value
         *  of all blobs (this is to remove small blobs and too big blobs)
         */
        double median_area_max_deviation;
        
        /**
         *  The method to estimate a first rough location of the centroid
         *  only from the blob info (which is a binarized information)
         *
         *  \see target_extraction. only ISL_BOUNDING_RECT_CENTER
         *  and ISL_ELLIPSE_CENTER are supported.
         */
        int centroid_rough_extraction;

        /**
         *  The method to estimate a more precise location of the centroids
         *  with sub-pixel accuracy
         */
        int refinement_method;
    };




    /**
     *  \brief Constructor
     */
    Calib(void);
    
    /**
     *  \brief Destructor
     */
    ~Calib();

    /**
     *  \brief Calibrate a system from an image of calibration pattern
     *  \param[in] image the input image (must be of integer type)
     *  \exception Exception
     */
    void
      learn(const Image& image);

    /**
     *  \brief Warps the distorsion in an image after having learned a calibration grid
     *  \param[in] src the input image
     *  \param[in] dst the destination image
     *  \param[in] interp the interpolation method (see \ref interp_type)
     *  \exception Exception
     */
    void
      undistort(const Image& src, Image& dst, int interp);

    /**
     *  \brief Accessor for the magnification factor
     *
     *  It is expressed in the same units than the one given in Calib::learn,
     *  and gives the number of units per pixel in the destination image
     */
    double
      x_mag_factor() const;

    /**
     *  \brief Accessor for the magnification factor
     *
     *  It is expressed in the same units than the one given in Calib::learn,
     *  and gives the number of units per pixel in the destination image
     */
    double
      y_mag_factor() const;

    /**
     *  \brief Accessor for the mean error of the calibration points
     *
     *  This value describes how well the model used for calibration fits to
     *  the data on which this model is learned.
     *
     *  It gives the mean deviation in pixel from the model
     */
    double
      model_error() const;

    /**
     *  \brief Draws the error map on an image
     *
     *  This function draws, for each calibration points used to learn the model,
     *  the error vector representing the deviation between the estimated position
     *  of the calibration point (which is the input data used to feed the model)
     *  and the calibration point computed from the model
     *
     *  \param[in] img the image on which the error map is drawn (this should be the
     *                 undistorted calibration pattern)
     *  \param[in] scale the scale factor for the error vector
     *  \param[in] intensity the intensity of the line
     *  \param[in] thickness the thickness of the line
     *  \param[in] line_type the line connectivity (see \ref line_type)
     */
    void
      draw_error_map(Image& img, double scale, double intensity, int thickness = 1, int line_type = ISL_LINETYPE_4) const;

    /**
     *  \brief Draws the detected blobs of the calibration pattern
     *
     *  \param[in] img the image on which the blobs are drawn (this should be the
     *                 source calibration pattern image)
     *  \param[in] intensity the intensity of the line
     *  \param[in] thickness the thickness of the line
     *  \param[in] line_type the line connectivity (see \ref line_type)
     */
    void
      draw_blobs(Image& img, double intensity, int thickness = 1, int line_type = ISL_LINETYPE_4) const;

    /**
     *  \brief Draws the delaunay subdivision obtained from the calibration points
     *
     *  \param[in] img the image on which the blobs are drawn (this should be the
     *                 source calibration pattern image)
     *  \param[in] intensity the intensity of the line
     *  \param[in] thickness the thickness of the line
     *  \param[in] line_type the line connectivity (see \ref line_type)
     */
    void
      draw_delaunay(Image& img, double intensity, int thickness = 1, int line_type = ISL_LINETYPE_4) const;
    
    /**
     *  \brief Accessor for the preprocessed input image (gaussian smoothed, 
     *         histogram-equalized, binarized and dilated)
     */
    const Image&
      preprocessed_image() const;

    /**
     *  \brief The current configuration used
     */
    Config config;

  private:
    PatternAnalyzer* p_;
    Engine* e_;


};
/** @} */


}


#endif
