#ifndef __INCLUDE_POINT_H__
#define __INCLUDE_POINT_H__

#include <isl/ISLConfig.h>

namespace isl {
/**
 *  \addtogroup geometry Geometry Elements
 *  @{
 */

/**
 *  \brief Class used to describe a point
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
template <typename T>
class Point2D
{
  public:
    /**
     *  \brief Constructor
     */
    Point2D(const T& x = 0, const T& y = 0);

    /**
     *  \brief Copy constructor
     */
    Point2D(const Point2D<T>& p);

    /**
     *  \brief Destructor
     */
    ~Point2D();

    /**
     *  \brief Accessor for x coordinate
     */
    T& x();
    
    /**
     *  \brief Const accessor for x coordinate
     */
    T x() const;
   
    /**
     *  \brief Accessor for y coordinate
     */
    T& y();
    
    /**
     *  \brief Const accessor for y coordinate
     */
    T y() const;

    /**
     *  \brief Assignment operator
     */
    Point2D<T>& operator=(const Point2D<T>&);

    /**
     *  \brief Assignment operator
     */
    Point2D<T>& operator+=(const Point2D<T>&);
    
    /**
     *  \brief Assignment operator
     */
    Point2D<T>& operator-=(const Point2D<T>&);
    
    /**
     *  \brief Scalar product operator
     */
    double operator*(const Point2D<T>&) const;

    /**
     *  \brief Returns \f$ \| p \|_1 = | x | + | y | \f$
     */
    double norm_L1() const;
    /**
     *  \brief Returns \f$ \| p \|_{\infty} = max \left( | x | , | y | \right)\f$
     */
    double norm_Linf() const;
    /**
     *  \brief Returns \f$ \| p \|_2 = \sqrt{| x |^2 + | y |^2} \f$
     */
    double norm_L2() const;
    /**
     *  \brief Returns \f$ \| p \|_2^2 = | x |^2 + | y |^2 \f$
     */
    double norm_L2_square() const;

    /**
     *  \brief Returns \f$ \| p - q \|_1\f$
     */
    double dist_L1(const Point2D<T>& q) const;
    
    /**
     *  \brief Returns \f$ \| p - q \|_{\infty}\f$
     */
    double dist_Linf(const Point2D<T>& q) const;
    
    /**
     *  \brief Returns \f$ \| p - q \|_2\f$
     */
    double dist_L2(const Point2D<T>& q) const;
    
    /**
     *  \brief Returns \f$ \| p - q \|_2^2\f$
     */
    double dist_L2_square(const Point2D<T>& q) const;

  private:
    T x_, y_;
};


template <typename T>
Point2D<T>
operator +(const Point2D<T>&, const Point2D<T>&);

template <typename T>
Point2D<T>
operator -(const Point2D<T>&, const Point2D<T>&);

template <typename T>
bool
operator ==(const Point2D<T>&, const Point2D<T>&);

template <typename T>
bool
operator !=(const Point2D<T>&, const Point2D<T>&);

/** @} */

}


#include <isl/geometry/Point.t>


#if defined (__ISL_INLINE__)
# include <isl/geometry/Point.i>
#endif // __ISL_INLINE__


#endif
