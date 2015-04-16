#ifndef __INCLUDE_PRINCIPALAXIS_H__
#define __INCLUDE_PRINCIPALAXIS_H__

#include <isl/ISLConfig.h>
#include <isl/geometry/Point.h>

namespace isl {

class ISL_DECL Axis
{

public :
	// JGO 
	Axis(Point2D<int> ai_origin, Point2D<int> ai_end)
	{
		origin = ai_origin;
		end = ai_end;
		values_ = 0;
		size = 0;
	};
	
	Axis(Point2D<int>& ai_origin, Point2D<int>& ai_end)
	{
		origin = ai_origin;
		end = ai_end;
		values_ = 0;
		size = 0;
	};
	Axis(const Axis& src) 
	{
		if (&src != this)
		{
			*this = src;
		}
	}

	~Axis()
	{
		if(values_)
		{
			delete[] values_;
			values_ = 0;
			size = 0;
		}
	}

	/**
     *  \brief Assignment operator
     */
    Axis& operator=(const Axis& _a)
	{
		if (&_a == this)
		{
			return *this;
		}

	  this->origin = _a.getOrigin();
	  this->end = _a.getEnd();
	  this->size = _a.getSize();
	  if (this->size)
	  {
		  this->values_ = new double[this->size];
		  memcpy(this->values_, _a.getValues(), this->size * sizeof (double));
		  //values_ = cvCreateMat( size, 1, CV_64F ); 
	  }
	  else
	  {
		this->values_ = NULL;
	  }
	  return(*this);
	}

	Point2D<int> getOrigin() const
	{
		return origin;
	}

	Point2D<int> getEnd() const
	{
		return end;
	}

	int getSize() const
	{
		return size;
	}

	const double* getValues() const
	{
		if(values_)
			return values_;
			//return values_->data.db;
		else
			return NULL;
	}	
private:


private:

	Point2D<int> origin;	
	Point2D<int> end;
	int size;
	double *values_;
    //CvMat* values_;
	//CvMat* fittedValues_;
};	

/**
 *  \addtogroup statistics
 *  @{
 */

/**
 *  \brief Class computing the principal axis of an image, a scatter of point
 *         or anything that can be represented by its moments
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL PrincipalAxis
{

  public:
    /**
     *  \brief Constructor
     *
     *  \exception Exception
     */
    PrincipalAxis(double mx, double my, double cxx, double cxy, double cyy);
    
    /**
     *  \brief Destructor
     */
    ~PrincipalAxis(void);

    /**
     *  \brief Computes the principal axis from moments
     *  \param[in] mx the 1st order moment in the X direction
     *  \param[in] my the 1st order moment in the Y direction
     *  \param[in] cxx the 2nd order moment in the X direction
     *  \param[in] cxy the 2nd order moment (cross correlation in X and Y directions)
     *  \param[in] cyy the 2nd order moment in the Y direction
     *  \exception Exception
     */
    void
      compute(double mx, double my, double cxx, double cxy, double cyy);

    /**
     *  \brief Accessor for the first principal axis
     */
    void
      get_first(double &x, double &y, double &std_dev) const;

    /**
     *  \brief Accessor for the second principal axis (orthogonal to the first one, oriented in the trigonometric direction)
     */
    void
      get_second(double &x, double &y, double &std_dev) const;

    /**
     *  \brief Accessor for the center [mx, my]
     */
    void
      get_center(double &x, double &y) const;

    /**
     *  \brief Accessor for the angle between the X direction and the first principal axis
     */
    double
      get_angle(void) const;

	/**
     *  \brief compute major and minor axis positions
	 */
	void computeMajorMinorPositions(const double center_x, const double center_y, const int width, const int height);

	/**
     *  \brief compute major and minor values
	 */
	void computeMajorAndMinorAxis();

	/**
     *  \brief get major axis
	 */
	const Axis* getMajor() const;
	
	/**
     *  \brief get minor axis
	 */
	const Axis* getMinor() const;

  private:
    void
      release(void);

	/**
     *  \brief get values on major or minor axes
	 */
	double
	getValueOnAxisEllipse(const bool isMajor, const bool isGetY, const double  value) const;

    CvMat* covariance_;
    CvMat* eigen_vectors_;
    CvMat* eigen_values_;
    CvPoint2D64f center_;
    double angle_;
	Axis*   major_;
	Axis*   minor_;
	Point2D<int> centroide_;
	int    width_;
	int    height_;
};
/** @} */

}

#if defined (__ISL_INLINE__)
# include <isl/statistics/PrincipalAxis.i>
#endif // __ISL_INLINE__


#endif
