//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
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
