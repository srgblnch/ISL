//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#ifndef __INCLUDE_EXCEPTIONS_H__
#define __INCLUDE_EXCEPTIONS_H__

#include <string>
#include <ostream>
#include <vector>
#include <isl/ISLConfig.h>

#include <iostream>


namespace isl {


/**
 *  \addtogroup error
 *  @{
 */

/**
 *  \brief The class describing a single error
 *
 *  The class is used internally by the Exception class,
 *  which is mainly a stack of Error instances
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL Error
{
  public:

    Error();
    Error(int status, const char* func_name,
          const char* err_msg, const char* file_name,
          int line);
    ~Error();

    int status;
    std::string reason;
    std::string description;
    std::string origin;
};

ISL_DECL bool operator < (const class Error&, const class Error&);
ISL_DECL bool operator == (const class Error&, const class Error&);


EXPORT_STL_VECTOR(Error);
typedef std::vector<Error> ErrorList;

/**
 *  \brief The ISL exception class, thrown by methods of all other classes
 *         when encountering an error
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL Exception
{
  friend class InternalErrorHandler;
  friend class ErrorHandler;

  public:
    Exception();
    ~Exception();
    
    std::string
    to_string() const;
  
    ErrorList errors;
    
  private:
    void
    push_error(int status, const char* func_name,
               const char* err_msg, const char* file_name,
               int line);

    void
    reset();

};



/**
 *  \brief Function for dumping an exception to a stream
 *
 *  \param[in] os an abstract output stream (cout, ofstream, ostrstream...)
 *  \param[in] exc the exception to be dumped
 *  \author Julien Malik
 *  \date 08-2006
 */
inline
std::ostream& operator<<( std::ostream& os,
                          const Exception& exc)
{
    os << exc.to_string();
    return (os);
}
/** @} */


}


#if defined (__ISL_INLINE__)
# include <isl/Exception.i>
#endif // __ISL_INLINE__



#endif
