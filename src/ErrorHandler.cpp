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
