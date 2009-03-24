#include "PerspEngine.h"

#include "InternalErrorHandler.h"


#if !defined (__ISL_INLINE__)
# include "PerspEngine.i"
#endif


namespace isl {

PerspEngine::PerspEngine()
: hom_w2i_(0),
  hom_i2w_(0),
  new_world_coord_(0),
  error_map_(0)
{
}

PerspEngine::~PerspEngine()
{
  cvReleaseMat(&this->hom_w2i_);
  cvReleaseMat(&this->hom_i2w_);
  cvReleaseMat(&this->new_world_coord_);
  cvReleaseMat(&this->error_map_);
}

void
PerspEngine::learn(const PatternAnalyzer& _pc)
{
  const Image& im          = _pc.preprocessed_image();
  const CvMat* world_coord = _pc.world_coord();
  const CvMat* image_coord = _pc.image_coord();

  CvMat* hom_w2i = 0;
  CvMat* hom_i2w = 0;
  CvMat* hom_w2i_s = 0;
  CvMat* hom_i2w_s = 0;

  CvMat* world_coord_s = 0;
  CvMat* ideal_world_coord_s = 0;
  CvMat* observed_world_coord_s = 0;
  CvMat* error_map = 0;
  CvMat* error_norm = 0;

  int w = im.width();
  int h = im.height();
  
  int n = world_coord->rows;

  double xmag = 0;
  double ymag = 0;

  CV_FUNCNAME( "PerspEngine::learn" );
  __BEGIN__;

  CV_CALL( hom_w2i = cvCreateMat(3, 3, CV_64F) );
  CV_CALL( cvFindHomography(world_coord, image_coord, hom_w2i) );

  //- compute the inverse model homography
  CV_CALL( hom_i2w = cvCreateMat(3, 3, CV_64F) );
  CV_CALL( cvInvert( hom_w2i, hom_i2w, CV_SVD ) );

  //- back-project the corners of 'im' into the world space
  //- by applying the inverse homography
  CvPoint2D64f corners_i[4];
  CvPoint2D64f corners_w[4];

  corners_i[0] = cvPoint2D64f(0, 0);
  corners_i[1] = cvPoint2D64f(w - 1, 0);
  corners_i[2] = cvPoint2D64f(w - 1, h - 1);
  corners_i[3] = cvPoint2D64f(0, h - 1);

  CvMat corners_i_mat, corners_w_mat;

  CV_CALL( corners_i_mat = cvMat(4, 1, CV_64FC2, corners_i) );
  CV_CALL( corners_w_mat = cvMat(4, 1, CV_64FC2, corners_w) );
  
  CV_CALL( cvPerspectiveTransform(&corners_i_mat, &corners_w_mat, hom_i2w) );



  //- compute bounding box of corner point in the world coordinate system
  double xmin,xmax,ymin,ymax;
  int i;
  CvPoint2D64f pt;
  xmin = xmax = corners_w[0].x;
  ymin = ymax = corners_w[0].y;

  for (i = 1; i < 4; i++)
  {
    pt = corners_w[i];
    if (xmin > pt.x)
      xmin = pt.x;
    if (xmax < pt.x)
      xmax = pt.x;
    if (ymin > pt.y)
      ymin = pt.y;
    if (ymax < pt.y)
      ymax = pt.y;
  }

  xmag = (xmax - xmin) / w;
  ymag = (ymax - ymin) / h;

  //- map : [xmin, ymin] on [0, 0]
  //-       (xmax-xmin) on width
  //-       (ymax-ymin) on height
  CvPoint2D64f corners_w_scaled[4];
  corners_w_scaled[0].x = (corners_w[0].x - xmin) / xmag;
  corners_w_scaled[0].y = (corners_w[0].y - ymin) / ymag;

  corners_w_scaled[1].x = (corners_w[1].x - xmin) / xmag;
  corners_w_scaled[1].y = (corners_w[1].y - ymin) / ymag;

  corners_w_scaled[2].x = (corners_w[2].x - xmin) / xmag;
  corners_w_scaled[2].y = (corners_w[2].y - ymin) / ymag;

  corners_w_scaled[3].x = (corners_w[3].x - xmin) / xmag;
  corners_w_scaled[3].y = (corners_w[3].y - ymin) / ymag;



  //- find the corresponding homography world->image
  CV_CALL( hom_w2i_s = cvCreateMat(3, 3, CV_64F) );
  CV_CALL( hom_i2w_s = cvCreateMat(3, 3, CV_64F) );

  CvMat corners_w_s_mat;
  CV_CALL( corners_w_s_mat = cvMat(4, 2, CV_64F, corners_w_scaled) );

  CV_CALL( cvFindHomography(&corners_w_s_mat, &corners_i_mat, hom_w2i_s) );
  CV_CALL( cvInvert(hom_w2i_s, hom_i2w_s, CV_SVD) );



  //- compute error & error map
  //--------------------------------------------------------------------------------

  //- project all image points wrt the homography
  CvMat image_coord_persp;
  CV_CALL( cvReshape(image_coord, &image_coord_persp, 2, n) );
  
  CV_CALL( observed_world_coord_s = cvCreateMat(n, 1, CV_64FC2) );
  CV_CALL( cvPerspectiveTransform(&image_coord_persp, observed_world_coord_s, hom_i2w_s) );
  CV_CALL( cvReshape(observed_world_coord_s, observed_world_coord_s, 1, n) );

  CV_CALL( error_map    = cvCreateMat(n, 2, CV_64F) ); //- the components of the error per calibration point
  CV_CALL( error_norm   = cvCreateMat(n, 1, CV_64F) ); //- the norm of the error

  CV_CALL( ideal_world_coord_s = cvCloneMat(world_coord) );
  CvMat xw_s, yw_s, xy;
  CV_CALL( cvGetCol(ideal_world_coord_s, &xw_s, 0) );
  CV_CALL( cvGetCol(ideal_world_coord_s, &yw_s, 1) );
  CV_CALL( cvGetCols(ideal_world_coord_s, &xy, 0, 2) );
  CV_CALL( cvConvertScale(&xw_s, &xw_s, w / (xmax - xmin), - w * xmin / (xmax - xmin) ) );
  CV_CALL( cvConvertScale(&yw_s, &yw_s, h / (ymax - ymin), - h * ymin / (ymax - ymin) ) );

  CV_CALL( cvSub(observed_world_coord_s, &xy, error_map) );
  CvMat error_map_x, error_map_y;
  CV_CALL( cvGetCol(error_map, &error_map_x, 0) );
  CV_CALL( cvGetCol(error_map, &error_map_y, 1) );
  CV_CALL( cvCartToPolar(&error_map_x, &error_map_y, error_norm) );

  double model_error_pix,
         x_model_error,
         y_model_error;

  CV_CALL( model_error_pix = cvNorm(error_map) );
  model_error_pix /= n;
  CV_CALL( x_model_error = cvNorm(&error_map_x) );
  x_model_error /= n;
  CV_CALL( y_model_error = cvNorm(&error_map_y) );
  y_model_error /= n;

  this->hom_w2i_         = hom_w2i_s;
  this->hom_i2w_         = hom_i2w_s;
  //-  this->mag_factor_      = (xmag / x_model_error + ymag / y_model_error) / (1 / x_model_error + 1 / y_model_error );
  this->x_mag_factor_    = xmag;
  this->y_mag_factor_    = ymag;
  this->new_world_coord_ = observed_world_coord_s;
  this->error_map_       = error_map;
  this->model_error_     = ::sqrt(xmag * xmag * x_model_error * x_model_error + ymag * ymag * y_model_error * y_model_error);

  __END__;

  cvReleaseMat(&hom_w2i);
  cvReleaseMat(&hom_i2w);
  cvReleaseMat(&world_coord_s);
  cvReleaseMat(&ideal_world_coord_s);

  __ISL_CHECK_ERROR__;
}

void
PerspEngine::undistort(const Image& _src, Image& _dst, int interp) const
{

  CV_FUNCNAME( "PerspEngine::undistort" );
  __BEGIN__;

  CV_CALL( cvWarpPerspective(_src.get_ipl_image(), 
                             _dst.get_ipl_image(),
                             this->hom_w2i_,
                             interp + CV_WARP_FILL_OUTLIERS + CV_WARP_INVERSE_MAP) );


  __END__;
  __ISL_CHECK_ERROR__;
}

void
PerspEngine::draw_error_map(Image& _dst, double _scale, double _intensity, int _thickness, int _line_type) const
{
  CV_FUNCNAME( "PerspEngine::draw_error_map" );
  __BEGIN__;

  const CvMat* world_c    = this->new_world_coord_;
  const CvMat* error_map  = this->error_map_;
  int n = world_c->rows;
  int i;

  for (i = 0; i < n; i++)
  {
    CvPoint2D64f ori, dst;
    CvPoint orip, dstp;

    ori.x = cvmGet(world_c, i, 0) - cvmGet(error_map, i, 0);
    ori.y = cvmGet(world_c, i, 1) - cvmGet(error_map, i, 1);

    dst.x = ori.x + _scale * cvmGet(error_map, i, 0);
    dst.y = ori.y + _scale * cvmGet(error_map, i, 1);

    orip.x = cvRound(ori.x);
    orip.y = cvRound(ori.y);
    dstp.x = cvRound(dst.x);
    dstp.y = cvRound(dst.y);

    CV_CALL( cvLine(_dst.get_ipl_image(), orip, dstp, cvScalar(_intensity), _thickness, _line_type) );
  }

  __END__;
  __ISL_CHECK_ERROR__;
}


}
