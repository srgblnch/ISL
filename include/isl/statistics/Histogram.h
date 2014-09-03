//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#ifndef __INCLUDE_HISTOGRAM_H__
#define __INCLUDE_HISTOGRAM_H__

#include <isl/ISLConfig.h>

namespace isl {

class Image;

/**
 *  \addtogroup statistics
 *  @{
 */

/**
 *  \brief Class computing the uniform histogram of an image
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL Histogram
{

  public:

    /**
     *  \brief Constructor
     */
    Histogram(const Image&, int nb_bins, float low_thresh, float high_thresh);
    
    /**
     *  \brief Destructor
     */
    ~Histogram();

    int nb_bins() const;

    float low_thresh() const;
    
    float high_thresh() const;

    const float* bins() const;

  private:
    /**
     *  \brief Computes the moments of an image up to the 3rd order
     *  \param[in] image the input image
     *  \exception Exception
     */
    void
      compute(const Image& image, int nb_bins, float low_thresh, float high_thresh);

    CvHistogram* cv_hist_;
    int nb_bins_;
 
};


/** @} */

}


#if defined (__ISL_INLINE__)
# include <isl/statistics/Histogram.i>
#endif // __ISL_INLINE__

#endif
