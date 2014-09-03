//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#include "InternalErrorHandler.h"


namespace isl {

Exception   InternalErrorHandler::exc;

int InternalErrorHandler::store_error( int status, const char* func_name,
                                       const char* err_msg, const char* file_name,
                                       int line, void*)
{
  exc.push_error(status, func_name, err_msg, file_name, line);
  return(0);
}


}
