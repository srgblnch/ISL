#include <isl/statistics/LMOptim.h>
#include "InternalErrorHandler.h"

#include <iostream>

#if !defined (__ISL_INLINE__)
# include <isl/statistics/LMOptim.i>
#endif


namespace isl {


LMOptim::LMOptim()
: nb_params_(0),
  nb_func_(0),
  init_guess_(0),
  result_(0),
  result_covariance_(0),
  esp_fwd_dif_(1.0E-8),
  max_iter_(100),
  nb_iter_(0),
  eps_bd_(1.0E-6),
  eps_(0),
  chi2_(0)
{
  
}

LMOptim::~LMOptim()
{
  this->release();
}

void LMOptim::release()
{
  cvReleaseMat(&this->result_);
  cvReleaseMat(&this->result_covariance_);
}

void
LMOptim::init(int _nb_functions, CvMat* _init_guess)
{
  this->release();

  //- store the initial guess
  this->init_guess_ = _init_guess;

  //- retrieve the nb of params
  this->nb_params_  = _init_guess->rows;
  this->nb_func_    = _nb_functions;

  //- allocate the result (final parameters) and the result covariance matrix
  this->result_ = cvCreateMat(this->nb_params_, 1, CV_64F);
  this->result_covariance_ = cvCreateMat(this->nb_params_, this->nb_params_, CV_64F);
}

void
LMOptim::run(int _flags)
{
    //- Allocate memory
    CvMat *init_guess = this->init_guess_;
    CvMat *result = this->result_;
    CvMat *vectX = 0;
    CvMat *vectNewX = 0;
    CvMat *errorFunc = 0;
    CvMat *newErrorFunc = 0;
    CvMat *Jac = 0;
    CvMat *delta = 0;
    CvMat *matrJtJ = 0;
    CvMat *matrJtJN = 0;
    CvMat *matrJt = 0;
    CvMat *vectB = 0;
    CvMat *scale = 0;
    int nb_parm = this->nb_params_;
    int nb_func = this->nb_func_;
   
    CV_FUNCNAME( "LMOptim::run" );
    __BEGIN__;

    double valError;
    double valNewError;

    //- test input data
    if( init_guess->cols != 1 )
        CV_ERROR( CV_StsUnmatchedSizes, "Number of column of vector X0 must be 1" );
    
    if( result->cols != 1 || result->rows != nb_parm )
        CV_ERROR( CV_StsUnmatchedSizes, "X and X0 must have the same size" );

    if( this->max_iter_ <= 0  )
        CV_ERROR( CV_StsUnmatchedSizes, "Max number of iteration must be > 0" );

    if( this->eps_bd_ < 0 )
        CV_ERROR( CV_StsUnmatchedSizes, "Epsilon must be >= 0" );

    CV_CALL( vectX        = cvCreateMat(nb_parm, 1,       CV_64F) );
    CV_CALL( vectNewX     = cvCreateMat(nb_parm, 1,       CV_64F) );
    CV_CALL( errorFunc    = cvCreateMat(nb_func, 1,       CV_64F) );
    CV_CALL( newErrorFunc = cvCreateMat(nb_func, 1,       CV_64F) );
    CV_CALL( Jac          = cvCreateMat(nb_func, nb_parm, CV_64F) );
    CV_CALL( delta        = cvCreateMat(nb_parm, 1,       CV_64F) );
    CV_CALL( matrJtJ      = cvCreateMat(nb_parm, nb_parm, CV_64F) );
    CV_CALL( matrJtJN     = cvCreateMat(nb_parm, nb_parm, CV_64F) );
    CV_CALL( matrJt       = cvCreateMat(nb_parm, nb_func, CV_64F) );
    CV_CALL( vectB        = cvCreateMat(nb_parm, 1,       CV_64F) );
    CV_CALL( scale        = cvCreateMat(nb_parm, 1,       CV_64F) );

    //- copy x0 to current value of x
    CV_CALL( cvCopy(init_guess,vectX) );

    //========== Main optimization loop ============
    double change;
    int currIter;
    double alpha;
    int i;
    double x_i;
    double h;
    double eps;
    
    eps = ::sqrt(this->esp_fwd_dif_);

    change = 1;
    currIter = 0;
    alpha = 1.0E-3;

    do {

        if (_flags & ISL_LMOPTIM_FWDDIF)
        {
          //- Compute value of the function
          ISL_CALL( this->errfunc(vectX, errorFunc) );

          //- Compute a forward-difference jacobian
          for (i = 0; i < nb_parm; i++)
          {
            CvMat jac_col;
            CV_CALL( cvGetCol(Jac, &jac_col, i) );
            
            x_i = cvmGet(vectX, i, 0);
            h = eps * fabs(x_i);
            if (h == 0)
              h = eps;

            CV_CALL( cvCopy(vectX, vectNewX) );
            cvmSet( vectNewX, i, 0, x_i + h );

            ISL_CALL( this->errfunc(vectNewX, &jac_col) );

            CV_CALL( cvSub(errorFunc, &jac_col, &jac_col) );

            CV_CALL( cvScale(&jac_col, &jac_col, 1/h) );
          }
        }
        else
        {
          //- Compute value of function and jacobian
          ISL_CALL( this->errfunc_and_jac(vectX, errorFunc, Jac) );
        }

        //- Compute error */
        CV_CALL( valError = cvNorm(errorFunc) );

        //- Normalize the cols of the Jacobian K = J * D
        for (i = 0; i < nb_parm; i++)
        {
          CvMat col;
          double col_norm;

          CV_CALL( cvGetCol(Jac, &col, i) );
          CV_CALL( col_norm = cvNorm(&col) );
          if (fabs(col_norm) <= DBL_EPSILON)
            col_norm = 1;
          CV_CALL( cvScale(&col, &col, 1/col_norm) ); // normalize Jacobian column
          cvmSet(scale, i, 0, 1/col_norm); // save the norm for future un-scaling
        }
        
        //- Define optimal delta for J'*J*delta=J'*error
        
        //- compute J'J */
        CV_CALL( cvMulTransposed(Jac,matrJtJ,1) );
        
        CV_CALL( cvCopy(matrJtJ,matrJtJN) );

        //- compute J'*error */
        CV_CALL( cvTranspose(Jac,matrJt) );
        CV_CALL( cvMatMul(matrJt,errorFunc,vectB) );

        //- Solve normal equation for given alpha and Jacobian
        do
        {
//            std::cout << "alpha = " << alpha << std::endl;
//            std::cout << "iter  = " << currIter << std::endl;

            //- Increase diagonal elements by alpha
            for( int i = 0; i < nb_parm; i++ )
            {
                double val;
                CV_CALL( val = cvmGet(matrJtJ,i,i) );
                cvmSet(matrJtJN,i,i,(1+alpha)*val);
            }

            //- Solve system to define delta
            CV_CALL( cvSolve(matrJtJN,vectB,delta,CV_SVD) );
            
            //- Unscale
            CV_CALL( cvMul(delta, scale, delta) );
            
            //- We know delta and we can define new value of vector X
            CV_CALL( cvAdd(vectX,delta,vectNewX) );

            //- Compute result of function for new vector X
            ISL_CALL( this->errfunc(vectNewX,newErrorFunc) );
            

            CV_CALL( valNewError = cvNorm(newErrorFunc) );

            currIter++;

            if( valNewError < valError )
            {
              //- accept new value
              valError = valNewError;

              //- Compute relative change of required parameter vectorX. change = norm(curr-prev) / norm(curr) )
              CV_CALL( change = cvNorm(vectX, vectNewX, CV_RELATIVE_L2) );

              alpha /= 10;
              CV_CALL( cvCopy(vectNewX,vectX) );
              break;
            }
            else
            {
              alpha *= 10;
            }

        } while ( currIter < this->max_iter_  );
        //- new value of X and alpha were accepted

    } while ( change > this->eps_bd_ && currIter < this->max_iter_ );


    //- result was computed
    CV_CALL( cvCopy(vectX,result) );
    this->nb_iter_ = currIter;
    this->eps_ = change;
    this->chi2_ = valError * valError / (nb_func - nb_parm);

    //- compute J
    CV_CALL( this->jac(this->result_, Jac) );

    //- compute J'J
    CV_CALL( cvMulTransposed(Jac, matrJtJ, 1) );

    //- compute G = (J'J)^-1
    CV_CALL( cvInvert(matrJtJ, this->result_covariance_) );


    __END__;

    cvReleaseMat(&vectX);
    cvReleaseMat(&vectNewX);
    cvReleaseMat(&errorFunc);
    cvReleaseMat(&newErrorFunc);
    cvReleaseMat(&Jac);
    cvReleaseMat(&delta);
    cvReleaseMat(&matrJtJ);
    cvReleaseMat(&matrJtJN);
    cvReleaseMat(&matrJt);
    cvReleaseMat(&vectB);
    cvReleaseMat(&scale);
    
    __ISL_CHECK_ERROR__;
}


void
LMDiffOptim::errfunc_and_jac(CvMat*, CvMat*, CvMat*) const
{
  //- this function is not called, so an empty body is enough
}
  
void
LMDiffOptim::jac(CvMat*, CvMat*) const
{
  //- this function is not called, so an empty body is enough
}

void
LMDiffOptim::run(int _flags)
{
  //- we force the use of the ISL_LMOPTIM_FWDDIF flag
  LMOptim::run(_flags | ISL_LMOPTIM_FWDDIF);
}


}
