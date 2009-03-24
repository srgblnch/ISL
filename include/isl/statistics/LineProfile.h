#ifndef __INCLUDE_LINEPROFILE_H__
#define __INCLUDE_LINEPROFILE_H__

#include <isl/ISLConfig.h>
#include <isl/geometry/Point.h>

namespace isl {

class Image;
/**
 *  \addtogroup statistics Statistical Estimation
 *  @{
 */


/**
 *  \brief Class computing the line profiles inside an image, 
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL LineProfile
{
  public:
    /**
     *  \brief Constructor
     */
    LineProfile();

    /**
     *  \brief Destructor
     */
    ~LineProfile();

    /**
     *  \brief Computes the profiles
     *  \param[in] image the input image
     *  \exception Exception
     */
    Image*
      compute(const Image& image, Point2D<int> origin, Point2D<int> end, int thickness);
 
    /**
     *  \brief Accessor for the profile values
     */
    const double*
      values();

    /**
     *  \brief Number of profile values
     */
    int
      size();

  private:
    void
      clean();

    CvMat* profile_;
};
/** @} */

}

#if defined (__ISL_INLINE__)
# include <isl/statistics/LineProfile.i>
#endif // __ISL_INLINE__

#endif
