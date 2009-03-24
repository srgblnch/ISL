#ifndef __INCLUDE_PERSPENGINE_H__
#define __INCLUDE_PERSPENGINE_H__

#include "Engine.h"


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
 *  \brief Models and corrects the perspective distorsion by an homography
 *         (no radial distorion is taken into account)
 *
 *
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class PerspEngine : public Engine
{
  public:

    PerspEngine(void);
    virtual ~PerspEngine();

    virtual void
      learn(const PatternAnalyzer& pa);

    virtual void
      undistort(const Image& src, Image& dst, int interp) const;

    void
      draw_error_map(Image& dst, double scale, double intensity, int thickness = 1, int line_type = ISL_LINETYPE_4) const;

  private:
    CvMat* hom_w2i_;
    CvMat* hom_i2w_;
    CvMat* new_world_coord_;
    CvMat* error_map_;
};
/** @} */
/** @} */


}

#if defined (__ISL_INLINE__)
# include "PerspEngine.i"
#endif // __ISL_INLINE__


#endif
