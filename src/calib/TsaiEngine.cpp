//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#include "TsaiEngine.h"
#include "InternalErrorHandler.h"
#include <iostream>

#if !defined (__ISL_INLINE__)
# include "TsaiEngine.i"
#endif


namespace isl {

#define SIGNBIT(a)      (((a) > 0) ? 0 : 1)
#define SQR(a)          ((a) * (a))



TsaiEngine::TsaiEngine()
{
}

TsaiEngine::~TsaiEngine()
{
  cvReleaseMat(&this->distorted_coord_);
  cvReleaseMat(&this->r_distorted_squared_);
  cvReleaseMat(&this->U_);
}

void
TsaiEngine::learn(const PatternAnalyzer& _pc)
{
  this->p_c_ = &_pc;
  this->nb_pt_ = _pc.nb_pt();


  const Image& im = _pc.preprocessed_image();

  this->cam_p_.Ncx = im.width();
  this->cam_p_.Nfx = im.width();
  this->cam_p_.dx = 7.4E-3;
  this->cam_p_.dy = 7.4E-3;
  this->cam_p_.dpx = 7.4E-3;
  this->cam_p_.dpy = 7.4E-3;
  this->cam_p_.Cx = 0.5 * im.width();
  this->cam_p_.Cy = 0.5 * im.height();
  this->cam_p_.sx = 1;

  this->result_.Cx = this->cam_p_.Cx;
  this->result_.Cy = this->cam_p_.Cy;



  std::cout << "------------------------------------------------" << std::endl;

  this->three_parm_optimization();

  std::cout << "------------------------------------------------" << std::endl;

  this->five_parm_optimization_with_late_distortion_removal();

  std::cout << "------------------------------------------------" << std::endl;

  this->five_parm_optimization_with_early_distortion_removal();

  std::cout << "------------------------------------------------" << std::endl;

  this->nic_optimization();

  std::cout << "------------------------------------------------" << std::endl;

  this->full_optimization();

}



void
TsaiEngine::undistort(const Image&, Image&, int) const
{

}



void
TsaiEngine::draw_error_map(Image&, double, double, int, int) const
{

}






void
TsaiEngine::three_parm_optimization()
{
  this->compute_Xd_Yd_and_r_squared ();

  this->compute_U ();

  this->compute_Tx_and_Ty ();

  this->compute_R ();

  this->compute_approximate_f_and_Tz ();

  if (this->result_.f < 0)
  {
    /* try the other solution for the orthonormal matrix */
    this->result_.r3 = -this->result_.r3;
    this->result_.r6 = -this->result_.r6;
    this->result_.r7 = -this->result_.r7;
    this->result_.r8 = -this->result_.r8;
	  this->solve_RPY_transform ();

    this->compute_approximate_f_and_Tz ();
  }

  this->compute_exact_f_and_Tz();

}

void
TsaiEngine::solve_RPY_transform ()
{
  double  sg,
          cg;
  
  CalibResults& result = this->result_;

  result.Rz = atan2 (result.r4, result.r1);

  sg = sin(result.Rz);
  cg = cos(result.Rz);

  result.Ry = atan2 (-result.r7, result.r1 * cg + result.r4 * sg);

  result.Rx = atan2 (result.r3 * sg - result.r6 * cg, result.r5 * cg - result.r2 * sg);

}

void
TsaiEngine::apply_RPY_transform ()
{
  double  sx, cx, sy, cy, sz, cz;
  
  CalibResults& result = this->result_;

  sx = sin(result.Rx);
  cx = cos(result.Rx);
  sy = sin(result.Ry);
  cy = cos(result.Ry);
  sz = sin(result.Rz);
  cz = cos(result.Rz);

  result.r1 = cy * cz;
  result.r2 = sx * sy * cz - cx * sz;
  result.r3 = sx * sz + cx * sy * cz;
  result.r4 = cy * sz;
  result.r5 = sx * sy * sz + cx * cz;
  result.r6 = cx * sy * sz - sx * cz;
  result.r7 = - sy;
  result.r8 = sx * cy;
  result.r9 = cx * cy;

}


void
TsaiEngine::remove_Xd_Yd_sensor_plane_distorsion ()
{
  CvMat* dist_coord = this->distorted_coord_;
  CvMat* r_squared  = this->r_distorted_squared_;
  double kappa1 = this->result_.kappa1;
  CvMat* temp = 0;

  CV_FUNCNAME( "Optimizer_F_Tz_Kappa::errfunc" );
  __BEGIN__;



  CvMat Xd, Yd;
  cvGetCol(dist_coord, &Xd, 0);
  cvGetCol(dist_coord, &Yd, 1);

  CV_CALL( temp = cvCreateMat(this->nb_pt_, 1, CV_64F) );

  //- put (1 + kappa1 * r²) in the temp vector
  CV_CALL( cvScale( r_squared, temp, kappa1, 1) ); 

  //- Xd' = Xd * (1 + k r²)
  CV_CALL( cvMul( &Xd, temp, &Xd, 1) );
  //- Yd' = Yd * (1 + k r²)
  CV_CALL( cvMul( &Yd, temp, &Yd, 1) );

  //- temp = Xd'²
  CV_CALL( cvMul( &Xd, &Xd, temp, 1) );
  //- r_squared = Yd'²
  CV_CALL( cvMul( &Yd, &Yd, r_squared, 1) );
 
  //- r_squared = Xd'² + Yd'²
  CV_CALL( cvAdd( temp, r_squared, r_squared) );

  __END__;

  cvReleaseMat(&temp);

  __ISL_CHECK_ERROR__;
}

void
TsaiEngine::compute_Xd_Yd_and_r_squared()
{
  CvMat* dc = 0;
  CvMat* r_squared = 0;
  CvMat* r_squared_tmp = 0;
  const CvMat* image_coord = this->p_c_->image_coord();
  
  int nb_pt = this->nb_pt_;

  CV_FUNCNAME( "TsaiEngine::compute_Xd_Yd_and_r_squared" );
  __BEGIN__;

  CV_CALL( dc = cvCreateMat(nb_pt, 2, CV_64F) );
  CV_CALL( r_squared = cvCreateMat(nb_pt, 1, CV_64F) );
  CV_CALL( r_squared_tmp = cvCreateMat(nb_pt, 1, CV_64F) );

  CvMat dc_X, image_coord_X;
  CvMat dc_Y, image_coord_Y;   
  
  CV_CALL( cvGetCol(dc, &dc_X, 0) );
  CV_CALL( cvGetCol(image_coord, &image_coord_X, 0) );
  CV_CALL( cvGetCol(dc, &dc_Y, 1) );
  CV_CALL( cvGetCol(image_coord, &image_coord_Y, 1) );

  CV_CALL( cvConvertScale(&image_coord_X, &dc_X, cam_p_.dpx / cam_p_.sx, - cam_p_.Cx * cam_p_.dpx / cam_p_.sx ) );
  CV_CALL( cvConvertScale(&image_coord_Y, &dc_Y, cam_p_.dpy, - cam_p_.Cy * cam_p_.dpy ) );

  CV_CALL( cvMul(&dc_X, &dc_X, r_squared_tmp) );
  CV_CALL( cvMul(&dc_Y, &dc_Y, r_squared) );
  CV_CALL( cvAdd(r_squared_tmp, r_squared, r_squared) );


  __END__;
  cvReleaseMat(&r_squared_tmp);

  this->distorted_coord_ = dc;
  this->r_distorted_squared_ = r_squared;
  __ISL_CHECK_ERROR__;
}


void
TsaiEngine::compute_U()
{
  CvMat* U;
  CvMat* M;
  const CvMat* world_coord = this->p_c_->world_coord();
  const CvMat* dist_image_coord = this->distorted_coord_;
  int nb_pt = this->nb_pt_;


  CV_FUNCNAME( "TsaiEngine::compute_U" );
  __BEGIN__;

  CV_CALL( U = cvCreateMat(5, 1, CV_64F) );
  CV_CALL( M = cvCreateMat(nb_pt, 5, CV_64F) );

  CvMat x_w, y_w, x_i, y_i, col_M;
  
  CV_CALL( cvGetCol(world_coord, &x_w, 0) );
  CV_CALL( cvGetCol(world_coord, &y_w, 1) );
  CV_CALL( cvGetCol(dist_image_coord, &x_i, 0) );
  CV_CALL( cvGetCol(dist_image_coord, &y_i, 1) );

  //- column[0] : x_w * y_i
  CV_CALL( cvGetCol(M, &col_M, 0) );
  CV_CALL( cvMul(&x_w, &y_i, &col_M) );

  //- column[1] : y_w * y_i
  CV_CALL( cvGetCol(M, &col_M, 1) );
  CV_CALL( cvMul(&y_w, &y_i, &col_M) );

  //- column[2] : y_i
  CV_CALL( cvGetCol(M, &col_M, 2) );
  CV_CALL( cvCopy(&y_i, &col_M) );
  
  //- column[3] : - x_w * x_i
  CV_CALL( cvGetCol(M, &col_M, 3) );
  CV_CALL( cvMul(&x_w, &x_i, &col_M, -1) );
 
  //- column[5] : - y_w * x_i
  CV_CALL( cvGetCol(M, &col_M, 4) );
  CV_CALL( cvMul(&y_w, &x_i, &col_M, -1) );


  CV_CALL( cvSolve( M, &x_i, U, CV_SVD) );

  __END__;

  cvReleaseMat(&M);
  this->U_ = U;

  __ISL_CHECK_ERROR__;
}


void
TsaiEngine::compute_Tx_and_Ty()
{
  int       far_point;
  double    Tx,
            Ty,
            Ty_squared,
            x,
            y,
            xw,
            yw,
            Sr,
            r1p,
            r2p,
            Txp,
            r4p,
            r5p,
            r1,
            r2,
            r4,
            r5;
 
  const CvMat* world_coord = this->p_c_->world_coord();
  const CvMat* U = this->U_;
  const CvMat* dc = this->distorted_coord_;
  const CvMat* r_squared = this->r_distorted_squared_;
  
  CV_FUNCNAME( "TsaiEngine::compute_Tx_and_Ty" );
  __BEGIN__;


  r1p = cvmGet(U, 0, 0);
  r2p = cvmGet(U, 1, 0);
  Txp = cvmGet(U, 2, 0);
  r4p = cvmGet(U, 3, 0);
  r5p = cvmGet(U, 4, 0);


  const double EPSILON = 1.0E-8;

  if ( fabs (r1p) < EPSILON && fabs (r2p) < EPSILON )
    Ty_squared = 1 / (SQR (r4p) + SQR (r5p));
  else if ( fabs (r4p) < EPSILON  &&  fabs (r5p) < EPSILON )
    Ty_squared = 1 / (SQR (r1p) + SQR (r2p));
  else if ( fabs (r1p) < EPSILON  &&  fabs (r4p) < EPSILON )
    Ty_squared = 1 / (SQR (r2p) + SQR (r5p));
  else if ( fabs (r2p) < EPSILON  &&  fabs (r5p) < EPSILON )
    Ty_squared = 1 / (SQR (r1p) + SQR (r4p));
  else 
  {
    Sr = SQR (r1p) + SQR (r2p) + SQR (r4p) + SQR (r5p);
    Ty_squared = (Sr - sqrt (SQR (Sr) - 4 * SQR (r1p * r5p - r4p * r2p))) /
                  (2 * SQR (r1p * r5p - r4p * r2p));
  }


  //- find the farthest point from the image center
  CvPoint pt;
  CV_CALL( cvMinMaxLoc(r_squared, 0, 0, 0, &pt) );
  far_point = pt.y;

  /* now find the sign for Ty */
  /* start by assuming Ty > 0 */
  Ty = sqrt (Ty_squared);
  r1 = r1p * Ty;
  r2 = r2p * Ty;
  Tx = Txp * Ty;
  r4 = r4p * Ty;
  r5 = r5p * Ty;
  xw = cvmGet(world_coord, far_point, 0);
  yw = cvmGet(world_coord, far_point, 1);
  x = r1 * xw + r2 * yw + Tx;
  y = r4 * xw + r5 * yw + Ty;

  /* flip Ty if we guessed wrong */
  if ( SIGNBIT (x) != SIGNBIT (cvmGet(dc, far_point, 0)) ||
       SIGNBIT (y) != SIGNBIT (cvmGet(dc, far_point, 1)) )
    Ty = -Ty;

  /* update the calibration constants */
  this->result_.Tx = Txp * Ty;
  this->result_.Ty = Ty;

  __END__;

  __ISL_CHECK_ERROR__;

}


void
TsaiEngine::compute_R()
{
  double  r1,
          r2,
          r3,
          r4,
          r5,
          r6,
          r7,
          r8,
          r9;
  
  CalibResults& result = this->result_;
  CvMat* U = this->U_;

//- this method does not return error
//  CV_FUNCNAME( "TsaiEngine::compute_R" );
//  __BEGIN__;


  r1 = cvmGet(U, 0, 0) * result.Ty;
  r2 = cvmGet(U, 1, 0) * result.Ty;
  r3 = sqrt (1 - SQR (r1) - SQR (r2));

  r4 = cvmGet(U, 3, 0) * result.Ty;
  r5 = cvmGet(U, 4, 0) * result.Ty;
  r6 = sqrt (1 - SQR (r4) - SQR (r5));
  if (!SIGNBIT (r1 * r4 + r2 * r5))
    r6 = -r6;

  /* use the outer product of the first two rows to get the last row */
  r7 = r2 * r6 - r3 * r5;
  r8 = r3 * r4 - r1 * r6;
  r9 = r1 * r5 - r2 * r4;

  /* update the calibration constants */
  result.r1 = r1;
  result.r2 = r2;
  result.r3 = r3;
  result.r4 = r4;
  result.r5 = r5;
  result.r6 = r6;
  result.r7 = r7;
  result.r8 = r8;
  result.r9 = r9;

  /* fill in result.Rx, result.Ry and result.Rz */
  this->solve_RPY_transform ();

//  __END__;
//  __ISL_CHECK_ERROR__;
}



void
TsaiEngine::compute_approximate_f_and_Tz()
{
  CvMat* M;
  CvMat* a;
  CvMat* b;
  const CvMat* world_coord = this->p_c_->world_coord();
  const CvMat* dist_coord = this->distorted_coord_;

  CV_FUNCNAME( "TsaiEngine::compute_approximate_f_and_Tz" );
  __BEGIN__;

  CalibResults& result = this->result_;
  int nb_pt = this->nb_pt_;

  CV_CALL( M = cvCreateMat(nb_pt, 2, CV_64F) );
  CV_CALL( a = cvCreateMat(2,     1, CV_64F) );
  CV_CALL( b = cvCreateMat(nb_pt, 1, CV_64F) );


  CvMat x_w, y_w, col_M, Yd;
  CV_CALL( cvGetCol(world_coord, &x_w, 0) );
  CV_CALL( cvGetCol(world_coord, &y_w, 1) );
  
  CV_CALL( cvGetCol(dist_coord, &Yd, 1) );

  //- fill the 1st col of M
  CV_CALL( cvGetCol(M, &col_M, 0) );
  CV_CALL( cvAddWeighted(&x_w, result.r4, &y_w, result.r5, result.Ty, &col_M) );

  //- fill the 2nd col of M
  CV_CALL( cvGetCol(M, &col_M, 1) );
  CV_CALL( cvConvertScale(&Yd, &col_M, -1) );

  //- fill the 'b' matrix
  CV_CALL( cvAddWeighted(&x_w, result.r7, &y_w, result.r8, 0, b) );
  CV_CALL( cvMul(b, &Yd, b) );


  CV_CALL( cvSolve( M, b, a, CV_SVD) );

  result.f = cvmGet(a, 0, 0);
  result.Tz = cvmGet(a, 1, 0);
  result.kappa1 = 0; // this is the assumption that our calculation was made under

  __END__;

  cvReleaseMat(&M);
  cvReleaseMat(&a);
  cvReleaseMat(&b);

  __ISL_CHECK_ERROR__;

}


void
Optimizer_F_Tz_Kappa::errfunc(CvMat* _params, CvMat* _err_func) const
{

  CvMat* xc = 0;
  CvMat* yc = 0;
  CvMat* zc = 0;
  CvMat* Xu1 = 0;
  CvMat* Yu1 = 0;
  CvMat* Xu2 = 0;
  CvMat* Yu2 = 0;

  CV_FUNCNAME( "Optimizer_F_Tz_Kappa::errfunc" );
  __BEGIN__;

  double f,
         Tz,
         kappa1;

  CvMat xw, yw, Xd, Yd;

  f      = cvmGet(_params, 0, 0);
  Tz     = cvmGet(_params, 1, 0);
  kappa1 = cvmGet(_params, 2, 0);

  const CalibResults& cr = this->engine_.result_;
  const TsaiEngine& eng = this->engine_;

  CV_CALL( cvGetCol(eng.p_c_->world_coord(), &xw, 0) );
  CV_CALL( cvGetCol(eng.p_c_->world_coord(), &yw, 1) );
  CV_CALL( cvGetCol(eng.distorted_coord_, &Xd, 0) );
  CV_CALL( cvGetCol(eng.distorted_coord_, &Yd, 1) );
  CV_CALL( xc  = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( yc  = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( zc  = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( Xu1 = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( Yu1 = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( Xu2 = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( Yu2 = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );


  /* convert from world coordinates to camera coordinates */
  /* Note: zw is implicitly assumed to be zero for these (coplanar) calculations */
  CV_CALL( cvAddWeighted(&xw, cr.r1, &yw, cr.r2, cr.Tx, xc) );
  CV_CALL( cvAddWeighted(&xw, cr.r4, &yw, cr.r5, cr.Ty, yc) );
  CV_CALL( cvAddWeighted(&xw, cr.r7, &yw, cr.r8, Tz,    zc) );

  /* convert from camera coordinates to undistorted sensor coordinates */
  CV_CALL( cvDiv( xc, zc, Xu1, f) ); //-   Xu_1 <- f * xc / zc
  CV_CALL( cvDiv( yc, zc, Yu1, f) ); //-   Yu_1 <- f * yc / zc

  //- Xu_2 = Xd * (1 + kappa1 * r²)
  //- Yu_2 = Yd * (1 + kappa1 * r²)
  CV_CALL( cvScale( eng.r_distorted_squared_, Xu2, kappa1, 1) ); // Xu_2 <- 1 + kappa1 * r²
  CV_CALL( cvMul( Xu2, &Yd, Yu2, 1) ); // Yu_2 <- Yd * Xu_2
  CV_CALL( cvMul( Xu2, &Xd, Xu2, 1) ); // Xu_2 <- Xd * Xu_2

  /* record the error in the undistorted sensor coordinates */
  CV_CALL( cvSub( Xu2, Xu1, Xu1) );
  CV_CALL( cvMul( Xu1, Xu1, Xu1) );
  CV_CALL( cvSub( Yu2, Yu1, Yu1) );
  CV_CALL( cvMul( Yu1, Yu1, Yu1) );
  CV_CALL( cvAdd( Xu1, Yu1, _err_func) );

  __END__;

  cvReleaseMat(&xc);
  cvReleaseMat(&yc);
  cvReleaseMat(&zc);
  cvReleaseMat(&Xu1);
  cvReleaseMat(&Yu1);
  cvReleaseMat(&Xu2);
  cvReleaseMat(&Yu2);
  
  __ISL_CHECK_ERROR__;

}

void
TsaiEngine::compute_exact_f_and_Tz()
{
  CvMat* init_guess = 0;
  const CvMat* result = 0;

  Optimizer_F_Tz_Kappa optim(*this);

  CV_FUNCNAME( "TsaiEngine::compute_exact_f_and_Tz" );
  __BEGIN__;

  CV_CALL( init_guess = cvCreateMat(3, 1, CV_64F) );
  cvmSet(init_guess, 0, 0, this->result_.f);
  cvmSet(init_guess, 1, 0, this->result_.Tz);
  cvmSet(init_guess, 2, 0, this->result_.kappa1);

  std::cout << "f  = " << this->result_.f << std::endl;
  std::cout << "Tz = " << this->result_.Tz << std::endl;
  std::cout << "k1 = " << this->result_.kappa1 << std::endl;


  optim.init(this->nb_pt_, init_guess);
  
  optim.run(ISL_LMOPTIM_SCALE_JAC);

  result = optim.result();
  

  std::cout << "compute_exact_f_and_Tz : " <<  optim.nb_iter() << std::endl;

  this->result_.f      = cvmGet(result, 0, 0);
  this->result_.Tz     = cvmGet(result, 1, 0);
  this->result_.kappa1 = cvmGet(result, 2, 0);

  std::cout << "f  = " << this->result_.f << std::endl;
  std::cout << "Tz = " << this->result_.Tz << std::endl;
  std::cout << "k1 = " << this->result_.kappa1 << std::endl;

  __END__;
  cvReleaseMat(&init_guess);
  __ISL_CHECK_ERROR__;

}


void
Optimizer_5Par_LateDistorRemov::errfunc(CvMat* _params, CvMat* _err_func) const
{
  CvMat* xc = 0;
  CvMat* yc = 0;
  CvMat* zc = 0;
  CvMat* Xu1 = 0;
  CvMat* Yu1 = 0;
  CvMat* Xu2 = 0;
  CvMat* Yu2 = 0;


  CV_FUNCNAME( "Optimizer_5Par_LateDistorRemov::errfunc" );
  __BEGIN__;

  double f,
         Tz,
         kappa1;

  CvMat xw, yw, Xd, Yd;

  CalibResults& cr = this->engine_.result_;
  TsaiEngine& eng = this->engine_;

  f      = cvmGet(_params, 0, 0);
  Tz     = cvmGet(_params, 1, 0);
  kappa1 = cvmGet(_params, 2, 0);

  cr.Cx = cvmGet(_params, 3, 0);
  cr.Cy = cvmGet(_params, 4, 0);

  eng.compute_Xd_Yd_and_r_squared();

  eng.compute_U();

  eng.compute_Tx_and_Ty();

  eng.compute_R();
  
  eng.compute_approximate_f_and_Tz();

  if (cr.f < 0)
  {
    /* try the other solution for the orthonormal matrix */
    cr.r3 = - cr.r3;
    cr.r6 = - cr.r6;
    cr.r7 = - cr.r7;
    cr.r8 = - cr.r8;

	  eng.solve_RPY_transform ();

    eng.compute_approximate_f_and_Tz ();
  }

  CV_CALL( cvGetCol(eng.p_c_->world_coord(), &xw, 0) );
  CV_CALL( cvGetCol(eng.p_c_->world_coord(), &yw, 1) );
  CV_CALL( cvGetCol(eng.distorted_coord_, &Xd, 0) );
  CV_CALL( cvGetCol(eng.distorted_coord_, &Yd, 1) );
  CV_CALL( xc = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( yc = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( zc = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );

  CV_CALL( Xu1 = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( Yu1 = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( Xu2 = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( Yu2 = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );

  /* convert from world coordinates to camera coordinates */
  /* Note: zw is implicitly assumed to be zero for these (coplanar) calculations */
  CV_CALL( cvAddWeighted(&xw, cr.r1, &yw, cr.r2, cr.Tx, xc) );
  CV_CALL( cvAddWeighted(&xw, cr.r4, &yw, cr.r5, cr.Ty, yc) );
  CV_CALL( cvAddWeighted(&xw, cr.r7, &yw, cr.r8, Tz, zc) );

  /* convert from camera coordinates to undistorted sensor coordinates */
  CV_CALL( cvDiv( xc, zc, Xu1, f) ); //-   Xu_1 = f * xc / zc
  CV_CALL( cvDiv( yc, zc, Yu1, f) ); //-   Yu_1 = f * yc / zc

  //- Xu_2 = Xd * (1 + kappa1 * r²)
  //- Yu_2 = Yd * (1 + kappa1 * r²)
  CV_CALL( cvScale( eng.r_distorted_squared_, Xu2, kappa1, 1) ); // Xu_2 = 1 + kappa1 * r²
  CV_CALL( cvMul( Xu2, &Yd, Yu2, 1) ); // Yu_2 = Yd * Xu_2
  CV_CALL( cvMul( Xu2, &Xd, Xu2, 1) ); // Xu_2 = Xd * Xu_2

  /* record the error in the undistorted sensor coordinates */
  CV_CALL( cvSub( Xu1, Xu2, Xu1) );
  CV_CALL( cvMul( Xu1, Xu1, Xu1) );
  CV_CALL( cvSub( Yu1, Yu2, Yu1) );
  CV_CALL( cvMul( Yu1, Yu1, Yu1) );
  CV_CALL( cvAdd( Xu1, Yu1, _err_func) );
  
  __END__;

  cvReleaseMat(&xc);
  cvReleaseMat(&yc);
  cvReleaseMat(&zc);
  cvReleaseMat(&Xu1);
  cvReleaseMat(&Yu1);
  cvReleaseMat(&Xu2);
  cvReleaseMat(&Yu2);
  
  __ISL_CHECK_ERROR__;
}

void
TsaiEngine::five_parm_optimization_with_late_distortion_removal()
{
  CvMat* init_guess = 0;
  const CvMat* result = 0;

  Optimizer_5Par_LateDistorRemov optim(*this);

  CV_FUNCNAME( "TsaiEngine::five_parm_optimization_with_late_distortion_removal" );
  __BEGIN__;

  CV_CALL( init_guess = cvCreateMat(5, 1, CV_64F) );
  cvmSet(init_guess, 0, 0, this->result_.f);
  cvmSet(init_guess, 1, 0, this->result_.Tz);
  cvmSet(init_guess, 2, 0, this->result_.kappa1);
  cvmSet(init_guess, 3, 0, this->result_.Cx);
  cvmSet(init_guess, 4, 0, this->result_.Cy);

  optim.init(this->nb_pt_, init_guess);
  
  std::cout << "f  = " << this->result_.f << std::endl;
  std::cout << "Tz = " << this->result_.Tz << std::endl;
  std::cout << "k1 = " << this->result_.kappa1 << std::endl;
  std::cout << "Cx = " << this->result_.Cx << std::endl;
  std::cout << "Cy = " << this->result_.Cy << std::endl;

  optim.run(ISL_LMOPTIM_SCALE_JAC);

  result = optim.result();

  std::cout << "five_parm_optimization_with_late_distortion_removal : " <<  optim.nb_iter() << std::endl;

  this->result_.f      = cvmGet(result, 0, 0);
  this->result_.Tz     = cvmGet(result, 1, 0);
  this->result_.kappa1 = cvmGet(result, 2, 0);
  this->result_.Cx     = cvmGet(result, 3, 0);
  this->result_.Cy     = cvmGet(result, 4, 0);

  std::cout << "f  = " << this->result_.f << std::endl;
  std::cout << "Tz = " << this->result_.Tz << std::endl;
  std::cout << "k1 = " << this->result_.kappa1 << std::endl;
  std::cout << "Cx = " << this->result_.Cx << std::endl;
  std::cout << "Cy = " << this->result_.Cy << std::endl;

  __END__;
  cvReleaseMat(&init_guess);
  __ISL_CHECK_ERROR__;
}


void
Optimizer_5Par_EarlyDistorRemov::errfunc(CvMat* _params, CvMat* _err_func) const
{
  CvMat* xc = 0;
  CvMat* yc = 0;
  CvMat* zc = 0;
  CvMat* Xu1 = 0;
  CvMat* Yu1 = 0;


  CV_FUNCNAME( "Optimizer_5Par_EarlyDistorRemov::errfunc" );
  __BEGIN__;

  double f,
         Tz;
  
  CvMat xw, yw, Xd, Yd;

  CalibResults& cr = this->engine_.result_;
  TsaiEngine& eng = this->engine_;

  f         = cvmGet(_params, 0, 0);
  Tz        = cvmGet(_params, 1, 0);

  cr.kappa1 = cvmGet(_params, 2, 0);
  cr.Cx     = cvmGet(_params, 3, 0);
  cr.Cy     = cvmGet(_params, 4, 0);

  eng.compute_Xd_Yd_and_r_squared();

  eng.remove_Xd_Yd_sensor_plane_distorsion();

  eng.compute_U();

  eng.compute_Tx_and_Ty();

  eng.compute_R();
  
  eng.compute_approximate_f_and_Tz();

  if (cr.f < 0)
  {
    /* try the other solution for the orthonormal matrix */
    cr.r3 = - cr.r3;
    cr.r6 = - cr.r6;
    cr.r7 = - cr.r7;
    cr.r8 = - cr.r8;

	  eng.solve_RPY_transform ();

    eng.compute_approximate_f_and_Tz ();
  }


  CV_CALL( cvGetCol(eng.p_c_->world_coord(), &xw, 0) );
  CV_CALL( cvGetCol(eng.p_c_->world_coord(), &yw, 1) );
  CV_CALL( cvGetCol(eng.distorted_coord_, &Xd, 0) );
  CV_CALL( cvGetCol(eng.distorted_coord_, &Yd, 1) );
  CV_CALL( xc = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( yc = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( zc = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );

  CV_CALL( Xu1 = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( Yu1 = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );

  /* convert from world coordinates to camera coordinates */
  /* Note: zw is implicitly assumed to be zero for these (coplanar) calculations */
  CV_CALL( cvAddWeighted(&xw, cr.r1, &yw, cr.r2, cr.Tx, xc) );
  CV_CALL( cvAddWeighted(&xw, cr.r4, &yw, cr.r5, cr.Ty, yc) );
  CV_CALL( cvAddWeighted(&xw, cr.r7, &yw, cr.r8, Tz, zc) );


  /* convert from camera coordinates to undistorted sensor coordinates */
  CV_CALL( cvDiv( xc, zc, Xu1, f) ); //-   Xu_1 = f * xc / zc
  CV_CALL( cvDiv( yc, zc, Yu1, f) ); //-   Yu_1 = f * yc / zc

  //- Xu_2 = Xd * (1 + kappa1 * r²) -> operation already done in remove_Xd_Yd_sensor_plane_distorsion()
  //- Yu_2 = Yd * (1 + kappa1 * r²) -> operation already done in remove_Xd_Yd_sensor_plane_distorsion()


  /* record the error in the undistorted sensor coordinates */
  CV_CALL( cvSub( Xu1, &Xd, Xu1) );
  CV_CALL( cvMul( Xu1, Xu1, Xu1) );
  CV_CALL( cvSub( Yu1, &Yd, Yu1) );
  CV_CALL( cvMul( Yu1, Yu1, Yu1) );
  CV_CALL( cvAdd( Xu1, Yu1, _err_func) );


  __END__;

  cvReleaseMat(&xc);
  cvReleaseMat(&yc);
  cvReleaseMat(&zc);
  cvReleaseMat(&Xu1);
  cvReleaseMat(&Yu1);
  
  __ISL_CHECK_ERROR__;

}


void
TsaiEngine::five_parm_optimization_with_early_distortion_removal()
{
  CvMat* init_guess = 0;
  const CvMat* result = 0;

  Optimizer_5Par_EarlyDistorRemov optim(*this);

  CV_FUNCNAME( "TsaiEngine::five_parm_optimization_with_early_distortion_removal" );
  __BEGIN__;

  CV_CALL( init_guess = cvCreateMat(5, 1, CV_64F) );
  cvmSet(init_guess, 0, 0, this->result_.f);
  cvmSet(init_guess, 1, 0, this->result_.Tz);
  cvmSet(init_guess, 2, 0, this->result_.kappa1);
  cvmSet(init_guess, 3, 0, this->result_.Cx);
  cvmSet(init_guess, 4, 0, this->result_.Cy);

  optim.init(this->nb_pt_, init_guess);
  
  std::cout << "f  = " << this->result_.f << std::endl;
  std::cout << "Tz = " << this->result_.Tz << std::endl;
  std::cout << "k1 = " << this->result_.kappa1 << std::endl;
  std::cout << "Cx = " << this->result_.Cx << std::endl;
  std::cout << "Cy = " << this->result_.Cy << std::endl;

  optim.run(ISL_LMOPTIM_SCALE_JAC);

  result = optim.result();

  std::cout << "five_parm_optimization_with_early_distortion_removal : " <<  optim.nb_iter() << std::endl;

  this->result_.f      = cvmGet(result, 0, 0);
  this->result_.Tz     = cvmGet(result, 1, 0);
  this->result_.kappa1 = cvmGet(result, 2, 0);
  this->result_.Cx     = cvmGet(result, 3, 0);
  this->result_.Cy     = cvmGet(result, 4, 0);

  std::cout << "f  = " << this->result_.f << std::endl;
  std::cout << "Tz = " << this->result_.Tz << std::endl;
  std::cout << "k1 = " << this->result_.kappa1 << std::endl;
  std::cout << "Cx = " << this->result_.Cx << std::endl;
  std::cout << "Cy = " << this->result_.Cy << std::endl;


  __END__;
  cvReleaseMat(&init_guess);
  __ISL_CHECK_ERROR__;

}


void
Optimizer_NonIC::errfunc(CvMat* _params, CvMat* _err_func) const
{
  CvMat* xc = 0;
  CvMat* yc = 0;
  CvMat* zc = 0;
  CvMat* Xu1 = 0;
  CvMat* Yu1 = 0;
  CvMat* Xu2 = 0;
  CvMat* Yu2 = 0;

  CV_FUNCNAME( "Optimizer_NonIC::errfunc" );
  __BEGIN__;

  double    Rx,
            Ry,
            Rz,
            Tx,
            Ty,
            Tz,
            kappa1,
            f,
            r1,
            r2,
            r4,
            r5,
            r7,
            r8,
            sx,
            sy,
            sz,
            cx,
            cy,
            cz;
  
  CvMat xw, yw, Xd, Yd;

  TsaiEngine& eng = this->engine_;


  Rx     = cvmGet(_params, 0, 0);
  Ry     = cvmGet(_params, 1, 0);
  Rz     = cvmGet(_params, 2, 0);
  Tx     = cvmGet(_params, 3, 0);
  Ty     = cvmGet(_params, 4, 0);
  Tz     = cvmGet(_params, 5, 0);
  kappa1 = cvmGet(_params, 6, 0);
  f      = cvmGet(_params, 7, 0);

  sx = sin(Rx);
  cx = cos(Rx);
  sy = sin(Ry);
  cy = cos(Ry);
  sz = sin(Rz);
  cz = cos(Rz);

  r1 = cy * cz;
  r2 = sx * sy * cz - cx * sz;
  r4 = cy * sz;
  r5 = sx * sy * sz + cx * cz;
  r7 = - sy;
  r8 = sx * cy;

  CV_CALL( cvGetCol(eng.p_c_->world_coord(), &xw, 0) );
  CV_CALL( cvGetCol(eng.p_c_->world_coord(), &yw, 1) );
  CV_CALL( cvGetCol(eng.distorted_coord_, &Xd, 0) );
  CV_CALL( cvGetCol(eng.distorted_coord_, &Yd, 1) );
  CV_CALL( xc = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( yc = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( zc = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );

  CV_CALL( Xu1 = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( Yu1 = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( Xu2 = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( Yu2 = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );

  /* convert from world coordinates to camera coordinates */
  /* Note: zw is implicitly assumed to be zero for these (coplanar) calculations */
  CV_CALL( cvAddWeighted(&xw, r1, &yw, r2, Tx, xc) );
  CV_CALL( cvAddWeighted(&xw, r4, &yw, r5, Ty, yc) );
  CV_CALL( cvAddWeighted(&xw, r7, &yw, r8, Tz, zc) );


  /* convert from camera coordinates to undistorted sensor coordinates */
  CV_CALL( cvDiv( xc, zc, Xu1, f) ); //-   Xu_1 = f * xc / zc
  CV_CALL( cvDiv( yc, zc, Yu1, f) ); //-   Yu_1 = f * yc / zc

  //- Xu_2 = Xd * (1 + kappa1 * r²)
  //- Yu_2 = Yd * (1 + kappa1 * r²)
  CV_CALL( cvScale( eng.r_distorted_squared_, Xu2, kappa1, 1) ); // Xu_2 = 1 + kappa1 * r²
  CV_CALL( cvMul( Xu2, &Yd, Yu2, 1) ); // Yu_2 = Yd * Xu_2
  CV_CALL( cvMul( Xu2, &Xd, Xu2, 1) ); // Xu_2 = Xd * Xu_2

  /* record the error in the undistorted sensor coordinates */
  CV_CALL( cvSub( Xu1, Xu2, Xu1) );
  CV_CALL( cvMul( Xu1, Xu1, Xu1) );
  CV_CALL( cvSub( Yu1, Yu2, Yu1) );
  CV_CALL( cvMul( Yu1, Yu1, Yu1) );
  CV_CALL( cvAdd( Xu1, Yu1, _err_func) );

  __END__;

  cvReleaseMat(&xc);
  cvReleaseMat(&yc);
  cvReleaseMat(&zc);
  cvReleaseMat(&Xu1);
  cvReleaseMat(&Yu1);
  cvReleaseMat(&Xu2);
  cvReleaseMat(&Yu2);
  
  __ISL_CHECK_ERROR__;


}

void
TsaiEngine::nic_optimization()
{
  CvMat* init_guess = 0;
  const CvMat* result = 0;

  Optimizer_NonIC optim(*this);

  CV_FUNCNAME( "TsaiEngine::nic_optimization" );
  __BEGIN__;

  CV_CALL( init_guess = cvCreateMat(8, 1, CV_64F) );
  cvmSet(init_guess, 0, 0, this->result_.Rx);
  cvmSet(init_guess, 1, 0, this->result_.Ry);
  cvmSet(init_guess, 2, 0, this->result_.Rz);
  cvmSet(init_guess, 3, 0, this->result_.Tx);
  cvmSet(init_guess, 4, 0, this->result_.Ty);
  cvmSet(init_guess, 5, 0, this->result_.Tz);
  cvmSet(init_guess, 6, 0, this->result_.kappa1);
  cvmSet(init_guess, 7, 0, this->result_.f);

  std::cout << "Rx = " << this->result_.Rx << std::endl;
  std::cout << "Ry = " << this->result_.Ry << std::endl;
  std::cout << "Rz = " << this->result_.Rz << std::endl;
  std::cout << "Tx = " << this->result_.Tx << std::endl;
  std::cout << "Ty = " << this->result_.Ty << std::endl;
  std::cout << "Tz = " << this->result_.Tz << std::endl;
  std::cout << "k1 = " << this->result_.kappa1 << std::endl;
  std::cout << "f  = " << this->result_.f << std::endl;

  optim.init(this->nb_pt_, init_guess);

  this->compute_Xd_Yd_and_r_squared();
  
  optim.run(ISL_LMOPTIM_SCALE_JAC);

  result = optim.result();

  std::cout << "nic_optimization : " <<  optim.nb_iter() << std::endl;

  this->result_.Rx     = cvmGet(result, 0, 0);
  this->result_.Ry     = cvmGet(result, 1, 0);
  this->result_.Rz     = cvmGet(result, 2, 0);
  
  this->apply_RPY_transform();

  this->result_.Tx     = cvmGet(result, 3, 0);
  this->result_.Ty     = cvmGet(result, 4, 0);
  this->result_.Tz     = cvmGet(result, 5, 0);
  this->result_.kappa1 = cvmGet(result, 6, 0);
  this->result_.f      = cvmGet(result, 7, 0);

  std::cout << "Rx = " << this->result_.Rx << std::endl;
  std::cout << "Ry = " << this->result_.Ry << std::endl;
  std::cout << "Rz = " << this->result_.Rz << std::endl;
  std::cout << "Tx = " << this->result_.Tx << std::endl;
  std::cout << "Ty = " << this->result_.Ty << std::endl;
  std::cout << "Tz = " << this->result_.Tz << std::endl;
  std::cout << "k1 = " << this->result_.kappa1 << std::endl;
  std::cout << "f  = " << this->result_.f << std::endl;

  __END__;
  cvReleaseMat(&init_guess);
  __ISL_CHECK_ERROR__;
}


void
Optimizer_Full::errfunc(CvMat* _params, CvMat* _err_func) const
{
  CvMat* xc = 0;
  CvMat* yc = 0;
  CvMat* zc = 0;
  CvMat* Xu1 = 0;
  CvMat* Yu1 = 0;
  CvMat* Xu2 = 0;
  CvMat* Yu2 = 0;

  CV_FUNCNAME( "Optimizer_Full::errfunc" );
  __BEGIN__;

  double    Rx,
            Ry,
            Rz,
            Tx,
            Ty,
            Tz,
            kappa1,
            f,
            Cx,
            Cy,
            r1,
            r2,
            r4,
            r5,
            r7,
            r8,
            sx,
            sy,
            sz,
            cx,
            cy,
            cz;
  
  CvMat xw, yw, Xd, Yd;

  CalibResults& result = this->engine_.result_;
  TsaiEngine& eng = this->engine_;


  Rx     = cvmGet(_params, 0, 0);
  Ry     = cvmGet(_params, 1, 0);
  Rz     = cvmGet(_params, 2, 0);
  Tx     = cvmGet(_params, 3, 0);
  Ty     = cvmGet(_params, 4, 0);
  Tz     = cvmGet(_params, 5, 0);
  kappa1 = cvmGet(_params, 6, 0);
  f      = cvmGet(_params, 7, 0);
  Cx     = cvmGet(_params, 8, 0);
  Cy     = cvmGet(_params, 9, 0);

  sx = sin(Rx);
  cx = cos(Rx);
  sy = sin(Ry);
  cy = cos(Ry);
  sz = sin(Rz);
  cz = cos(Rz);

  r1 = cy * cz;
  r2 = sx * sy * cz - cx * sz;
  r4 = cy * sz;
  r5 = sx * sy * sz + cx * cz;
  r7 = - sy;
  r8 = sx * cy;

  CV_CALL( cvGetCol(eng.p_c_->world_coord(), &xw, 0) );
  CV_CALL( cvGetCol(eng.p_c_->world_coord(), &yw, 1) );
  CV_CALL( cvGetCol(eng.distorted_coord_, &Xd, 0) );
  CV_CALL( cvGetCol(eng.distorted_coord_, &Yd, 1) );
  CV_CALL( xc = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( yc = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( zc = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );

  CV_CALL( Xu1 = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( Yu1 = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( Xu2 = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );
  CV_CALL( Yu2 = cvCreateMat(eng.nb_pt_, 1, CV_64FC1) );

  /* convert from world coordinates to camera coordinates */
  /* Note: zw is implicitly assumed to be zero for these (coplanar) calculations */
  CV_CALL( cvAddWeighted(&xw, r1, &yw, r2, Tx, xc) );
  CV_CALL( cvAddWeighted(&xw, r4, &yw, r5, Ty, yc) );
  CV_CALL( cvAddWeighted(&xw, r7, &yw, r8, Tz, zc) );


  /* convert from camera coordinates to undistorted sensor coordinates */
  CV_CALL( cvDiv( xc, zc, Xu1, f) ); //-   Xu_1 = f * xc / zc
  CV_CALL( cvDiv( yc, zc, Yu1, f) ); //-   Yu_1 = f * yc / zc

  result.Cx = Cx;
  result.Cy = Cy;

  eng.compute_Xd_Yd_and_r_squared();

  //- Xu_2 = Xd * (1 + kappa1 * r²)
  //- Yu_2 = Yd * (1 + kappa1 * r²)
  CV_CALL( cvScale( eng.r_distorted_squared_, Xu2, kappa1, 1) ); // Xu_2 = 1 + kappa1 * r²
  CV_CALL( cvMul( Xu2, &Yd, Yu2, 1) ); // Yu_2 = Yd * Xu_2
  CV_CALL( cvMul( Xu2, &Xd, Xu2, 1) ); // Xu_2 = Xd * Xu_2

  /* record the error in the undistorted sensor coordinates */
  CV_CALL( cvSub( Xu1, Xu2, Xu1) );
  CV_CALL( cvMul( Xu1, Xu1, Xu1) );
  CV_CALL( cvSub( Yu1, Yu2, Yu1) );
  CV_CALL( cvMul( Yu1, Yu1, Yu1) );
  CV_CALL( cvAdd( Xu1, Yu1, _err_func) );

  __END__;

  cvReleaseMat(&xc);
  cvReleaseMat(&yc);
  cvReleaseMat(&zc);
  cvReleaseMat(&Xu1);
  cvReleaseMat(&Yu1);
  cvReleaseMat(&Xu2);
  cvReleaseMat(&Yu2);
  
  __ISL_CHECK_ERROR__;


}

void
TsaiEngine::full_optimization()
{
  CvMat* init_guess = 0;
  const CvMat* result = 0;

  Optimizer_Full optim(*this);

  CV_FUNCNAME( "TsaiEngine::full_optimization" );
  __BEGIN__;

  CV_CALL( init_guess = cvCreateMat(10, 1, CV_64F) );
  cvmSet(init_guess, 0, 0, this->result_.Rx);
  cvmSet(init_guess, 1, 0, this->result_.Ry);
  cvmSet(init_guess, 2, 0, this->result_.Rz);
  cvmSet(init_guess, 3, 0, this->result_.Tx);
  cvmSet(init_guess, 4, 0, this->result_.Ty);
  cvmSet(init_guess, 5, 0, this->result_.Tz);
  cvmSet(init_guess, 6, 0, this->result_.kappa1);
  cvmSet(init_guess, 7, 0, this->result_.f);
  cvmSet(init_guess, 8, 0, this->result_.Cx);
  cvmSet(init_guess, 9, 0, this->result_.Cy);


  std::cout << "Rx = " << this->result_.Rx << std::endl;
  std::cout << "Ry = " << this->result_.Ry << std::endl;
  std::cout << "Rz = " << this->result_.Rz << std::endl;
  std::cout << "Tx = " << this->result_.Tx << std::endl;
  std::cout << "Ty = " << this->result_.Ty << std::endl;
  std::cout << "Tz = " << this->result_.Tz << std::endl;
  std::cout << "k1 = " << this->result_.kappa1 << std::endl;
  std::cout << "f  = " << this->result_.f << std::endl;
  std::cout << "Cx = " << this->result_.Cx << std::endl;
  std::cout << "Cy = " << this->result_.Cy << std::endl;

  optim.init(this->nb_pt_, init_guess);

  this->compute_Xd_Yd_and_r_squared();
  
  optim.run(ISL_LMOPTIM_SCALE_JAC);

  result = optim.result();

  std::cout << "full_optimization : " <<  optim.nb_iter() << std::endl;

  this->result_.Rx     = cvmGet(result, 0, 0);
  this->result_.Ry     = cvmGet(result, 1, 0);
  this->result_.Rz     = cvmGet(result, 2, 0);
  
  this->apply_RPY_transform();

  this->result_.Tx     = cvmGet(result, 3, 0);
  this->result_.Ty     = cvmGet(result, 4, 0);
  this->result_.Tz     = cvmGet(result, 5, 0);
  this->result_.kappa1 = cvmGet(result, 6, 0);
  this->result_.f      = cvmGet(result, 7, 0);
  this->result_.Cx     = cvmGet(result, 8, 0);
  this->result_.Cy     = cvmGet(result, 9, 0);

  std::cout << "Rx = " << this->result_.Rx << std::endl;
  std::cout << "Ry = " << this->result_.Ry << std::endl;
  std::cout << "Rz = " << this->result_.Rz << std::endl;
  std::cout << "Tx = " << this->result_.Tx << std::endl;
  std::cout << "Ty = " << this->result_.Ty << std::endl;
  std::cout << "Tz = " << this->result_.Tz << std::endl;
  std::cout << "k1 = " << this->result_.kappa1 << std::endl;
  std::cout << "f  = " << this->result_.f << std::endl;
  std::cout << "Cx = " << this->result_.Cx << std::endl;
  std::cout << "Cy = " << this->result_.Cy << std::endl;

  __END__;
  cvReleaseMat(&init_guess);
  __ISL_CHECK_ERROR__;
}




}
