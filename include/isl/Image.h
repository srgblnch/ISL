//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#ifndef __INCLUDE_IMAGE_H__
#define __INCLUDE_IMAGE_H__


#include <isl/ISLConfig.h>

namespace isl {

class Rectangle;
template <typename T> class Point2D;

/**
 *  \brief The class which encapsulates a grayscale image
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL Image
{
  public:
    /**
     *  \brief Constructor
     *  Allocates space for storing an image given its width, height and storage type (from \ref storage)
     *  \param[in] width the desired width in pixels
     *  \param[in] height the desired height in pixels
     *  \param[in] st the depth of a pixel (see \ref storage)
     *  \exception Exception on memory allocation error
     */
    Image(int width, int height, int st);    
    
    /**
     *  \brief Constructor
     *  Loads an image from a graphic file (with automatic grayscale conversion)
     *  \param[in] filename the path to the file to load
     *  \exception Exception
     */
    Image(const char* filename);

    /**
     *  \brief Copy Constructor
     *
     *  \exception Exception
     */
    Image(const Image& img);

    /**
     *  \brief Destructor
     */
    ~Image();

    /**
     *  \brief Assignment operator
     */
    Image&
      operator = (const Image& img);

    /**
     *  \brief Pixel value accessor
     */
    double
      value(int row, int col) const;
    
    /**
     *  \brief Width of the image
     */
    int   
      width() const;

    /**
     *  \brief Height of the image
     */
    int   
      height() const;

    /**
     *  \brief Depth of pixel (returns a value in \ref storage)
     */
    int
      depth() const;

    /**
     *  \brief Number of bits per pixel
     */
    int
      bit_per_pixel() const;

    /**
     *  \brief Size of a row in bytes (with possible alignment taken into account)
     */
    int   
      row_byte_step() const;
    
    /**
     *  \brief Data buffer accessor
     */
    const void*
      data() const;

    /**
     *  \brief Data buffer accessor
     */
    void* 
      data();

    /**
     *  \brief Addition assignment operator
     */
    Image&
      operator += (const Image& img);

    /**
     *  \brief Substraction assignment operator
     */
    Image&
      operator -= (const Image& img);

    /**
     *  \brief Scalar addition operator
     */
    Image&
      operator += (double val);

    /**
     *  \brief Scalar substraction operator
     */
    Image&
      operator -= (double val);

    /**
     *  \brief Sets every element to zero
     *
     *  \exception Exception
     */
    Image&
      set_zero();
    
    /**
     *  \brief Sets every element to a particular value
     *
     *  \exception Exception
     */
    Image&
      set(double value);

    /**
     *  \brief Raises the pixel value to power
     *
     *  \param[in] p the power to raise every pixel value
     *  \exception Exception
     *  \warning For non-integer power value, the image must be of floating point type
     */
    Image&
      power(double p);

    /**
     *  \brief Corrects for a specified gamma

     *
     *  \param[in] gamma the gamma value whose effect is canceled
     *  \param[in] max_pixel_value the maximal pixel value used to preserve the dynamic range
     *  \warning For non-integer gamma value, the image must be of floating point type
     *  \exception Exception
     */
    Image&
      gamma_correction(double gamma, double max_pixel_value);

    /**
     *  \brief Retrieves the minimal and maximal pixel value in the image, 
     *         and their location
     *
     *  \param[out] min_value reference to the minimal value
     *  \param[out] max_value reference to the minimal value
     *  \param[out] min_loc location of the minimal value
     *  \param[out] max_loc location of the maximal value
     *  \exception Exception
     */
    void
      min_max_location(double& min_value, double& max_value, Point2D<int>& min_loc, Point2D<int>& max_loc) const;
    void
      min_max(double& min_value, double& max_value) const;

    /**
     *  \brief 
     *
     *  \exception Exception
     */
    double
      estimate_background(int border_size);

    /**
     *  \brief Rotate and/or flip the image
     *  \param[in] operation the desired operation (see \ref rotate_flip)
     *  \exception Exception
     */
    Image&
      rotate_flip(int operation);

    /**
     *  \brief Fixed thresholding
     *  \param[in] value threshold level
     *  \param[in] max_value value used for ISL_THRESH_BINARY and ISL_THRESH_BINARY_INV method
     *  \param[in] type the thresholding method (see \ref thresh)
     *  \exception Exception
     *  \warning The image must be 8-bit or 32-bit floating point
     */
    Image&
      threshold(double value, double max_value, int type = ISL_THRESH_BINARY);

    /**
     *  \brief Adaptive thresholding
     *  \param[in] max_value value used for ISL_THRESH_BINARY and ISL_THRESH_BINARY_INV method
     *  \param[in] method the method used to compute the threshold associated to each pixel (see \ref ad_thresh) 
     *  \param[in] type the thresholding method (see \ref thresh)
     *  \param[in] block_size the size of a pixel neighborhood that is used to calculate a threshold value for the pixel: 3, 5, 7, ...
     *  \param[in] offset constant subtracted from mean or weighted mean (depending on 'method' parameter), though it may be negative
     *  \exception Exception
     *  \warning The image must be 8-bit or 32-bit floating point
     */
    Image&
      adaptive_threshold(double max_value, int method = ISL_ADTHRESH_MEAN, int type = ISL_THRESH_BINARY, int block_size = 3, double offset = 0);

    /**
     *  \brief Smoothes the image with gaussian blur
     *  \param[in] kernel_size_x the horizontal size of the kernel
     *  \param[in] kernel_size_y the vertical size of the kernel (if zero, it is set to 'kernel_size_x')
     *  \exception Exception
     *  \warning The image must be 32-bit floating point
     */
    Image&
      smooth_gaussian(int kernel_size_x = 3, int kernel_size_y = 0);

