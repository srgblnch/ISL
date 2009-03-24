#ifndef __INCLUDE_LMOPTIM_H__
#define __INCLUDE_LMOPTIM_H__

#include <isl/ISLConfig.h>

namespace isl {

const int ISL_LMOPTIM_FWDDIF    = 0x1; ///< Indicates that the Jacobian must be computed by forward differenciation
const int ISL_LMOPTIM_SCALE_JAC = 0x2; ///< Indicates that the columns of the Jacobian must be normalized internally to avoid ill-conditionned problem

/**
 *  \addtogroup statistics
 *  @{
 */

/**
 *  \brief Base class for Levenberg-Marquardt non-linear least squares minimization problem
 *
 *  This class is intended to be inherited.
 *  It provides the generic algorithm of Levenberg-Marquardt.
 *
 *  Examples of use of this class are GaussianFit1D and GaussianFit2D
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL LMOptim
{
public:

  /**
   *  \brief True if the number of iteration is strictly inferior 
   *         to the maximal number of iteration set at initialization
   */
  bool
    has_converged() const;

  /**
   *  \brief Sets the maximal number of iterations
   */
  void
    nb_iter(int value);

  /**
   *  \brief Gets the effective number of iterations used to converge
   */
  int
    nb_iter() const;

  /**
   *  \brief Sets an input variable used in determining a suitable step length
   *         for the forward-difference approximation
   */
  void
    eps_fwd_diff(double value);

  /**
   *  \brief Sets the maximal relative error in the parameters 
   *         tolerated between two iterations
   */
  void
    epsilon(double value);

  /**
   *  \brief Gets the effective relative error in the parameters 
   *         obtained between the two last iterations
   */
  double
    epsilon() const;

  /**
   *  \brief Gets the \f$ \chi^2 \f$ after convergence
   *
   *  It is defined as : \f[ \chi^2 = \frac{1}{N - m} \sum f_p (x_i)^2 \f]
   *  where :
   *  \li \f$ N \f$ is the number of functions
   *  \li \f$ m \f$ is the number of parameters
   *  \li \f$ f_p \f$ is the error function parameterized by the \f$ m \f$ parameters
   */
  double
    chi2() const;

  /**
   *  \brief Fills a buffer with the covariance matrix of the best fit parameters, 
   *         row after row
   *
   *  This matrix is computed from the Jacobian matrix \f$ J \f$ by 
   *  \f$ \Gamma = (J^t J)^{-1} \f$
   *
   *  \warning No bound array checks are done here, so \a covariance_matrix 
   *           must be able to store \f$ m^2 \f$ values, 
   *           where \f$ m \f$ is the number of parameters
   */
  void
    parameters_cov(double* covariance_matrix) const;

protected:
  /**
   *  \brief Constructor
   */
  LMOptim();
  
  /**
   *  \brief Destructor
   */
  virtual ~LMOptim();


  /**
   *  \brief Computes the error function given a set of parameters
   *
   *  This is a pure virtual functions intended to be implemented by the user
   *
   *  \param[in] params the set of parameters
   *  \param[out] err_func the error function
   */
  virtual void 
    errfunc(CvMat* params, CvMat* err_func) const = 0;

  /**
   *  \brief Computes the error function and the associated Jacobian 
   *         given a set of parameters
   *
   *  This is a pure virtual functions intended to be implemented by the user
   *
   *  \param[in] params the set of parameters
   *  \param[out] err_func the error function
   *  \param[out] jac the Jacobian matrix
   */
  virtual void
    errfunc_and_jac(CvMat* params, CvMat* err_func, CvMat* jac) const = 0;
  
  /**
   *  \brief Computes the Jacobian given a set of parameters
   *
   *  This is a pure virtual functions intended to be implemented by the user
   *
   *  \param[in] params the set of parameters
   *  \param[out] jac the Jacobian matrix
   */
  virtual void
    jac(CvMat* params, CvMat* jac) const = 0;
  
  /**
   *  \brief Initializes the solver by giving the number of functions, 
   *         and the set of initial parameters
   *  \param[in] nb_functions the number of function on which parameters are evaluated
   *  \param[out] init_guess the initial set of parameters
   */
  void
    init(int nb_functions, CvMat* init_guess);

  /**
   *  \brief Runs the solver
   *
   *  The solver supports several modes :
   *  \li if ISL_LMOPTIM_FWDDIF is set, the Jacobian is computed from a 
   *      forward difference approximation, so LMOptim::errfunc_and_jac or LMOptim::jac
   *      do not need to do anything (empty implementation) since they are not called
   *  \li if ISL_LMOPTIM_SCALE_JAC is set, the colums of Jacobian are normalized to avoid
   *      ill-conditionned systems
   *
   *  \param[in] flags a set of flags (see the discussion)
   */
  virtual void
    run(int flags);

  /**
   *  \brief Accessor for the final set of parameters
   */
  const CvMat*
    result() const;


protected:
  void release();


  int nb_params_;
  int nb_func_;
  
  CvMat* init_guess_;
  CvMat* result_;

  CvMat* result_covariance_;

  double esp_fwd_dif_;

  int max_iter_;
  int nb_iter_;
  double eps_bd_;
  double eps_;

  double chi2_;
};


/**
 *  \brief Base class for Levenberg-Marquardt non-linear least squares minimization problem
 *         with forward difference Jacobian
 *
 *  This class is intended to be inherited.
 *  It provides the generic algorithm of Levenberg-Marquardt with forward difference
 *  approximation of the Jacobian
 *
 *  It is derived from LMOptim since this class can handle forward difference Jacobian
 *  Its aim is to simplify the use for the user since it implements empty \a errfunc_and_jac 
 *  and \a jac functions, and it automatically launches the solver with 
 *  the ISL_LMOPTIM_FWDDIF flag
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL LMDiffOptim : public LMOptim
{
protected:

  virtual void
    errfunc_and_jac(CvMat* params, CvMat* err_func, CvMat* jac) const;
  
  virtual void
    jac(CvMat* params, CvMat* jac) const;
  
  virtual void
    run(int flags);
};

/** @} */


}

#if defined (__ISL_INLINE__)
# include <isl/statistics/LMOptim.i>
#endif // __ISL_INLINE__

#endif
