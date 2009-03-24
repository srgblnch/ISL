#ifndef __INCLUDE_BEAM_BOX_H__
#define __INCLUDE_BEAM_BOX_H__

#include <isl/geometry/Rectangle.h>
#include <isl/geometry/Ellipse.h>

namespace isl {

class Image;
class Blob;

/**
 *  \brief Class which extracts the main blob from an image
 *         and approximate the contour by an ellipse
 *
 *  The BeamBox class can process a 8-bit unsigned image where non-zero
 *  pixels are treated as 1's.
 *  It finds the blobs present in the image, keeps only blobs with more than
 *  10 points describing the contour of the blob, then find the one with greater area,
 *  which is considered to be the beam.
 *  The bounding rectangle of the blob is computed, and its ellipse approximation
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL BeamBox : public Rectangle
{
  public:

    /**
     *  \brief Constructor
     */
    BeamBox(void);
    
    /**
     *  \brief Constructor
     *  \exception Exception
     *  \warning The image must be 8-bit unsigned and already preprocessed (binarized)
    */
    BeamBox(const Image& image, double magnification_factor_x, double magnification_factor_y);

    /**
     *  \brief Destructor
     */
    ~BeamBox();

  private:
    /**
     *  \brief Computes the beam box from an image
     *  \param[in] image the input 8-bit thresholded image
     *  \param[in] magnification_factor the factor by which the bounding rectangle is scaled
     *  \exception Exception
     *  \returns a boolean indicating if the beam box was found
     *  \warning The image must be 8-bit unsigned and already preprocessed (binarized)
     */
    bool
      compute(const Image& image, double magnification_factor_x, double magnification_factor_y);

    /**
     *  \brief Draws the beam box
     */
    void
      draw(Image& img, double intensity = 255.0f,  int thickness = 1);

  private:
    Blob* main_blob_;
};


}

#if defined (__ISL_INLINE__)
# include <isl/BeamBox.i>
#endif // __ISL_INLINE__


#endif
