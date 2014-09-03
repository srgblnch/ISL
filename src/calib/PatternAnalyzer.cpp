//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#include "PatternAnalyzer.h"

#include <isl/blob/BlobList.h>
#include <isl/blob/BlobComparator.h>
#include <algorithm>
#include <sstream>

#include <isl/statistics/AccurateCentroid.h>
#include <isl/statistics/Moments.h>

#include "InternalErrorHandler.h"


#if !defined (__ISL_INLINE__)
# include "PatternAnalyzer.i"
#endif

//- #define DEBUG_PATTERN_ANALYZER

namespace isl {


class L2DistSquareFrom
{
public:
  L2DistSquareFrom(CvPoint2D32f _p) : p_(_p.x,_p.y) {};
  ~L2DistSquareFrom() {};

  bool operator()(const CvPoint2D32f& _pA, const CvPoint2D32f& _pB)
  {
    Point2D<double> pA = Point2D<double>(_pA.x, _pA.y);
    Point2D<double> pB = Point2D<double>(_pB.x, _pB.y);
    return(pA.dist_L2_square(p_) < pB.dist_L2_square(p_));
  };

private:
  Point2D<double> p_;
};

class LinfDistFrom
{
public:
  LinfDistFrom(CvPoint2D32f _p) : p_(_p.x,_p.y) {};
  ~LinfDistFrom() {};

  bool operator()(const CvPoint2D32f& _pA, const CvPoint2D32f& _pB)
  {
    Point2D<double> pA = Point2D<double>(_pA.x, _pA.y);
    Point2D<double> pB = Point2D<double>(_pB.x, _pB.y);
    return(pA.dist_Linf(p_) < pB.dist_Linf(p_));
  };

private:
  Point2D<double> p_;
};


bool operator==(const CvPoint2D32f& p, const CvPoint2D32f& q)
{
  return( p.x == q.x && p.y == q.y );
}


class IsInsideContour : public Filter
{
public:
  IsInsideContour(CvMat* contour)
  {
    this->contour_ = contour;
  };

  virtual ~IsInsideContour()
  {
  };

