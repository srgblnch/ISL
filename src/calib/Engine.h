#ifndef __INCLUDE_ENGINE_H__
#define __INCLUDE_ENGINE_H__

#include "PatternAnalyzer.h"

namespace isl {

/**
 *  \addtogroup calib
 *  @{
 */

/**
 *  \addtogroup intern_calib Internal Classes
 *  @{
 */

/**
 *  \brief Base class for a calibration engine
 *
 *  What we call a calibration engine takes in input the 3D coordinates of the
 *  calibration points in the world coordinate system, and the corresponding
 *  2D image coordinates.
 *
 *  Then it solves a model which maps the 2D image coordinates of calibration
 *  points to 'aligned' coordinates, as if the scene was viewed under no perspective
 *  and as if the camera did not introduce any distorsion.
 *
 *  After the model has been solved, it is possible to undistort images with respect
 *  to the solved model, and to get the model parameters, mainly the magnification factor
 *  giving the number of units per pixel.
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class Engine
{
  public:

    /**
     *  \brief Constructor
     */
    Engine(void);

    /**
     *  \brief Destructor
     */
    virtual ~Engine();

    /**
     *  \brief Learns the model from the 2D-3D correspondence of points
     *  \param[in] pa a reference to a PatternAlanlyzer class giving the correspondence 
     *                betwwen real world and image coordinates
     *  \exception Exception
     */
    virtual void
      learn(const PatternAnalyzer& pa) = 0;

    /**
     *  \brief Warps the distorsion in an image after having learned a calibration grid
     *  \param[in] src the input image
     *  \param[in] dst the destination image
     *  \param[in] interp the interpolation method (see \ref interp_type)
     *  \exception Exception
     */
    virtual void
      undistort(const Image& src, Image& dst, int interp) const = 0;

    /**
     *  \brief Draws the error map on an image
     *
     *  This function draws, for each calibration points used to learn the model,
     *  the error vector representing the deviation between the estimated position
     *  of the calibration point (which is the input data used to feed the model)
     *  and the calibration point computed from the model
     *
     *  \param[in] dst the image on which the error map is drawn (this should be the
     *                 undistorted calibration pattern)
     *  \param[in] scale the scale factor for the error vector
     *  \param[in] intensity the intensity of the line
     *  \param[in] thickness the thickness of the line
     *  \param[in] line_type the line connectivity (see \ref line_type)
     */
    virtual void
      draw_error_map(Image& dst, double scale, double intensity, int thickness = 1, int line_type = ISL_LINETYPE_4) const = 0;

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

  protected:
    double x_mag_factor_,
           y_mag_factor_,
           model_error_;
};
/** @} */
/** @} */


}

#if defined (__ISL_INLINE__)
# include "Engine.i"
#endif // __ISL_INLINE__


#endif
