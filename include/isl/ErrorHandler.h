//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#ifndef __INCLUDE_ERRORHANDLER_H__
#define __INCLUDE_ERRORHANDLER_H__

#include <isl/ISLConfig.h>

namespace isl {

/**
 *  \addtogroup error Error Handling
 *
 *  <b>User point of view</b>
 *
 *  Error handling in ISL is implemented with exceptions. ISL functions do not 
 *  return any error code, instead they raise an exception of the Exception 
 *  class.
 *
 *  The user can then easily visualize the call stack responsible 
 *  for the error by dumping the exception to a stream 
 *  (see operator<<( std::ostream& os, const Exception& exc) )
 *
 *  It is very important to call ErrorHandler::init before doing anything with ISL,
 *  since it initializes internal stuff for handling errors properly
 *
 *  Here is an example of code using ISL and its error handling
 *  \code
 *  #include <iostream>
 *  #include <isl/ErrorHandler.h>
 *  #include <isl/Image.h>
 *  #include <isl/Exception.h>
 *
 *  int main( int argc, char **argv)
 *  {
 *    isl::ErrorHandler::init();
 *    isl::Image* i = 0;
 *    try
 *    {
 *      i = new isl::Image("toto.jpg");
 *      i->equalize_hist();
 *      i->save("titi.png");
 *    }
 *    catch(isl::Exception& e)
 *    {
 *      std::cout << e;
 *      isl::Errorhandler::reset(); //- re-init the error status if you want to
 *                                  //- call other ISL functions after that
 *      return 1;
 *    }
 *    return 0;
 *  }
 *  \endcode
 *
 *  <b>Internal error handling mechanisms</b>
 *
 *  The internal handling of errors is inspired by OpenCV error handling.
 *  OpenCV functions do not return error codes, but instead make use of a global
 *  error status through static variables. It allows to properly separate variables
 *  declaration, allocation, and deallocation inside the function so that it is written
 *  only one time.
 *
 *  OpenCV functions use macros for handling errors in a simple way:
 *  \li __BEGIN__ & __END__ are used for enclosing processing statements within 
 *      a function and separating them from prologue (resource initialization)
 *      and epilogue (guaranteed resource release)
 *  \li EXIT proceeds to "resource release" stage
 *  \li CV_FUNCNAME() declares locally the function name for CV_ERROR() use
 *  \li CV_ERROR() raises an error within the current context
 *  \li CV_CALL() provides a shorthand for OpenCV functions call, error checking
 *      and (if necessary) error raising
 *
 *  ISL also make use of these macros internally, and adds two new ones :
 *  \li __ISL_CHECK_ERROR__, used to check the current error status and throw 
 *      an ISL exception (see Exception) if an error has been raised during 
 *      the function
 *  \li ISL_TRY, used to enclose an ISL function call within a try-catch statement
 *      and silently re-throw the Exception in case of error
 *
 *  To record the error stack and provide it to the user, the OpenCV error callback
 *  mechanism is used through InternalErrorHandler::store_error.
 *  Each time an error is raised, either by an OpenCV function or by an ISL function,
 *  the callback is called and a new Error instance is created and appended to the 
 *  static Exception instance encapsulated in the InternalErrorHandler class.
 *  If inside OpenCV functions, errors are silently appended to the list.
 *  When arriving to the ISL function which called the failed OpenCV function,
 *  the current Exception is thrown.
 *
 *  The case of an ISL function calling another ISL function is handled this way :
 *  a macro ISL_TRY has been defined which takes as argument an ISL function call;
 *  this macro encloses the statement within a try-catch block, and in case an error 
 *  occurs, the ISL exception is properly caught and a new error is raised. The effect
 *  is that the error stack will contain all the history of OpenCV and ISL errors
 *  which lead to the current point.
 *
 *  The following code shows a typical ISL function skeleton :
 *  \code
 *  
 *  void MyClass::my_isl_func()
 *  {
 *    //- variable declaration & initialization to 0
 *    IplImage* an_internal_image = 0;
 *    Image* an_internal_isl_image = 0;
 *
 *    CV_FUNCNAME( "MyClass::my_isl_func" );
 *    __BEGIN__;
 *
 *    //- call an OpenCV function. On problem, go to __END__
 *    CV_CALL( an_image = cvCreateImage(cvSize(640, 480), st, 1) );
 *
 *    //- call ISL functions. On problem, go to __END__
 *    ISL_TRY( an_internal_isl_image = new Image(an_image) );
 *    ISL_TRY( an_internal_isl_image->morphology(ISL_DILATE) );
 *
 *    __END__;
 *
 *    //- guaranteed ressource release
 *    //- (either the variable has been allocated or not)
 *    cvReleaseImage(&an_internal_image);
 *    
 *    if (an_internal_isl_image != 0)
 *      delete an_internal_isl_image;
 *
 *    //- check error status and throw an exception in case a problem occured
 *    __ISL_CHECK_ERROR__;
 *  }
 *
 *
 *  \endcode
 *
 *
 *
 *  @{
 */

/**
 *  \brief Static class used to init the error handling system and
 *         to reset the current error status (after an exception has been caught)
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL ErrorHandler
{
  friend class InternalErrorHandler;

public:

  /**
   *  \brief Initializes the error handling system
   *
   *  This function MUST BE CALLED before using any other ISL class,
   *  because it sets the error callback so as errors are properly handled.
   */
  static void init();

  /**
   *  \brief Clears all errors stored in the error list
   *
   *  This function is intended to be used by the user after having 
   *  caught an exception and after having handled it, so as processing
   *  can continue in the same process using ISL.
   */
  static void reset();

//private :
  static void disable_error_management();

  static void enable_error_management();
};
/** @} */


}


#endif
