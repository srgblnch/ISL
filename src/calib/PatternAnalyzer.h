//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#ifndef __INCLUDE_PATTERNANALYZER_H__
#define __INCLUDE_PATTERNANALYZER_H__

#include <isl/Image.h>
#include <isl/blob/BlobList.h>
#include <vector>
#include <isl/calib/Calib.h> //- for the config

namespace isl {


/**
 *  \addtogroup calib
 *  @{
 */

/**
 *  \addtogroup intern_calib
 *  @{
 */


/**
 *  \brief Class for analyzing a calibration pattern image
 *       
 *  Given a calibration pattern image, this class :
 *  \li Preprocesses the input image to binarize it
 *  \li Extracts blobs from the preprocessed image to find a rough position of
 *      the calibration points
 *  \li Finds the center of each blob with an accurate method giving sub-pixel accuracy
 *  \li Computes the Delaunay subdivision of the point set formed by all the centers
 *  \li Identifies 3 corners of the pattern
 *  \li Finds the pattern size (find M and N such that the pattern contains M
 *      lines and N columns)
 *  \li Computes the homography which maps the corners and some of their neighbors
 *      to the corner of the non distorted pattern
 *  \li Finds the correspondence between image points and real-world calibration points
 *      using the previously computed homography
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class PatternAnalyzer
{
  public:
    /**
     *  \brief Constructor
     */
    PatternAnalyzer(void);
    
    /**
     *  \brief Destructor
     */
    ~PatternAnalyzer();

    /**
     *  \brief Main function for analyzing a calibration pattern image
     *  \param[in] image the calibration pattern image
     *  \param[in] config the calibration config parameters
     *  \exception Exception
     */
    void
      compute(const Image& image, Calib::Config config);

    /**
     *  \brief Accessor for the number of calibration points in the Y direction
     */
    int
      calibrator_height() const;
    
    /**
     *  \brief Accessor for the number of calibration points in the X direction
     */
    int
      calibrator_width() const;

    /**
     *  \brief Accessor for the total number of calibration points
     */
    int
      nb_pt() const;

    /**
     *  \brief Accessor for the matrix of real-world coordinates (N x 3)
     *
     *  Actually only coplanar calibration pattern are supported, thus the third 
     *  column will always be null
     */
    const CvMat*
      world_coord() const;
    
    /**
     *  \brief Accessor for the matrix of image coordinates (N x 2)
     */
    const CvMat*
      image_coord() const;

    /**
     *  \brief Accessor for the preprocessed image
     */
    const Image&
      preprocessed_image() const;
    
    /**
     *  \brief Accessor for the detected blob list
     */
    BlobList&
      blob_list();

    /**
     *  \brief Draws the detected blobs on an image
     */
    void
      draw_blobs(Image& img, double intensity, int thickness = 1, int line_type = ISL_LINETYPE_4);

    /**
     *  \brief Draws the Delaunay triangulation of the calibration 
     *         points obtained during processing
     */
    void
      draw_delaunay(Image& img, double intensity, int thickness = 1, int line_type = ISL_LINETYPE_4) const;

  private:

    void
      preprocess_input(const Image& _image);

    void
      extract_blobs();

    void
      refine_position();
    
    Point2D<double>
      compute_true_centroid(const Image& im);
    
    Point2D<double>
      compute_accurate_centroid(const Image& im);

    void
      compute_delaunay_subdiv();

    void
      identify_infinity_points();

    void
      identify_3_corners();

    void
      find_pattern_size();

    void
      compute_homography();

    void
      find_correspondence();

    void
      clean();
    
    Calib::Config config_;

    Image* input_image_;
    Image* preproc_image_;

    BlobList blob_list_;

    std::vector<CvPoint2D32f> calib_points_;

    CvSubdiv2D*   delaunay_subdiv_;
    CvMemStorage* delaunay_storage_;

    CvPoint2D32f   infinity_points_ [3];

    enum CORNER_LOCATION {UP_RIGHT = 0, BOTTOM_RIGHT = 1, BOTTOM_LEFT = 2};
    CvPoint2D32f   corners_ [3][4];
    CvSubdiv2DEdge corners_edge_ [3];

    CvSize calibrator_size_;
    CvMat* homography_mat; //-   3 * 3 matrix

    CvMat* world_coord_; //-   N * 3 matrix
    CvMat* image_coord_; //-   N * 2 matrix

};
/** @} */
/** @} */

}

#if defined (__ISL_INLINE__)
# include "PatternAnalyzer.i"
#endif // __ISL_INLINE__


#endif
