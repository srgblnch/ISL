#ifndef __INCLUDE_TSAIENGINE_H__
#define __INCLUDE_TSAIENGINE_H__

#include "Engine.h"
#include <isl/statistics/LMOptim.h>

namespace isl {

/**
 *  \addtogroup calib
 *  @{
 */

/**
 *  \addtogroup intern_calib
 *  @{
 */


class CameraParameters
{
  public:
    double    Ncx;  ///< [sel]     Number of sensor elements in camera's x direction
    double    Nfx;  ///< [pix]     Number of pixels in frame grabber's x direction
    double    dx;   ///< [mm/sel]  X dimension of camera's sensor element (in mm)
    double    dy;   ///< [mm/sel]  Y dimension of camera's sensor element (in mm)
    double    dpx;  ///< [mm/pix]  Effective X dimension of pixel in frame grabber
    double    dpy;  ///< [mm/pix]  Effective Y dimension of pixel in frame grabber
    double    Cx;   ///< [pix]     Z axis intercept of camera coordinate system
    double    Cy;   ///< [pix]     Z axis intercept of camera coordinate system
    double    sx;   ///< []        Scale factor to compensate for any error in dpx
};

class CalibResults
{
  public:
    double    f;      ///< [mm]
    double    kappa1; ///< [1/mm^2]
    double    p1;     ///< [1/mm]
    double    p2;     ///< [1/mm]
    double    Tx;     ///< [mm]
    double    Ty;     ///< [mm]
    double    Tz;     ///< [mm]
    double    Rx;     ///< [rad]
    double    Ry;     ///< [rad]
    double    Rz;     ///< [rad]
    double    r1;     ///< []
    double    r2;     ///< []
    double    r3;     ///< []
    double    r4;     ///< []
    double    r5;     ///< []
    double    r6;     ///< []
    double    r7;     ///< []
    double    r8;     ///< []
    double    r9;     ///< []
    double    Cx;     ///< [pix]
    double    Cy;     ///< [pix]
};


/**
 *  \brief Models and corrects the image with Tsai's calibration model
 *
 *  The code of this class is highly inspired by the library of Reg G. Willson, 
 *  originally written in C. It is just a re-write in C++ using OpenCV for matrix 
 *  operations, and using the ISL class LMDiffOptim in place of the LMDIFF Fortran
 *  package used for Levenberg-Marquardt optimization.
 *  
 *  Please refer to this library to get information about the algorithm.
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class TsaiEngine : public Engine
{
  friend class Optimizer_F_Tz_Kappa;
  friend class Optimizer_5Par_LateDistorRemov;
  friend class Optimizer_5Par_EarlyDistorRemov;
  friend class Optimizer_NonIC;
  friend class Optimizer_Full;

  public:

    TsaiEngine(void);
    virtual ~TsaiEngine();

    virtual void
      learn(const PatternAnalyzer& pa);

    virtual void
      undistort(const Image& src, Image& dst, int interp) const;

    void
      draw_error_map(Image& dst, double scale, double intensity, int thickness = 1, int line_type = ISL_LINETYPE_4) const;

    const CalibResults&
      result() const;


  private:
    
    void
      three_parm_optimization();
    
    void
      five_parm_optimization_with_late_distortion_removal();
    
    void
      five_parm_optimization_with_early_distortion_removal();

    void
      nic_optimization();

    void
      full_optimization();




    void
      compute_Xd_Yd_and_r_squared();

    void
      compute_U();

    void
      compute_Tx_and_Ty();

    void
      compute_R();

    void
      compute_approximate_f_and_Tz();

    void
      compute_exact_f_and_Tz();


    void
      solve_RPY_transform ();

    void
      apply_RPY_transform ();


    void
      remove_Xd_Yd_sensor_plane_distorsion();


    const PatternAnalyzer*  p_c_;
    CameraParameters        cam_p_;
    CalibResults            result_;

    int nb_pt_;

    CvMat* distorted_coord_;
    CvMat* r_distorted_squared_;
    CvMat* U_;
};


//- Optimizer_F_Tz_Kappa
class Optimizer_F_Tz_Kappa : public LMDiffOptim
{
  friend class TsaiEngine;

protected:
  virtual void 
    errfunc(CvMat* params, CvMat* err_func) const;
  
private:
  Optimizer_F_Tz_Kappa(TsaiEngine& _e) : engine_(_e) {};

  TsaiEngine& engine_;
};

//- Optimizer_5Par_LateDistorRemov
class Optimizer_5Par_LateDistorRemov : public LMDiffOptim
{
  friend class TsaiEngine;

protected:
  virtual void 
    errfunc(CvMat* params, CvMat* err_func) const;
  
private:
  Optimizer_5Par_LateDistorRemov(TsaiEngine& _e) : engine_(_e) {};

  TsaiEngine& engine_;
};

//- Optimizer_5Par_EarlyDistorRemov
class Optimizer_5Par_EarlyDistorRemov : public LMDiffOptim
{
  friend class TsaiEngine;

protected:
  virtual void 
    errfunc(CvMat* params, CvMat* err_func) const;
  
private:
  Optimizer_5Par_EarlyDistorRemov(TsaiEngine& _e) : engine_(_e) {};

  TsaiEngine& engine_;
};

//- Optimizer_NonIC
class Optimizer_NonIC : public LMDiffOptim
{
  friend class TsaiEngine;

protected:
  virtual void 
    errfunc(CvMat* params, CvMat* err_func) const;
  
private:
  Optimizer_NonIC(TsaiEngine& _e) : engine_(_e) {};

  TsaiEngine& engine_;
};

//- Optimizer_Full
class Optimizer_Full : public LMDiffOptim
{
  friend class TsaiEngine;

protected:
  virtual void 
    errfunc(CvMat* params, CvMat* err_func) const;
  
private:
  Optimizer_Full(TsaiEngine& _e) : engine_(_e) {};

  TsaiEngine& engine_;
};
/** @} */
/** @} */


}

#if defined (__ISL_INLINE__)
# include "TsaiEngine.i"
#endif // __ISL_INLINE__


#endif
