#include <isl/blob/Filter.h>
#include "InternalErrorHandler.h"

#include <isl/blob/Criteria.h>
#include <isl/blob/Operator.h>
#include <isl/blob/Blob.h>

#if !defined (__ISL_INLINE__)
# include <isl/blob/Filter.i>
#endif


namespace isl {


NumericalFilter::NumericalFilter(Criteria* _crit, Operator* _op) 
: crit_(_crit),
  op_(_op)
{
}


bool
NumericalFilter::accept(Blob& _blob)
{
  return( (*op_)((*crit_)(_blob)) );
}


}
