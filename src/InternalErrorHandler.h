#ifndef __INCLUDE_INTERNALERRORHANDLER_H__
#define __INCLUDE_INTERNALERRORHANDLER_H__

#include <isl/Exception.h>
#include <isl/ErrorHandler.h>

namespace isl {

/**
 *  \addtogroup error
 *  @{
 */

/**
 *  \brief Static class defining the callback used by OpenCV to redirect errors
 *  \author Julien Malik
 *  \date 08-2006
 */
class InternalErrorHandler
{
  friend class ErrorHandler;

public:
  /**
   *  \brief A static Exception class used to store the different errors
   */
  static Exception exc;

private:
  /**
   *  \brief The callback used by OpenCV to redirect errors
   */
  static int store_error( int status, const char* func_name,
                          const char* err_msg, const char* file_name,
                          int line, void* userdata = NULL);


};



/**
 *  \brief Macro used at the end of ISL functions to check error status
 *         and throw an exception if an error occured
 */
#define __ISL_CHECK_ERROR__           \
{                                     \
  if(cvGetErrStatus() < 0)            \
  {                                   \
    throw InternalErrorHandler::exc;  \
  }                                   \
}

/**
 *  \brief Macro used inside ISL functions when calling other ISL functions
 *         which could throw an exception.
 *
 *  It adds an Error to the error list showing where the failed call occured
 */
#define ISL_CALL( Statement )                             \
{                                                         \
  try                                                     \
  {                                                       \
    Statement;                                            \
  }                                                       \
  catch (Exception&)                                      \
  {                                                       \
    CV_ERROR( CV_StsBackTrace, "Inner function failed." ) \
  }                                                       \
}


#define __ISL_DISABLE_ERROR__               \
  int err_status_save = cvGetErrStatus();   \
  cvSetErrStatus(CV_StsOk);                 \
  ErrorHandler::disable_error_management();

#define __ISL_RESTORE_ERROR__               \
  cvSetErrStatus(err_status_save);          \
  ErrorHandler::enable_error_management();


/** @} */

}


#endif
