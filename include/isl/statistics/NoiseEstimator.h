//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#ifndef __INCLUDE_NOISEESTIMATOR_H__
#define __INCLUDE_NOISEESTIMATOR_H__

#include <isl/ISLConfig.h>

namespace isl {

class Image;

/**
 *  \brief 
 *
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL NoiseEstimator
{
  public:

    /**
     *  \brief Constructor
     */
    NoiseEstimator(void);
    
    /**
     *  \brief Destructor
     */
    ~NoiseEstimator();

    void init();

    const Image& add_image(const Image& im);

    int nb_image() const;

  private:
    Image* mean_;
    int n_;
};


}

#if defined (__ISL_INLINE__)
# include <isl/statistics/NoiseEstimator.i>
#endif // __ISL_INLINE__


#endif
