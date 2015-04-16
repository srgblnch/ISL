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
