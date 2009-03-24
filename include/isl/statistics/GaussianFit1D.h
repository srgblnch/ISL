#ifndef __INCLUDE_GAUSSIANFIT1D_H__
#define __INCLUDE_GAUSSIANFIT1D_H__

#include <isl/ISLConfig.h>
#include <isl/statistics/LMOptim.h>

namespace isl {


/**
 *  \addtogroup statistics
 *  @{
 */

/**
 *  \brief Fits a curve to a gaussian function with Levenberg marquardt algorithm
 *
 *  Given a buffer of values \f$ f(x) \f$ where \f$ x \f$ is the zero-based index of
 *  the value in the buffer, this class computes the best approximation
 *  in the least square sense, with a gaussian function of the form :
 *
 *  \f[ G(x) = A \, \exp \left( - \frac{(x - x_0)^2}{2 \sigma^2} \right) + b \f]
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL GaussianFit1D : public LMOptim
{
  public:
    /**
     *  \brief Constructor
     */
    GaussianFit1D(void);

    /**
     *  \brief Destructor
     */
    ~GaussianFit1D();

    /**
     *  \brief Accessor for the magnitude \f$ A \f$
     */
    double
      magnitude() const;

    /**
     *  \brief Accessor for the mean \f$ x_0 \f$
     */
    double
      mean() const;

    /**
     *  \brief Accessor for the variance \f$ \sigma^2 \f$
     */
    double
      variance() const;

    /**
     *  \brief Accessor for the standard deviation \f$ \sigma \f$
     */
    double
      standard_deviation() const;

    /**
     *  \brief Accessor for the background \f$ b \f$
     */
    double
      background() const;

    /**
     *  \brief Main function for running the gaussian fit
     *  \param[in] vector the buffer of values corresponding to the distribution
     *  \param[in] vector_size the number of elements pointed to by \a vector
     *  \param[in] magnitude the initial magnitude \f$ A \f$
     *  \param[in] mean the initial mean \f$ x_0 \f$
     *  \param[in] covariance the initial covariance \f$ \sigma^2 \f$
     *  \param[in] background the initial background \f$ b \f$
     *  \exception Exception
     */
    void
      compute(const double* vector,
              size_t vector_size,
              double magnitude,
              double mean,
              double covariance,
              double background);
              
    /**
     *  \brief Helper function for running the gaussian fit.
     *
     *  This function estimates initial parameters itself
     *
     *  \param[in] vector the buffer of values corresponding to the distribution
     *  \param[in] vector_size the number of elements pointed to by \a vector
     *  \exception Exception
     */
    void
      compute(const double* vector,
              size_t vector_size);

    /**
     *  \brief Helper function for running the gaussian fit.
     *
     *  This function estimates initial parameters itself
     *
     *  \param[in] vector the buffer of values corresponding to the distribution
     *  \param[in] vector_size the number of elements pointed to by \a vector
     *  \param[in] fixed_bg_value the background noise value
     *  \exception Exception
     */
    void
      compute_fixed_bg(const double* vector,
                       size_t vector_size,
                       double fixed_bg_value);

    /**
     *  \brief Gives the value of the gaussian evaluated at a given index
     *  \param[in] idx the 0-based index
     *  \exception Exception
     */
    double
      get_fitted_value(int idx);
      
    /**
     *  \brief Gives the value of the fit error evaluated at a given index
     *  \param[in] idx the 0-based index
     *  \exception Exception
     */
    double
      get_fitted_error(int idx);

  protected:
    virtual void 
      errfunc(CvMat* params, CvMat* err_func) const;
  
    virtual void
      errfunc_and_jac(CvMat* params, CvMat* err_func, CvMat* jac) const;
  
    virtual void
      jac(CvMat* params, CvMat* jac) const;

  private:

    void release();

    void initial_guess(const double* vector,
                       size_t vector_size,
                       double& magnitude,
                       double& mean,
                       double& covariance,
                       double& background);

    CvMat* x_;
    CvMat  observ_;

    bool   fixed_bg;
    double fixed_bg_value;

};
/** @} */

}

#if defined (__ISL_INLINE__)
# include <isl/statistics/GaussianFit1D.i>
#endif // __ISL_INLINE__


#endif
