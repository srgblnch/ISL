#ifndef __INCLUDE_OPERATOR_H__
#define __INCLUDE_OPERATOR_H__

namespace isl {

/**
 *  \addtogroup blob
 *  @{
 */

/**
 *  \addtogroup blob_operators Operators
 *  @{
 */

/**
 *  \brief Base class for all operators used in filtering and sorting operations
 *
 * Operators can be used in two distinct situations : filtering, and sorting.
 * \li When filtering a BlobList, the user can use operators associated to a given 
 *     threshold, like in bloblist.filter( Area(), Greater(100) )
 *     In that case, the operation used is 'bool operator()(double)'
 *     which compares the value to the threshold given on construction
 * \li When sorting a blob list, the situation is different in that the operator 
 *     must evaluate the condition between two different blobs. In that case, the 
 *     operation used is 'bool operator()(double , double)'
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL Operator
{
public:
  Operator(double t = 0) : t_(t) {};


  /**
   *  \brief Compares the \a value with the threshold given on construction
   */
  virtual bool operator()(double value) const = 0;
  
  /**
   *  \brief Compares two given values
   */
  virtual bool operator()(double _v1, double _v2) const = 0;

  /**
   *  \brief Conversion operator
   */
  operator Operator*() const
  {
    return (Operator*)this;
  };

protected:
  double t_;
};


/**
 *  \brief Equality operator
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL Equal : public Operator
{
public:
  Equal(double t = 0) : Operator(t) {};

  virtual bool operator()(double value) const
  {
    return(value == t_); 
  };

  virtual bool operator()(double _v1, double _v2) const
  {
    return(_v1 == _v2); 
  };
};

/**
 *  \brief Non-equality operator
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL NotEqual : public Operator
{
public:
  NotEqual(double t = 0) : Operator(t) {};

  virtual bool operator()(double value) const
  {
    return(value != t_); 
  };

  virtual bool operator()(double _v1, double _v2) const
  {
    return(_v1 != _v2); 
  };
};

/**
 *  \brief 'Greater Than' operator
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL Greater : public Operator
{
public:
  Greater(double t = 0) : Operator(t) {};

  virtual bool operator()(double value) const
  {
    return(value > t_); 
  };

  virtual bool operator()(double _v1, double _v2) const
  {
    return(_v1 > _v2); 
  };
};

/**
 *  \brief 'Greater or Equal Than' operator
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL GreaterOrEqual : public Operator
{
public:
  GreaterOrEqual(double t = 0) : Operator(t) {};

  virtual bool operator()(double value) const
  {
    return(value >= t_); 
  };

  virtual bool operator()(double _v1, double _v2) const
  {
    return(_v1 >= _v2); 
  };
};

/**
 *  \brief 'Lesser Than' operator
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL Lesser : public Operator
{
public:
  Lesser(double t = 0) : Operator(t) {};

  virtual bool operator()(double value) const
  {
    return(value < t_); 
  };

  virtual bool operator()(double _v1, double _v2) const
  {
    return(_v1 < _v2); 
  };
};

/**
 *  \brief 'Lesser or Equal Than' operator
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL LesserOrEqual : public Operator
{
public:
  LesserOrEqual(double t = 0) : Operator(t) {};

  virtual bool operator()(double value) const
  {
    return(value <= t_); 
  };

  virtual bool operator()(double _v1, double _v2) const
  {
    return(_v1 <= _v2); 
  };
};
/** @} */
/** @} */

}

#endif