    /**
     *  \brief Equalizes the histogram of the image
     *
     *  \exception Exception
     *  \warning The image must be 8-bit unsigned
     */
    Image&
      equalize_hist();

    /**
     *  \brief Applies a morphological filter
     *  \param[in] op the type of operation (see \ref morpho_op)
     *  \param[in] cols the width of the structuring element
     *  \param[in] rows the height of the structuring element
     *  \param[in] anchor_x the X position of the anchor point inside the structuring element (unless specifically wanted, you should put it to half the width)
     *  \param[in] anchor_y the Y position of the anchor point inside the structuring element (unless specifically wanted, you should put it to half the height)
     *  \param[in] shape the shape of the structuring element (see \ref morpho_shape)
     *  \param[in] iterations the number of times the operation is applied
     *  \exception Exception
     *  \warning The image must be 8-bit unsigned or 32-bit floating point
     */
    Image&
      morphology(int op, int cols = 3, int rows = 3, int anchor_x = 1, int anchor_y = 1, int shape = ISL_SHAPE_RECT, int iterations = 1);
      

    /**
     *  \brief Normalize the image to a certain norm or range
     *  \param[in] a the minimum/maximum value or the norm
     *  \param[in] b the minimum/maximum value
     *  \param[in] norm_type the type of normalization (\see norm_type)
     */
    Image&
      normalize(double a = 1, double b = 0, int norm_type = ISL_NORM_L2);



    IplImage*
      get_ipl_image() const;

    /**
     *  \brief Serializes pixel values in a buffer, row by row
     *  \param[in] output a pointer to the buffer used to store pixel values
     */
    const Image&
      serialize(void* output) const;

    /**
     *  \brief Unserializes pixel values from a buffer, row by row
     *  \param[in] input a pointer to the buffer used to store pixel values
     */
    Image&
      unserialize(const void* input);

    /**
     *  \brief Converts an image from a storage type to another with optional linear transformation
     *
     *  The function copies one array to another with optional scaling, which is performed
     *  first, and/or optional type conversion, performed after.
     *
     *  The type conversion is done with rounding and saturation, that is if a result of 
     *  scaling + conversion can not be represented exactly by a value of destination array 
     *  element type, it is set to the nearest representable value on the real axis.
     *
     *  \param[in] type the destination type (see \ref storage)
     *  \param[in] scale optional scale factor
     *  \param[in] shift optional value added to the scaled source array elements
     *  \exception Exception
     */
    Image&
      convert(int type, double scale = 1, double shift = 0);

    /**
     *  \brief Copy an image into another with optional linear transformation and/or type conversion
     *  \param[in] dst_image the destination image (already allocated and well-sized)
     *  \param[in] scale optional scale factor
     *  \param[in] shift optional value added to the scaled source array elements
     *  \exception Exception
     */
    const Image&
      convert(Image& dst_image, double scale = 1, double shift = 0) const;

    /**
     *  \brief Makes a full copy of the image
     *  \returns a pointer to the newly cloned image
     *  \exception Exception
     */
    Image*
      clone() const;

    /**
     *  \brief Makes a copy of a rectangular area of the image
     *
     *  The function clips the rectangular area defined by the parameters to fit
     *  to the actual size of the image and updates the parameters to reflect the modifications
     *
     *  \param[in,out] actual_origin_x X-coordinate of the origin of the rectangular area
     *  \param[in,out] actual_origin_y Y-coordinate of the origin of the rectangular area
     *  \param[in,out] actual_width width of the rectangular area
     *  \param[in,out] actual_height height of the rectangular area
     *  \returns a pointer to the newly cloned sub-image
     *  \exception Exception
     */
    Image*
      get_roi(int& actual_origin_x, int& actual_origin_y, int& actual_width, int& actual_height) const;

    /**
     *  \brief Makes a copy of a rectangular area of the image
     *
     *  The function clips the rectangular area defined by the parameter to fit
     *  to the actual size of the image and updates the parameter to reflect the modifications
     *
     *  \param[in,out] rect coordinates of the rectangle of interest
     *  \returns a pointer to the newly cloned sub-image
     *  \exception Exception
     */
    Image*
      get_roi(Rectangle& rect) const;

    /**
     *  \brief Saves an image to a file
     *
     *  The following formats are supported:
     *  \li Windows Bitmaps - BMP, DIB
     *  \li Portable image format - PBM, PGM, PPM
     *  \li Sun rasters - SR, RAS
     *  \li TIFF files - TIFF, TIF
     *  \li JPEG files - JPEG, JPG, JPE
     *  \li Portable Network Graphics - PNG
     *
     *  \param[in] filename name of the file
     *  \exception Exception
     *  \warning The image must be 8-bit unsigned
     */
    void
      save(const char* filename) const;

    /**
     *  \brief Loads an image from a file
     *
     *  The following formats are supported:
     *  \li Windows Bitmaps - BMP, DIB
     *  \li Portable image format - PBM, PGM, PPM
     *  \li Sun rasters - SR, RAS
     *  \li TIFF files - TIFF, TIF
     *  \li JPEG files - JPEG, JPG, JPE
     *  \li Portable Network Graphics - PNG
     *
     *  In case of a colored (RGB) file, the file is automatically converted to grayscale
     *
     *  \param[in] filename name of the file
     *  \exception Exception
     */
    void
      load(const char* filename);

	/**
	* Resize
	*/
	void resize(Image& dest_image) const;

  private:
    void
      create(int width, int height, int st);
    
    void
      release();

    Image&
      copy(const IplImage& img);

    Image&
      take_ownership(IplImage* img);

    IplImage* cv_image_;

};

}



#if defined (__ISL_INLINE__)
# include <isl/Image.i>
#endif // __ISL_INLINE__


#endif //include protection
