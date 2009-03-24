#ifndef __INCLUDE_GAUSSIANFIT2D_H__
#define __INCLUDE_GAUSSIANFIT2D_H__

#include <isl/ISLConfig.h>
#include <isl/geometry/Point.h>
#include <isl/statistics/LMOptim.h>

namespace isl {

class Image;

/**
 *  \addtogroup statistics
 *  @{
 */

/**
 *  \brief Fits an image to a 2D Gaussian function with Levenberg-Marquardt algorithm
 *
 *  Given an image \f$ I \f$ this class computes the best approximation of \f$ I(x,y) \f$
 *  in the least square sense, with a 2D gaussian function of the form :
 *
 *  \f[ G(x,y) = A \, \exp \left( - \frac{1}{2} 
 *      \left[ \begin{array}{cc} x - x_c & y - y_c \end{array} \right] 
 *      \Gamma^{-1}
 *      \left[ \begin{array}{c} x - x_c \\ y - y_c \end{array} \right]
 *      \right) + b \f]
 *  where
 *  \li \f$ A \f$ is the magnitude
 *  \li \f$ \left[ \begin{array}{c} x_c \\ y_c \end{array} \right] \f$ is the center
 *  \li \f$ \Gamma = \left[ \begin{array}{cc} \sigma_x^2 & \sigma_{xy} \\ \sigma_{xy} & \sigma_y^2 \end{array} \right] \f$
 *      is a covariance matrix
 *  \li \f$ b \f$ is the background
 *  
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL GaussianFit2D : public LMOptim
{
  public:

    /**
     *  \brief Constructor
     */
    GaussianFit2D(void);

    /**
     *  \brief Destructor
     */
    ~GaussianFit2D();

    /**
     *  \brief Accessor for the magnitude \f$ A \f$
     */
    double
      magnitude() const;

    /**
     *  \brief Accessor for the mean \f$ \left[ \begin{array}{c} x_c \\ y_c \end{array} \right] \f$
     */
    Point2D<double>
      mean() const;

    /**
     *  \brief Accessor for the covariance matrix \f$ \Gamma = \left[ \begin{array}{cc} \sigma_x^2 & \sigma_{xy} \\ \sigma_{xy} & \sigma_y^2 \end{array} \right] \f$
     */
    void
      covariance(double& c_xx, double& c_xy, double& c_yy) const;

    /**
     *  \brief Accessor for the magnitude \f$ b \f$
     */
    double
      background() const;

    
    /**
     *  \brief Main function for running the gaussian fit
     *  \param[in] image the image where the gaussian is to be found
     *  \param[in] magnitude the initial magnitude \f$ A \f$
     *  \param[in] mean_x the initial X position of the mean \f$ x_c \f$
     *  \param[in] mean_y the initial Y position of the mean \f$ y_c \f$
     *  \param[in] c_xx the initial \f$ \sigma_x^2 \f$
     *  \param[in] c_xy the initial \f$ \sigma_{xy} \f$
     *  \param[in] c_yy the initial \f$ \sigma_y^2 \f$
     *  \param[in] background the initial background \f$ b \f$
     *  \exception Exception
     */
    void
      compute(const Image& image,
              double magnitude,
              double mean_x,
              double mean_y,
              double c_xx,
              double c_xy,
              double c_yy,
              double background);

    /**
     *  \brief Main function for running the gaussian fit
     *
     *  This function estimates initial parameters itself
     *
     *  \param[in] image the image where the gaussian is to be found
     *  \exception Exception
     */
    void
      compute(const Image& image);

  protected:
    virtual void 
      errfunc(CvMat* params, CvMat* err_func) const;
  
    virtual void
      errfunc_and_jac(CvMat* params, CvMat* err_func, CvMat* jac) const;
  
    virtual void
      jac(CvMat* params, CvMat* jac) const;

  private:

    void initial_guess( const Image& image,
                        double& magnitude,
                        double& mean_x,
                        double& mean_y,
                        double& c_xx,
                        double& c_xy,
                        double& c_yy,
                        double& background);

    CvMat* x_;
    CvMat* y_;
    CvMat observ_;

    CvMat* xc_;
    CvMat* yc_;
    CvMat* xc2_;
    CvMat* yc2_;
    CvMat* xcyc_;
    CvMat* quad_part_;
    CvMat* exp_part_;
};
/** @} */

}

#if defined (__ISL_INLINE__)
# include <isl/statistics/GaussianFit2D.i>
#endif // __ISL_INLINE__


#endif