  virtual bool accept(Blob& _blob)
  {
    double result;

    CV_FUNCNAME( "IsInsideContour::accept" );
    __BEGIN__;
    
    CvPoint2D32f p = cvPoint2D32f(_blob.bounding_rect().center().x(), _blob.bounding_rect().center().y());
    CV_CALL( result = cvPointPolygonTest(contour_, p, 0) );
    
    __END__;
    __ISL_CHECK_ERROR__;

    return( result > 0 );
  }

private:
  const CvMat* contour_;
};


PatternAnalyzer::PatternAnalyzer()
: input_image_(0),
  preproc_image_(0),
  delaunay_subdiv_(0),
  delaunay_storage_(0),
  homography_mat(0),
  world_coord_(0),
  image_coord_(0)
{
  this->clean();
}

PatternAnalyzer::~PatternAnalyzer()
{
  this->clean();
}


void
PatternAnalyzer::clean()
{
  SAFE_DELETE(this->input_image_);
  SAFE_DELETE(this->preproc_image_);
  this->blob_list_.clear();
  this->calib_points_.clear();

  cvReleaseMemStorage(&this->delaunay_storage_);
  this->delaunay_subdiv_ = 0;

  ::memset(this->infinity_points_, 0, 3 * sizeof(CvPoint2D32f));
  ::memset(this->corners_,         0, 4 * 3 * sizeof(CvPoint2D32f));
  ::memset(this->corners_edge_,    0, 3 * sizeof(CvSubdiv2DEdge));

  cvReleaseMat(&this->homography_mat);
  cvReleaseMat(&this->world_coord_);
  cvReleaseMat(&this->image_coord_);
}


void
PatternAnalyzer::compute(const Image& _image, Calib::Config _config)
{
  CV_FUNCNAME( "PatternAnalyzer::compute" );
  __BEGIN__;
  
  //- return to the initialization state
  ISL_CALL( this->clean() );

  //- store the parameters
  this->config_ = _config;

  //- compute the correspondence
  ISL_CALL( this->preprocess_input(_image) );
  ISL_CALL( this->extract_blobs() );
  ISL_CALL( this->refine_position() );
  ISL_CALL( this->compute_delaunay_subdiv() );
  ISL_CALL( this->identify_infinity_points() );
  ISL_CALL( this->identify_3_corners() );
  ISL_CALL( this->find_pattern_size() );
  ISL_CALL( this->compute_homography() );
  ISL_CALL( this->find_correspondence() );
  
  __END__;
  __ISL_CHECK_ERROR__;
}


void
PatternAnalyzer::preprocess_input(const Image& _image)
{
  Calib::Config& config = this->config_;

  CV_FUNCNAME( "PatternAnalyzer::preprocess_input" );
  __BEGIN__;
  
  int block_size, dilate_size;

  //- make a raw copy of the input image
  ISL_CALL( this->input_image_ = new Image(_image) );

  //- make a copy of the given image to a 8bit unsigned image
  ISL_CALL( this->preproc_image_ = new Image(_image.width(), _image.height(), ISL_STORAGE_UCHAR) );
  
  if (_image.depth() == ISL_STORAGE_CHAR
      || _image.depth() == ISL_STORAGE_UCHAR
      || _image.depth() == ISL_STORAGE_SHORT
      || _image.depth() == ISL_STORAGE_USHORT
      || _image.depth() == ISL_STORAGE_LONG)
  {
    //- convert the image and scale it so as the maximal value is 255
    double max;
    CV_CALL( cvMinMaxLoc( _image.get_ipl_image(), 0, &max ) );
    ISL_CALL( _image.convert(*this->preproc_image_, 255. / max) );  
  }
  else
  {
    SAFE_DELETE(this->input_image_);
    SAFE_DELETE(this->preproc_image_);
    CV_ERROR(CV_StsUnsupportedFormat, "Input image must be of integer type")
  }
    
  //- smooth the image to remove some pixel noise before extracting contours
  ISL_CALL( this->preproc_image_->smooth_gaussian(config.smoothing_kernel_width) );

  //- equalize the histogram
  ISL_CALL( this->preproc_image_->equalize_hist() );

  //- adaptive threshold
  block_size = cvRound( MIN(_image.width(), _image.height()) * config.adthresh_window_size_factor ) | 1;
  ISL_CALL( this->preproc_image_->adaptive_threshold(255,
                                                    ISL_ADTHRESH_MEAN,
                                                    ISL_THRESH_BINARY,
                                                    block_size,
                                                    config.adthresh_offset) );

  //- dilate the result
  dilate_size = config.dilatation_width | 1; //- be sure to have an even number
  ISL_CALL( this->preproc_image_->morphology(ISL_MORPHO_DILATE,
                                            dilate_size, dilate_size,
                                            dilate_size / 2, dilate_size / 2,
                                            config.dilatation_shape,
                                            1) );
  
  CV_CALL( cvRectangle(this->preproc_image_->get_ipl_image(),
                       cvPoint(0,0),
                       cvPoint(_image.width() - 1, _image.height() - 1),
                       cvRealScalar(255)) );
  CV_CALL( cvRectangle(this->preproc_image_->get_ipl_image(),
                       cvPoint(1,1),
                       cvPoint(_image.width() - 2, _image.height() - 2),
                       cvRealScalar(255)) );
  
  __END__;
  __ISL_CHECK_ERROR__;
}



void
PatternAnalyzer::extract_blobs()
{
  Calib::Config& config = this->config_;

  Point2D<int>* roi = 0;
  Point2D<int>* roi_hull = 0;

  CV_FUNCNAME( "PatternAnalyzer::extract_blobs" );
  __BEGIN__;

  double median_area, area_low_thresh, area_high_thresh;
  int i;
  BlobList::iterator it;
  BlobList& blob_list = this->blob_list_;

  isl::Image contour_im(*this->preproc_image_);
   
  ISL_CALL( blob_list.detect(contour_im) );

  //- keep only blobs with more than 10 points describing the contour
  ISL_CALL( blob_list.filter(ContourNbPoint(), Greater(10)) );

  if (blob_list.empty())
    CV_ERROR(CV_StsInternal, "Could not find any blob in the image");

  //- test the ROI : is it null
  bool test_null_roi;
  test_null_roi = ( config.roi[0].x() | config.roi[0].y() 
                  | config.roi[1].x() | config.roi[1].y() 
                  | config.roi[2].x() | config.roi[2].y() 
                  | config.roi[3].x() | config.roi[3].y() ) == 0;
  
  if (test_null_roi == true)
  {
    //- remove the blobs touching the border
    ISL_CALL( blob_list.filter(MinX(), Greater(1)) );
    ISL_CALL( blob_list.filter(MaxX(), Lesser(contour_im.width() - 2)) );
    ISL_CALL( blob_list.filter(MinY(), Greater(1)) );
    ISL_CALL( blob_list.filter(MaxY(), Lesser(contour_im.height() - 2)) );
  }
  else
  {
    //- keep only the blobs which are inside the ROI
    
    //- first, find the convex hull of the roi point set
    CV_CALL( roi      = (Point2D<int>*)cvAlloc(4 * sizeof(roi[0])) );
    CV_CALL( roi_hull = (Point2D<int>*)cvAlloc(4 * sizeof(roi_hull[0])) );

    CvMat roi_mat = cvMat( 1, 4, CV_32SC2, roi );
    CvMat roi_hull_mat = cvMat( 1, 4, CV_32SC2, roi_hull );

    roi[0] = config.roi[0];
    roi[1] = config.roi[1];
    roi[2] = config.roi[2];
    roi[3] = config.roi[3];

    CV_CALL( cvConvexHull2( &roi_mat, &roi_hull_mat, CV_CLOCKWISE, 1) );

    ISL_CALL( blob_list.filter(IsInsideContour(&roi_hull_mat)) );
  }

  if (blob_list.empty())
    CV_ERROR(CV_StsInternal, "No blobs detected in the ROI");

  //- get the median area value
  std::partial_sort(blob_list.begin(),
                    blob_list.begin() + blob_list.size() / 2 + 1,
                    blob_list.end(),
                    BlobComparator(Area(), Lesser()));
  
  median_area = blob_list[blob_list.size() / 2 + 1].area();


  //- keep only blobs whose area are in [.5 * median_area , 1.5 * median_area]
  area_low_thresh  = (1 - config.median_area_max_deviation) * median_area;
  area_high_thresh = (1 + config.median_area_max_deviation) * median_area;

  ISL_CALL( blob_list.filter(Area(), Greater(area_low_thresh) ) );
  ISL_CALL( blob_list.filter(Area(), Lesser (area_high_thresh)) );

  this->calib_points_.reserve(blob_list.size());
  
#ifdef DEBUG_PATTERN_ANALYZER
  std::cout << std::endl;
  std::cout << "========================================" << std::endl;
  std::cout << "Blobs centers :" << std::endl;
#endif
  for ( it = blob_list.begin(), i = 0; it != blob_list.end(); it++, i++)
  {
    Point2D<double> c;
    
    if (config.centroid_rough_extraction == ISL_BOUNDING_RECT_CENTER)
    {
      c = (*it).bounding_rect().center();
    }
    else
    {
      c = (*it).ellipse().center();
    }
    this->calib_points_.push_back(cvPoint2D32f(c.x(), c.y()));

#ifdef DEBUG_PATTERN_ANALYZER
    std::cout << "[ " << c.x() << " , " << c.y() << " ]" << std::endl;
#endif
  }

  __END__;

  cvFree(&roi);
  cvFree(&roi_hull);

  __ISL_CHECK_ERROR__;
}


void
PatternAnalyzer::refine_position()
{
  Calib::Config& config = this->config_;
  Image* sub_image = 0;

  CV_FUNCNAME( "PatternAnalyzer::refine_position" );
  __BEGIN__;

  int i;
  std::vector<CvPoint2D32f>::iterator it;
  std::vector<CvPoint2D32f>& calib_points = this->calib_points_;
  AccurateCentroid centroid_finder;
  
  //- make a local copy of the vector of points for sorting purpose
  std::vector<CvPoint2D32f>  calib_points_tmp = calib_points;
  
#ifdef DEBUG_PATTERN_ANALYZER
  std::cout << std::endl;
  std::cout << "========================================" << std::endl;
  std::cout << "Blobs centers after refinement:" << std::endl;
#endif

  for ( it = calib_points.begin(), i = 0; it != calib_points.end(); it++, i++)
  {
    Point2D<float> cur_point = Point2D<float>((*it).x, (*it).y);
    Point2D<float> closest;
    double dist;

    std::partial_sort(calib_points_tmp.begin(),
                      calib_points_tmp.begin() + 2,
                      calib_points_tmp.end(),
                      LinfDistFrom(*it));

    closest = Point2D<float>(calib_points_tmp[1].x, calib_points_tmp[1].y);
    dist = cur_point.dist_Linf(closest);
    Point2D<float> origin = cur_point - Point2D<float>(float(dist / 2), float(dist / 2));

    Rectangle roi = Rectangle(cvRound(origin.x()),
                              cvRound(origin.y()),
                              cvRound(dist),
                              cvRound(dist));

    ISL_CALL( sub_image = this->input_image_->get_roi(roi) );
    ISL_CALL( sub_image->convert(ISL_STORAGE_FLOAT) );
    Point2D<double> centroid;
    
    if (config.refinement_method == ISL_TRUE_CENTROID 
        || config.refinement_method == ISL_SQUARED_CENTROID)
    {
      //- get the min-max value
      double min, max;
      CV_CALL( cvMinMaxLoc(sub_image->get_ipl_image(), &min, &max) );

      //- make a negative of the image so as the target point to have high intensity
      //- and the background to have low intensity
      CV_CALL( cvSubRS(sub_image->get_ipl_image(), 
                       cvRealScalar(max),
                       sub_image->get_ipl_image()) );

      if (config.refinement_method == ISL_SQUARED_CENTROID)
      {
        CV_CALL( cvMul(sub_image->get_ipl_image(),
                       sub_image->get_ipl_image(),
                       sub_image->get_ipl_image()) );
      }

      Moments2D m(*sub_image);
      centroid.x() = m.get_spatial_moment(1,0) / m.get_spatial_moment(0,0);
      centroid.y() = m.get_spatial_moment(0,1) / m.get_spatial_moment(0,0);
    }
    else if (config.refinement_method == ISL_ACCURATE_CENTROID)
    {
      ISL_CALL( centroid = centroid_finder.compute(*sub_image) );
    }
    else
    {
      SAFE_DELETE( sub_image );
      CV_ERROR(CV_StsBadArg, "The specified centroid refinement method is not supported");
    }


#ifdef DEBUG_PATTERN_ANALYZER
    {
      std::stringstream s;
      s << "sub\\sub_" << i << ".pgm";

      sub_image->normalize(255, 0, ISL_MINMAX);
      sub_image->convert(ISL_STORAGE_UCHAR);
      sub_image->save(s.str().c_str());
    }
#endif

    SAFE_DELETE( sub_image );

#ifdef DEBUG_PATTERN_ANALYZER
    std::cout << "[ " << (*it).x << " , " << (*it).y << " ] --> ";
#endif

    *it = cvPoint2D32f(roi.origin().x() + centroid.x(), roi.origin().y() + centroid.y());

#ifdef DEBUG_PATTERN_ANALYZER
    std::cout << "[ " << (*it).x << " , " << (*it).y << " ]" << std::endl;
#endif
  }

  __END__;
  SAFE_DELETE( sub_image ); //- in case an error occured in the middle
  __ISL_CHECK_ERROR__;
}



void
PatternAnalyzer::compute_delaunay_subdiv()
{
  size_t i;
  CvMemStorage* storage = 0;
  CvSubdiv2D*   subdiv = 0;
  std::vector<CvPoint2D32f>& calib_points = this->calib_points_;
  int w = this->preproc_image_->width();
  int h = this->preproc_image_->height();

  CV_FUNCNAME( "PatternAnalyzer::compute_delaunay_subdiv" );
  __BEGIN__;

  //- The Delaunay subdivision is initialized with a triangle enclosing
  //- the whole point set. Then each point of the point set is added
  //- sequentially to the subdivision.
  //- For that, we give to cvCreateSubdivDelaunay2D a rectangle enclosing
  //- the point set and OpenCV internally generates 3 points enclosing
  //- this rectangle. These 3 points are referred as the "infinity points" in the sequel.
  CvRect init_rect = cvRect(0, 0, w, h);
  
  CV_CALL( storage = cvCreateMemStorage(0) );
  CV_CALL( subdiv  = cvCreateSubdivDelaunay2D(init_rect, storage) );

  for (i = 0; i < calib_points.size(); i++)
  {
    CV_CALL( cvSubdivDelaunay2DInsert( subdiv, calib_points[i]) );
  }

  __END__;
  __ISL_CHECK_ERROR__;

  this->delaunay_storage_ = storage;
  this->delaunay_subdiv_ = subdiv;
}



void
PatternAnalyzer::identify_infinity_points()
{
  CV_FUNCNAME( "PatternAnalyzer::identify_infinity_points" );
  __BEGIN__;

  CvSeqReader  reader;
  CvSubdiv2D* subdiv = this->delaunay_subdiv_;
  int elem_size = subdiv->edges->elem_size;

  //- extract the "infinity points"

  //- The two first edges connect the three infinity points.
  //- The first infinity point is at the top-right of the calibration pattern
  //- The second one is at the bottom-left
  //- The third one is at the top-right

  CvQuadEdge2D* edge;
  CvSubdiv2DPoint* org_pt;
  CvSubdiv2DPoint* dst_pt;

  CV_CALL( cvStartReadSeq( (CvSeq*)(subdiv->edges), &reader, 0 ) );
  
  edge = (CvQuadEdge2D*)(reader.ptr);
  org_pt = cvSubdiv2DEdgeOrg((CvSubdiv2DEdge)edge);
  dst_pt = cvSubdiv2DEdgeDst((CvSubdiv2DEdge)edge);
  this->infinity_points_[0] = org_pt->pt;
  this->infinity_points_[1] = dst_pt->pt;
  
  CV_NEXT_SEQ_ELEM( elem_size, reader );
  
  edge = (CvQuadEdge2D*)(reader.ptr);
  // org_pt = cvSubdiv2DEdgeOrg((CvSubdiv2DEdge)edge);
  CV_CALL( dst_pt = cvSubdiv2DEdgeDst((CvSubdiv2DEdge)edge) );
  this->infinity_points_[2] = dst_pt->pt;

  __END__;
  __ISL_CHECK_ERROR__;
}



void
PatternAnalyzer::identify_3_corners()
{

  //- The aim of this method is to find 3 of the four corners 
  //- of the calibration pattern.
  //- The way to achieve this is dependent of the OpenCV implementation
  //- of Delaunay subdivision.
  //- When looking at the Delaunay subdivision with the 3 infinity points as
  //- defined by OpenCV, we see that 3 of the corners of the calibration pattern
  //- are connected to 2 infinity points, the last one being connected only 
  //- to the top-left infinity point.
  //-
  //- Thus the method employed is
  //- + iterating among the edges of the Delaunay subdivision
  //- + compare the vertex of the edge to the infinity point coordinates
  //-   to identify edges connecting a point of the calibration pattern
  //-   and an infinity point
  //- + identify the position of the corner point by knowing the position
  //-   of the infinity point
  //- + iterate around the corner to find the neighbors of the corner
  //-   and identify their relative position to the corner




  CV_FUNCNAME( "PatternAnalyzer::identify_3_corners" );
  __BEGIN__;

  CvSeqReader  reader;
  CvSubdiv2D* subdiv = this->delaunay_subdiv_;
  int total = subdiv->edges->total;
  int elem_size = subdiv->edges->elem_size;
  int i;

  CvPoint2D32f ppA = this->infinity_points_[0];
  CvPoint2D32f ppB = this->infinity_points_[1];
  CvPoint2D32f ppC = this->infinity_points_[2];
  
  CV_CALL( cvStartReadSeq( (CvSeq*)(subdiv->edges), &reader, 0 ) );

  for( i = 0; i < total; i++ )
  {
    CvQuadEdge2D* edge = (CvQuadEdge2D*)(reader.ptr);

    if( CV_IS_SET_ELEM( edge ))
    {
      CvSubdiv2DPoint* org_pt;
      CvSubdiv2DPoint* dst_pt;
      CvPoint2D32f org;
      CvPoint2D32f dst;

      //- is infinity point ?
      bool org_inf = false;
      bool dst_inf = false;

      CV_CALL( org_pt = cvSubdiv2DEdgeOrg((CvSubdiv2DEdge)edge) );
      CV_CALL( dst_pt = cvSubdiv2DEdgeDst((CvSubdiv2DEdge)edge) );

      
      if (!org_pt || !dst_pt)
      {
        CV_ERROR(CV_StsInternal, "Unrecoverable error in the Delaunay subdivision");
      }

      org = org_pt->pt;
      if (org == ppA || org == ppB || org == ppC)
        org_inf = true;

      dst = dst_pt->pt;
      if (dst == ppA || dst == ppB || dst == ppC)
        dst_inf = true;

      if (org_inf && !dst_inf || !org_inf && dst_inf)
      {
        //- one (and only one) extremity of the edge is on the border of the calibrator

        //- make the destination be the border point, and origin be the infinity point
        CvSubdiv2DEdge well_oriented_edge = org_inf ? (CvSubdiv2DEdge)edge : cvSubdiv2DSymEdge((CvSubdiv2DEdge)edge);

        //- update origin/destination if edge is swapped
        if (dst_inf)
        {
          org = cvSubdiv2DEdgeOrg(well_oriented_edge)->pt;
          dst = cvSubdiv2DEdgeDst(well_oriented_edge)->pt;
        }

        //- browse adjacent edges to see if there is another infinity point
        //- turn around the destination (clockwise)
        CvSubdiv2DEdge next = cvSubdiv2DGetEdge( well_oriented_edge, CV_NEXT_AROUND_DST );
        CvPoint2D32f next_org = cvSubdiv2DEdgeOrg(next)->pt;

        if (next_org == ppA || next_org == ppB || next_org == ppC )
        {
          //- this edge vertex is necessarily connected to 2 different infinity points
          //- --> this is a corner point

          //- locate the corner
          int corner_idx = -1;
          
          if (org == ppA && next_org == ppB)
            corner_idx = BOTTOM_RIGHT;
          else if (org == ppB && next_org == ppC)
            corner_idx = BOTTOM_LEFT;
          else if (org == ppC && next_org == ppA)
            corner_idx = UP_RIGHT;
          else
            CV_ERROR(CV_StsInternal, "Unrecoverable error in the Delaunay subdivision");

          this->corners_[corner_idx][0] = cvSubdiv2DEdgeDst(next)->pt;
          this->corners_edge_[corner_idx] = well_oriented_edge; // save the edge pointer, with origin at infinity

          //- count the number of other edges ( = number of direct neighbors in delaunay triangulation)
          CvSubdiv2DEdge cur_edge = cvSubdiv2DGetEdge(next, CV_NEXT_AROUND_DST);
          int nb_adjacent = 0;
          while ( cur_edge != well_oriented_edge )
          {
            nb_adjacent++;
            this->corners_[corner_idx][nb_adjacent] = cvSubdiv2DEdgeOrg(cur_edge)->pt;
            cur_edge = cvSubdiv2DGetEdge(cur_edge, CV_NEXT_AROUND_DST);
          }

          if (nb_adjacent == 2)
          {
            //- reorder the neighbors so that they are organized clockwise
            //- and begin by the corner point
            this->corners_[corner_idx][3] = this->corners_[corner_idx][2];
            this->corners_[corner_idx][2] = cvPoint2D32f(0,0);

            //- find the last neighbor of the corner
            CvPoint2D32f corner    = this->corners_[corner_idx][0];
            CvPoint2D32f neighbor1 = this->corners_[corner_idx][1];
            CvPoint2D32f neighbor2 = this->corners_[corner_idx][3];
            CvPoint2D32f theoric_neighbor = cvPoint2D32f( neighbor1.x + neighbor2.x - corner.x,
                                                          neighbor1.y + neighbor2.y - corner.y);

            //- find the calibration point closest to 'theoric_neighbor'
            std::partial_sort(this->calib_points_.begin(),
                              this->calib_points_.begin() + 1,
                              this->calib_points_.end(),
                              L2DistSquareFrom(theoric_neighbor));
            
            this->corners_[corner_idx][2] = this->calib_points_[0];

          }
          else if(nb_adjacent == 3)
          {
            //- nothing to do
          }
          else
          {
            CV_ERROR(CV_StsInternal, "The corner points of the calibration pattern could not be found");
          }

          corner_idx++;
        }
      }
    }

    CV_NEXT_SEQ_ELEM( elem_size, reader );
  }

  __END__;
  __ISL_CHECK_ERROR__;
}



void
PatternAnalyzer::find_pattern_size()
{
  std::vector<CvPoint2D32f>& calib_points = this->calib_points_;
  int width, height;

  //- The pattern size is found by iterating around 2 infinity points :
  //-  + iterating around the first infinity point gives the height
  //-  + iterating around the second infinity point gives the width

  CV_FUNCNAME( "PatternAnalyzer::find_pattern_size" );
  __BEGIN__;

  CvSubdiv2DEdge e, e0;
  //- corners_edge_[BOTTOM_RIGHT] connects the bottom right corner to ppA
  e0 = this->corners_edge_[BOTTOM_RIGHT];
  e = e0;
  height = 0;
  //- turn around ppA
  do
  {
    height++;
    e = cvSubdiv2DGetEdge(e, CV_NEXT_AROUND_ORG);
  } while (e != e0);

  //- substract by 2 for the two edges [ppA ppC] and [ppA ppB]
  height -= 2;
  this->calibrator_size_.height = height;


  //- corners_edge_[BOTTOM_LEFT] connects the bottom left corner to ppB
  e0 = this->corners_edge_[BOTTOM_LEFT];
  e = e0;
  width = 0;
  //- turn around ppB
  do
  {
    width++;
    e = cvSubdiv2DGetEdge(e, CV_NEXT_AROUND_ORG);
  } while (e != e0);

  //- substract by 2 for the two edges [ppB ppA] and [ppB ppC]
  width -= 2;
  this->calibrator_size_.width = width;  
  
  //- verify that we are coherent
  int nb_calib_points = static_cast<int>(calib_points.size());
  if (height * width != nb_calib_points)
  {
    CV_ERROR( CV_StsInternal, "The size of the calibration pattern could not be found" );
  }


  __END__;
  __ISL_CHECK_ERROR__;
}



void
PatternAnalyzer::compute_homography()
{
  //- This method computes a first 'rough' homography, in the least square sense,
  //- with the previously identified correspondences.
  //- It will then be used to estimate the location of the other calibration
  //- points

  CvMat* homog_src_pt = 0;
  CvMat* homog_dst_pt = 0;
  CvMat* homog_mat = 0;
  
  CV_FUNCNAME( "PatternAnalyzer::compute_homography" );
  __BEGIN__;

  double o = 5.32; //- just an arbitrary offset to avoid (0,0) coordinates
                   //- when solving the least square system
                   //- for Tsai's calibration algorithm, it is recommended...
  int w, h;
  double dx, dy;
  int n;

  int count;
  std::vector<CvPoint2D32f> world_pt;
  std::vector<CvPoint2D32f> image_pt;

  //- find the homography that best fit the already
  //- determined corner points, their neighbors, and the edge points
  w = this->calibrator_size_.width;
  h = this->calibrator_size_.height;

  dx = this->config_.x_spacing;
  dy = this->config_.y_spacing;

  //- add the last known correspondence : the neighbors of corner points
  image_pt.push_back(this->corners_[UP_RIGHT][0]);
  world_pt.push_back(cvPoint2D32f((w - 1) * dx + o, o));
  image_pt.push_back(this->corners_[UP_RIGHT][1]);
  world_pt.push_back(cvPoint2D32f((w - 1) * dx + o, dy + o));
  image_pt.push_back(this->corners_[UP_RIGHT][2]);
  world_pt.push_back(cvPoint2D32f((w - 2) * dx + o, dy + o));
  image_pt.push_back(this->corners_[UP_RIGHT][3]);
  world_pt.push_back(cvPoint2D32f((w - 2) * dx + o, o));

  image_pt.push_back(this->corners_[BOTTOM_RIGHT][0]);
  world_pt.push_back(cvPoint2D32f((w - 1) * dx + o, (h - 1) * dy + o));
  image_pt.push_back(this->corners_[BOTTOM_RIGHT][1]);
  world_pt.push_back(cvPoint2D32f((w - 2) * dx + o, (h - 1) * dy + o));
  image_pt.push_back(this->corners_[BOTTOM_RIGHT][2]);
  world_pt.push_back(cvPoint2D32f((w - 2) * dx + o, (h - 2) * dy + o));
  image_pt.push_back(this->corners_[BOTTOM_RIGHT][3]);
  world_pt.push_back(cvPoint2D32f((w - 1) * dx + o, (h - 2) * dy + o));

  image_pt.push_back(this->corners_[BOTTOM_LEFT][0]);
  world_pt.push_back(cvPoint2D32f(o, (h - 1) * dy + o));
  image_pt.push_back(this->corners_[BOTTOM_LEFT][1]);
  world_pt.push_back(cvPoint2D32f(o, (h - 2) * dy + o));
  image_pt.push_back(this->corners_[BOTTOM_LEFT][2]);
  world_pt.push_back(cvPoint2D32f(dx + o, (h - 2) * dy + o));
  image_pt.push_back(this->corners_[BOTTOM_LEFT][3]);
  world_pt.push_back(cvPoint2D32f(dx + o, (h - 1) * dy + o));

  n = static_cast<int>(image_pt.size());

  CV_CALL( homog_src_pt = cvCreateMat(n, 2, CV_64F) );
  CV_CALL( homog_dst_pt = cvCreateMat(n, 2, CV_64F) );
  CV_CALL( homog_mat    = cvCreateMat(3, 3, CV_64F) );

  for (count = 0; count < n; count++)
  {
    CvPoint2D32f wpt = world_pt[count];
    CvPoint2D32f ipt = image_pt[count];
  
    cvmSet(homog_src_pt, count, 0, wpt.x);
    cvmSet(homog_src_pt, count, 1, wpt.y);
    cvmSet(homog_dst_pt, count, 0, ipt.x);
    cvmSet(homog_dst_pt, count, 1, ipt.y);
  }

  //- solve the homography
  CV_CALL( cvFindHomography(homog_src_pt, homog_dst_pt, homog_mat) );

  this->homography_mat = homog_mat;
  
  __END__;

  cvReleaseMat(&homog_src_pt);
  cvReleaseMat(&homog_dst_pt);

  __ISL_CHECK_ERROR__;
}


void
PatternAnalyzer::find_correspondence()
{
  //- With the previously computed homography, done with calibration points
  //- for which we knew the location on the pattern, we can now find the 
  //- position of the other calibration points by applying the homography to them
  //- and finding the closest point to the projection

  CvMat* world_coord = 0;
  CvMat* image_coord = 0;
  CvMat* homog_src = 0;
  CvMat* homog_dst = 0;

  CV_FUNCNAME( "PatternAnalyzer::find_correspondence" );
  __BEGIN__;

  double o = 5.32;
  int w = this->calibrator_size_.width;
  int h = this->calibrator_size_.height;
  int n = w * h;
  double dx = this->config_.x_spacing;
  double dy = this->config_.y_spacing;

  int i,j;

  CvMat* homog_mat = this->homography_mat;
  std::vector<CvPoint2D32f>& calib_points = this->calib_points_;

  //- allocate space for the coordinates
  CV_CALL( world_coord = cvCreateMat(n, 3, CV_64F) );
  CV_CALL( image_coord = cvCreateMat(n, 2, CV_64F) );

  //- compute coordinates of calibrator points

  //- temporary storage for homography transformation
  CV_CALL( homog_src = cvCreateMat(3, 1, CV_64F) );
  CV_CALL( homog_dst = cvCreateMat(3, 1, CV_64F) );


  for ( i = 0; i < w; i++)
  {
    for (j = 0; j < h; j++)
    {
      //- world coordinate
      CvPoint2D32f wpt = cvPoint2D32f(i * dx + o, j * dy + o);

      cvmSet(world_coord, i * h + j, 0, wpt.x);
      cvmSet(world_coord, i * h + j, 1, wpt.y);
      cvmSet(world_coord, i * h + j, 2, 0 );


      //- project the world point with the homography
      cvmSet(homog_src, 0, 0, wpt.x);
      cvmSet(homog_src, 1, 0, wpt.y);
      cvmSet(homog_src, 2, 0, 1 );

      cvMatMul(homog_mat, homog_src, homog_dst);

      double homog_dst_x = cvmGet(homog_dst, 0, 0);
      double homog_dst_y = cvmGet(homog_dst, 1, 0);
      double homog_dst_w = cvmGet(homog_dst, 2, 0);

      //- image coordinate of the projection
      CvPoint2D32f candidate = cvPoint2D32f( homog_dst_x / homog_dst_w, homog_dst_y / homog_dst_w);


      //- among calib_points, find the closer to 'candidate'
      std::partial_sort(calib_points.begin(),
                        calib_points.begin() + 1,
                        calib_points.end(),
                        L2DistSquareFrom(candidate));

      CvPoint2D32f ipt = calib_points[0];
      cvmSet(image_coord, i * h + j, 0, ipt.x);
      cvmSet(image_coord, i * h + j, 1, ipt.y);

    }
  }

  cvReleaseMat(&homog_src);
  cvReleaseMat(&homog_dst);

  this->world_coord_ = world_coord;
  this->image_coord_ = image_coord;

  __END__;

  cvReleaseMat(&homog_src);
  cvReleaseMat(&homog_dst);

  __ISL_CHECK_ERROR__;
}



void
PatternAnalyzer::draw_delaunay(Image& img, double intensity, int thickness, int line_type) const
{
  CvSeqReader  reader;
  IplImage* out_img = 0;
  CvSubdiv2D* subdiv = 0;
  int total;
  int elem_size;
  int i;

  CV_FUNCNAME( "PatternAnalyzer::draw_delaunay" );
  __BEGIN__;

  out_img = img.get_ipl_image();

  subdiv = this->delaunay_subdiv_;
  if (subdiv == 0)
    CV_ERROR(CV_StsInternal, "The Delaunay subdivision has not yet been computed");

  total = subdiv->edges->total;
  elem_size = subdiv->edges->elem_size;

  CV_CALL( cvStartReadSeq( (CvSeq*)(subdiv->edges), &reader, 0 ) );

  for( i = 0; i < total; i++ )
  {
      CvQuadEdge2D* edge = (CvQuadEdge2D*)(reader.ptr);

      if( CV_IS_SET_ELEM( edge ))
      {
        CvSubdiv2DPoint* org_pt;
        CvSubdiv2DPoint* dst_pt;
        CvPoint2D32f org;
        CvPoint2D32f dst;
        CvPoint iorg, idst;

        org_pt = cvSubdiv2DEdgeOrg((CvSubdiv2DEdge)edge);
        dst_pt = cvSubdiv2DEdgeDst((CvSubdiv2DEdge)edge);

        if( org_pt && dst_pt )
        {
            org = org_pt->pt;
            dst = dst_pt->pt;

            iorg = cvPoint( cvRound( org.x ), cvRound( org.y ));
            idst = cvPoint( cvRound( dst.x ), cvRound( dst.y ));

            CV_CALL( cvLine( out_img, iorg, idst, cvRealScalar(intensity), thickness, line_type) );
        }
      }

      CV_NEXT_SEQ_ELEM( elem_size, reader );
  }

  __END__;
  __ISL_CHECK_ERROR__;
}


void
PatternAnalyzer::draw_blobs(Image& img, double intensity, int thickness, int line_type)
{
  CV_FUNCNAME( "PatternAnalyzer::draw_delaunay" );
  __BEGIN__;

  ISL_CALL( this->blob_list_.draw(img, intensity, thickness, line_type) );

  __END__;
  __ISL_CHECK_ERROR__;
}



}
