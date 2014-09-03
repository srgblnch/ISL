//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#ifndef __INCLUDE_BLOBCOMPARATOR_H__
#define __INCLUDE_BLOBCOMPARATOR_H__

#include <isl/blob/Criteria.h>
#include <isl/blob/Operator.h>

namespace isl {

/**
 *  \addtogroup blob
 *  @{
 */

/**
 *  \addtogroup blob_comparison Blob Comparison
 *  @{
 */

/**
 *  \brief Class used to compare two blobs given a criteria (see \ref blob_criteria)
 *         and an operator (see \ref blob_operators)
 *
 *  This class is intended to be used as a predicate for STL functions like std::sort, 
 *  std::partial_sort,... Look at the code of PatternAnalyzer::extract_blobs to see an
 *  example of use.
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL BlobComparator
{
public:
  BlobComparator(Criteria* _crit, Operator* _op) : crit_(_crit), op_(_op) {};
  ~BlobComparator() {};

  /**
   *  \brief Conversion operator
   */
  operator BlobComparator*() const;

  bool operator()(Blob& _b1, Blob& _b2);

private:
  Criteria* crit_;
  Operator* op_;
};
/** @} */
/** @} */

}

#if defined (__ISL_INLINE__)
# include <isl/blob/BlobComparator.i>
#endif // __ISL_INLINE__


#endif
