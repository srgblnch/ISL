#ifndef __INCLUDE_CRITERIA_H__
#define __INCLUDE_CRITERIA_H__

#include <isl/ISLConfig.h>
#include <isl/geometry/Point.h>

namespace isl {

class Blob;
/**
 *  \addtogroup blob
 *  @{
 */

/**
 *  \addtogroup blob_criteria Criterias
 *  @{
 */

/**
 *  \brief Base class for a criteria used to filter a list of blob 
 *         or for sorting a list of blobs (see \ref blob_comparison)
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL Criteria
{
public:
  virtual ~Criteria() {};

  virtual double operator()(Blob&) const = 0;
  
  /**
   *  \brief Conversion operator
   */
  operator Criteria*() const;
};



/**
 *  \brief Get access to the blob area
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL Area : public Criteria
{
public:
  double operator()(Blob&) const;
};

/**
 *  \brief Get access to the blob perimeter
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL Perimeter : public Criteria
{
public:
  double operator()(Blob &blob) const;
};

/**
 *  \brief Get access to the number of points describing the contour
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL ContourNbPoint : public Criteria
{
public:
  double operator()(Blob &blob) const;
};

/**
 *  \brief Get access to the minimal X coordinate of the blob contour
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL MinX : public Criteria
{
public:
  double operator()(Blob &blob) const;
};

/**
 *  \brief Get access to the maximal X coordinate of the blob contour
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL MaxX : public Criteria
{
public:
  double operator()(Blob &blob) const;
};

/**
 *  \brief Get access to the minimal Y coordinate of the blob contour
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL MinY : public Criteria
{
public:
  double operator()(Blob &blob) const;
};

/**
 *  \brief Get access to the maximal Y coordinate of the blob contour
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL MaxY : public Criteria
{
public:
  double operator()(Blob &blob) const;
};

/** @} */
/** @} */

}

#if defined (__ISL_INLINE__)
# include <isl/blob/Criteria.i>
#endif // __ISL_INLINE__


#endif
