#ifndef __INCLUDE_ISLCONFIG_H__
#define __INCLUDE_ISLCONFIG_H__

//-----------------------------------------------------------------------------
// GENERIC INCLUDE
//-----------------------------------------------------------------------------
#include <cv.h>
#include <highgui.h>

//-----------------------------------------------------------------------------
// WINDOWS PRAGMA
//-----------------------------------------------------------------------------
#if defined (WIN32)
# pragma warning (disable : 4786)
#endif

//-----------------------------------------------------------------------------
// DEBUG
//-----------------------------------------------------------------------------
#if defined(WIN32) 
# if defined(_DEBUG)
#   define ISL_DEBUG
# else
#   undef ISL_DEBUG
# endif
#else
# if defined(DEBUG)
#   define ISL_DEBUG
# else
#   undef ISL_DEBUG
# endif
#endif

//-----------------------------------------------------------------------------
// EXPORT
//-----------------------------------------------------------------------------
#if defined(WIN32) 
# if defined(ISL_DLL)
#   define ISL_DECL_EXPORT __declspec(dllexport)
#   define ISL_DECL_IMPORT __declspec(dllimport)
#   if defined (ISL_BUILD)
#     define ISL_DECL ISL_DECL_EXPORT
#   else
#     define ISL_DECL ISL_DECL_IMPORT
#   endif
# else
#   define ISL_DECL_EXPORT
#   define ISL_DECL_IMPORT
#   define ISL_DECL
# endif
#else
#   define ISL_DECL_EXPORT
#   define ISL_DECL_IMPORT
#   define ISL_DECL
#endif

#ifdef WIN32
  #define EXPORT_STL_VECTOR( data_type ) \
    template class ISL_DECL std::allocator< data_type >; \
    template class ISL_DECL std::vector<data_type, std::allocator< data_type > >;
#else
  #define EXPORT_STL_VECTOR( data_type )
#endif

//-----------------------------------------------------------------------------
// INLINE
//-----------------------------------------------------------------------------
#if !defined(ISL_DEBUG)
# define __ISL_INLINE__
# define ISL_INLINE inline
#else
# undef __ISL_INLINE__
# define ISL_INLINE
#endif


//-----------------------------------------------------------------------------
// SAFE DELETE macros
//-----------------------------------------------------------------------------
#define SAFE_DELETE(ptr)     if (ptr != 0) { delete ptr; ptr = 0; } else (void)0
#define SAFE_DELETE_TAB(ptr) if (ptr != 0) { delete [] ptr; ptr = 0; } else (void)0


