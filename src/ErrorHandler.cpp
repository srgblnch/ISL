//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#include <isl/ISLConfig.h>
#include <isl/ErrorHandler.h>
#include "InternalErrorHandler.h"


namespace isl {

void
ErrorHandler::init()
{
  cvRedirectError( InternalErrorHandler::store_error );
  ErrorHandler::enable_error_management();
  ErrorHandler::reset();
}

void
ErrorHandler::reset()
{
  InternalErrorHandler::exc.reset();

  cvSetErrStatus(CV_StsOk);
}

void
ErrorHandler::disable_error_management()
{
  //- the error callback will not be called any more
  cvSetErrMode( CV_ErrModeSilent );
}

void
ErrorHandler::enable_error_management()
{
  //- the error callback will ba called
  cvSetErrMode( CV_ErrModeParent );
}


}
