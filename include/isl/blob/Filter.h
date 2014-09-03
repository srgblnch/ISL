//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#ifndef __INCLUDE_FILTER_H__
#define __INCLUDE_FILTER_H__

#include <isl/ISLConfig.h>

namespace isl {

class Criteria;
class Operator;
class Blob;

/**
 *  \addtogroup blob
 *  @{
 */

/**
 *  \addtogroup blob_comparison
 *  @{
 */

/**
 *  \brief Base class for filtering operation
 *
 *  This class only has a pure virtual method taking a Blob and returning a boolean
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL Filter
{
public:
  /**
   *  \brief Conversion operator
   */
  operator Filter*() const;

  /**
   *  \brief Pure virtual method returning true if the Blob instance satisfy the criteria
   *         and false otherwise
   */
  virtual bool accept(Blob&) = 0;
};




/**
 *  \brief Class used to generate a Filter from a Criteria and an Operator 
 *         (see \ref blob_criteria and \ref blob_operators)
 *
 *  This class is used for filters based on numerical values like perimeter, areas,...
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL NumericalFilter : public Filter
{
public:
  /**
   *  \brief Conversion operator
   */
  NumericalFilter(Criteria* _crit, Operator* _op);

  virtual bool accept(Blob& _blob);

private:
  Criteria* crit_;
  Operator* op_;
};

/** @} */
/** @} */

}

#if defined (__ISL_INLINE__)
# include <isl/blob/Filter.i>
#endif // __ISL_INLINE__


#endif
