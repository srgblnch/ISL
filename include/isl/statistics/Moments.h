//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#ifndef __INCLUDE_MOMENTS_H__
#define __INCLUDE_MOMENTS_H__

#include <isl/ISLConfig.h>

namespace isl {

class Image;

/**
 *  \addtogroup statistics
 *  @{
 */

/**
 *  \brief Class computing the moments of an image up to the third order
 *
 *  The spatial moments are defined as :
 *  \f[ M_{x_{order},y_{order}} = \sum_{x,y} x^{x_{order}} y^{y_{order}} I_{x,y} \f]
 *  where \f$ I_{x,y} \f$ is the intensity at pixel \f$ (x,y) \f$
 *
 *  These moments can be used to calculate the gravity center, the area, the main axises
 *  and various shape characteristics.
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL Moments2D
{

  public:

    /**
     *  \brief Constructor
     */
    Moments2D(const Image&);

    /**
     *  \brief Constructor
     */
    Moments2D(const Moments2D&);

    /**
     *  \brief Assignement operator
     */
    Moments2D& operator=(const Moments2D&);

    /**
     *  \brief Destructor
     */
    ~Moments2D();

    double m00  () const;
    double m10  () const;
    double m01  () const;
    double m20  () const;
    double m11  () const;
    double m02  () const;
    double m30  () const;
    double m21  () const;
    double m12  () const;
    double m03  () const;

    double mu00  () const;
    double mu10  () const;
    double mu01  () const;
    double mu20  () const;
    double mu11  () const;
    double mu02  () const;
    double mu30  () const;
    double mu21  () const;
    double mu12  () const;
    double mu03  () const;

    double eta00 () const;
    double eta10 () const;
    double eta01 () const;
    double eta20 () const;
    double eta11 () const;
    double eta02 () const;
    double eta30 () const;
    double eta21 () const;
    double eta12 () const;
    double eta03 () const;


    /**
     *  \brief Accessor for the spatial moments
     *
     *  The function returns
     *  \f[ M_{x_{order},y_{order}} = \sum_{x,y} x^{x_{order}} y^{y_{order}} I_{x,y} \f]
     *
     *  with \f$ x_{order} \geq 0 \f$, \f$ y_{order} \geq 0 \f$, and \f$ x_{order} + y_{order} \leq 3 \f$
     *
     *  \param[in] x_order X order of the retrieved moment
     *  \param[in] y_order Y order of the retrieved moment
     *  \exception Exception
     */
    double
      get_spatial_moment(int x_order, int y_order) const;

    /**
     *  \brief Accessor for the central moments
     *
     *  The function returns
     *  \f[ \mu_{x_{order},y_{order}} = \sum_{x,y} (x - x_c)^{x_{order}} (y - y_c)^{y_{order}} I_{x,y} \f]
     *  where \f$ x_c = M_{1,0} / M_{0,0} \f$ and \f$ y_c = M_{0,1} / M_{0,0} \f$
     *  
     *  with \f$ x_{order} \geq 0 \f$, \f$ y_{order} \geq 0 \f$, and \f$ x_{order} + y_{order} \leq 3 \f$
     *
     *  \param[in] x_order X order of the retrieved moment
     *  \param[in] y_order Y order of the retrieved moment
     *  \exception Exception
     */
    double
      get_central_moment(int x_order, int y_order) const;

    /**
     *  \brief Accessor for the normalized central moments
     *
     *  The function returns
     *  \f[ \eta_{x_{order},y_{order}} = \mu_{x_{order},y_{order}} / M_{0,0}^{1 + (x_{order} + y_{order})/2} \f]
     *  
     *  with \f$ x_{order} \geq 0 \f$, \f$ y_{order} \geq 0 \f$, and \f$ x_{order} + y_{order} \leq 3 \f$
     *
     *  \param[in] x_order X order of the retrieved moment
     *  \param[in] y_order Y order of the retrieved moment
     *  \exception Exception
     */
    double
      get_normalized_central_moment(int x_order, int y_order) const;

  private:
    /**
     *  \brief Computes the moments of an image up to the 3rd order
     *  \param[in] image the input image
     *  \exception Exception
     */
    void
      compute(const Image& image);

    CvMoments cv_moments_;
 
};



/**
 *  \brief Class computing the moments of a vector (a 1D distribution) up to the third order
 *
 *  The spatial moments are defined as :
 *  \f[ M_{order} = \sum_x x^{order} f(x) \f]
 *  where \f$ f(x) \f$ is the value of the distribution at index \f$ x \f$
 *
 *  These moments can be used to calculate the center, the variance or the skew of
 *  the distribution
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL Moments1D
{
  public:

    /**
     *  \brief Constructor
     */
    Moments1D(const double* vector, size_t vector_size);

    /**
     *  \brief Constructor
     */
    Moments1D(const Moments1D&);

    /**
     *  \brief Assignement operator
     */
    Moments1D& operator= (const Moments1D&);

    /**
     *  \brief Destructor
     */
    ~Moments1D();
    
    double m0  () const;
    double m1  () const;
    double m2  () const;
    double m3  () const;
    double mu0 () const;
    double mu1 () const;
    double mu2 () const;
    double mu3 () const;
    double eta0() const;
    double eta1() const;
    double eta2() const;
    double eta3() const;

    /**
     *  \brief Accessor for the spatial moments
     *
     *  The function returns
     *  \f[ M_{order} = \sum_x x^{order} f(x) \f]
     *
     *  with \f$ 0 \leq x_{order} \leq 3 \f$
     *
     *  \param[in] order order of the retrieved moment
     *  \exception Exception
     */
    double
      get_spatial_moment(int order) const;

    /**
     *  \brief Accessor for the central moments
     *
     *  The function returns
     *  \f[ \mu_{order} = \sum_x (x - x_c)^{order}  f(x) \f]
     *  where \f$ x_c = M_1 / M_0 \f$
     *  
     *  with \f$ 0 \leq x_{order} \leq 3 \f$
     *
     *  \param[in] order order of the retrieved moment
     *  \exception Exception
     */
    double
      get_central_moment(int order) const;

    /**
     *  \brief Accessor for the normalized central moments
     *
     *  The function returns
     *  \f[ \eta_{order} = \mu_{order} / M_0^{1 + order/2} \f]
     *  
     *  with \f$ 0 \leq x_{order} \leq 3 \f$
     *
     *  \param[in] order order of the retrieved moment
     *  \exception Exception
     */
    double
      get_normalized_central_moment(int order) const;

  private:
    /**
     *  \brief Computes the moments of a one dimensionnal vector up to the third order
     *  \param[in] vector a pointer to the data
     *  \param[in] vector_size the number of elements pointed by \a vector
     *  \exception Exception
     */
    void
      compute(const double* vector, size_t vector_size);

    CvMoments cv_moments_;
 
};
/** @} */

}


#if defined (__ISL_INLINE__)
# include <isl/statistics/Moments.i>
#endif // __ISL_INLINE__

#endif
