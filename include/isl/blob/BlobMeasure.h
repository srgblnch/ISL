#ifndef __INCLUDE_BLOBMEASURE_H__
#define __INCLUDE_BLOBMEASURE_H__

#include <isl/geometry/Rectangle.h>
#include <isl/geometry/Ellipse.h>

namespace isl {

class Blob;
/**
 *  \addtogroup blob
 *  @{
 */

/**
 *  \addtogroup blob_measures Blob Measurements
 *  @{
 */

/**
 *  \brief Base class for an "on-demand" blob measurement
 *
 *  Each blob contains a list of measures that are computed only on-demand, i.e. when
 *  the measure accessor is called the first time. This class handles the
 *  computation of the measure on first access and saves the value so that
 *  it can be accessed several times without performance consequences.
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
template <class T>
class ISL_DECL BlobMeasure
{
  public:
    /**
     *  \brief Computes the measurement if it is the first call, and 
     *         returns a reference to the measure
     *
     *  This class must be inherited
     *
     */
    const T& get()
    {
      if (!this->is_computed_)
      {
        this->compute();
        this->is_computed_ = true;
      }
      return(this->measure_);
    };

  protected:
    virtual void compute() = 0;

    BlobMeasure(Blob& _blob) 
      : blob_(_blob),
        is_computed_(false)
    {};


    void copy_data(const BlobMeasure& _src)
    {
      this->is_computed_ = _src.is_computed_;
      if (_src.is_computed_)
        this->measure_ = _src.measure_;
    };

    virtual ~BlobMeasure() {};

    Blob& blob_;
    T     measure_;

  private:
    bool is_computed_;
};




/**
 *  \brief Computes the perimeter of the blob
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL BlobPerimeter : public BlobMeasure<double>
{
  friend class Blob;

  protected:
    virtual void compute();

  private:
    //- disallow these operations for non-friend class
    BlobPerimeter(Blob& _blob) : BlobMeasure<double>(_blob) {};
    virtual ~BlobPerimeter() {};
};

/**
 *  \brief Computes the area of the blob
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL BlobArea : public BlobMeasure<double>
{
  friend class Blob;

  protected:
    virtual void compute();

  private:
    //- disallow these operations for non-friend class
    BlobArea(Blob& _blob) : BlobMeasure<double>(_blob) {};
    virtual ~BlobArea() {};
};

/**
 *  \brief Computes the least square ellipse approximation of the set of contour points
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL BlobEllipse : public BlobMeasure<Ellipse>
{
  friend class Blob;

  protected:
    virtual void compute();

  private:
    //- disallow these operations for non-friend class
    BlobEllipse(Blob& _blob) : BlobMeasure<Ellipse>(_blob) {};
    virtual ~BlobEllipse() {};
};

/**
 *  \brief Computes the bounding rectangle of the blob
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL BlobBoundingRect : public BlobMeasure<Rectangle>
{
  friend class Blob;

  protected:
    virtual void compute();

  private:
    //- disallow these operations for non-friend class
    BlobBoundingRect(Blob& _blob) : BlobMeasure<Rectangle>(_blob) {};
    virtual ~BlobBoundingRect() {};
};

/** @} */
/** @} */


}

#endif