namespace isl {

/**
 *  \defgroup constants Constants
 *  @{
 */

/**
 *  \defgroup storage Storage Types
 *  \brief The different storage type of elements of an image
 *  @{
 */
const int ISL_STORAGE_CHAR      = IPL_DEPTH_8S;  ///<   signed  8-bit integers
const int ISL_STORAGE_UCHAR     = IPL_DEPTH_8U;  ///< unsigned  8-bit integers
const int ISL_STORAGE_SHORT     = IPL_DEPTH_16S; ///<   signed 16-bit integers
const int ISL_STORAGE_USHORT    = IPL_DEPTH_16U; ///< unsigned 16-bit integers
const int ISL_STORAGE_LONG      = IPL_DEPTH_32S; ///<   signed 32-bit integers
const int ISL_STORAGE_ULONG     = IPL_DEPTH_32S; ///< unsigned 32-bit integers
const int ISL_STORAGE_FLOAT     = IPL_DEPTH_32F; ///< single precision floating-point numbers
const int ISL_STORAGE_DOUBLE    = IPL_DEPTH_64F; ///< double precision floating-point numbers
/** @} */

/**
 *  \defgroup morpho Morphology
 *  \brief Constants related to mathematical morphology
 *  @{
 */

/**
 *  \defgroup morpho_op Morphological Operations
 *  \brief The different morphological operations
 *  @{
 */
const int ISL_MORPHO_ERODE      = 0; ///< Erosion
const int ISL_MORPHO_DILATE     = 1; ///< Dilation
const int ISL_MORPHO_OPEN       = CV_MOP_OPEN; ///< Opening
const int ISL_MORPHO_CLOSE      = CV_MOP_CLOSE; ///< Closing
const int ISL_MORPHO_GRADIENT   = CV_MOP_GRADIENT; ///< Morphological Gradient
const int ISL_MORPHO_TOPHAT     = CV_MOP_TOPHAT; ///< Top Hat transform
const int ISL_MORPHO_BLACKHAT   = CV_MOP_BLACKHAT; ///< Black Hat transform
/** @} */

/**
 *  \defgroup morpho_shape Morphological Structuring Element Shapes
 *  \brief The possible shapes for a morphological structuring element
 *  @{
 */
const int ISL_SHAPE_RECT        = CV_SHAPE_RECT; ///< Rectangular Shape
const int ISL_SHAPE_CROSS       = CV_SHAPE_CROSS; ///< Cross Shape
const int ISL_SHAPE_ELLIPSE     = CV_SHAPE_ELLIPSE; ///< Elliptical Shape
/** @} */

/** @} */


/**
 *  \defgroup thresh Thresholding Methods
 *  \brief The different thresholding methods
 *  @{
 */


/**
 * \brief Binary thresholding
 *
 * \f{eqnarray*} dst(x,y) = &max value,& if src(x,y) > threshold \\ &0,& otherwise \f}
 */
const int ISL_THRESH_BINARY     = CV_THRESH_BINARY;
/**
 * \brief Inverted binary thresholding
 *
 * \f{eqnarray*} dst(x,y) = &0,& if src(x,y) > threshold \\ &max value,& otherwise \f}
 */
const int ISL_THRESH_BINARY_INV = CV_THRESH_BINARY_INV;
/**
 * \brief Truncated thresholding
 *
 * \f{eqnarray*} dst(x,y) = &threshold,& if src(x,y) > threshold \\ &src(x,y),& otherwise \f}
 */
const int ISL_THRESH_TRUNC      = CV_THRESH_TRUNC;
/**
 * \brief Threshold to zero
 *
 * \f{eqnarray*} dst(x,y) = &src(x,y),& if src(x,y) > threshold \\ &0,& otherwise \f}
 */
const int ISL_THRESH_TOZERO     = CV_THRESH_TOZERO;
/**
 * \brief Threshold to zero inverted
 *
 * \f{eqnarray*} dst(x,y) = &0,& if src(x,y) > threshold \\ &src(x,y),& otherwise \f}
 */
const int ISL_THRESH_TOZERO_INV = CV_THRESH_TOZERO_INV;
/** @} */

/**
 *  \defgroup ad_thresh Adaptive Thresholding Algorithms
 *  \brief The different methods for calculating the threshold associated to a pixel in an adaptive method
 *  @{
 */

/**
 * \brief The threshold is the mean value of a rectangular neighborhood of the pixel
 */
const int ISL_ADTHRESH_MEAN     = CV_ADAPTIVE_THRESH_MEAN_C;
/**
 * \brief The threshold is the weighted sum (gaussian) of a rectangular neighborhood of the pixel
 */
const int ISL_ADTHRESH_GAUSSIAN = CV_ADAPTIVE_THRESH_GAUSSIAN_C;
/** @} */


/**
 *  \defgroup line_type Drawing Related
 *  \brief The different types of line
 *  @{
 */
const int ISL_LINETYPE_4        = 4; ///< 4-connected line
const int ISL_LINETYPE_8        = 8; ///< 8-connected line
const int ISL_LINETYPE_AA       = CV_AA; ///< anti-aliased line
/** @} */


/**
 *  \defgroup calib_type Calibration Methods
 *  \brief The different algorithms for camera calibration
 *  @{
 */
const int ISL_CALIB_PERSP       = 0; ///< Perspective Correction
const int ISL_CALIB_TSAI        = 1; ///< Tsai Calibration Algorithm
/** @} */


/**
 *  \defgroup interp_type Interpolation Methods
 *  \brief The different algorithms for interpolating pixels
 *  @{
 */
const int ISL_INTERP_NN         = CV_INTER_NN; ///< nearest neighbor (worse quality)
const int ISL_INTERP_LINEAR     = CV_INTER_LINEAR; ///< bi-linear
const int ISL_INTERP_CUBIC      = CV_INTER_CUBIC; ///< bi-cubic
const int ISL_INTERP_AREA       = CV_INTER_AREA; ///< area mapping algorithm (best quality)
/** @} */


/**
 *  \defgroup rotate_flip Rotation/Flip Constants
 *  \brief The different transformations available
 *  @{
 */
const int ISL_ROTATE_NONE_FLIP_NONE = 0; ///< no transformation
const int ISL_ROTATE_90_FLIP_NONE   = 1; ///< 90� rotation
const int ISL_ROTATE_180_FLIP_NONE  = 2; ///< 180� rotation
const int ISL_ROTATE_270_FLIP_NONE  = 3; ///< 270� rotation
const int ISL_ROTATE_NONE_FLIP_H    = 4; ///< horizontal flip
const int ISL_ROTATE_90_FLIP_H      = 5; ///< 90� rotation, then horizontal flip
const int ISL_ROTATE_180_FLIP_H     = 6; ///< 180� rotation, then horizontal flip
const int ISL_ROTATE_270_FLIP_H     = 7; ///< 270� rotation, then horizontal flip
/** @} */


/**
 *  \defgroup target_extraction Calibration / Blob Centroid Location methods
 *  \brief The different methods for extracting the centroid of a blob in the calibration process
 *  @{
 */
const int ISL_TRUE_CENTROID        = 0; ///< 
const int ISL_SQUARED_CENTROID     = 1; ///< 
const int ISL_ACCURATE_CENTROID    = 2; ///< 
const int ISL_BOUNDING_RECT_CENTER = 3; ///< 
const int ISL_ELLIPSE_CENTER       = 4; ///< 
/** @} */


/**
 *  \defgroup norm_type Normalization Types
 *  \brief 
 *  @{
 */
const int ISL_NORM_INF = CV_C; ///< the maximum of absolute values is normalized
const int ISL_NORM_L1  = CV_L1; ///< the sum of absolute values is normalized
const int ISL_NORM_L2  = CV_L2; ///< the Euclidian norm is normalized
const int ISL_MINMAX   = CV_MINMAX; ///< the pixel values are scaled and shifted to a specified range
/** @} */



/** @} */

}



#endif
