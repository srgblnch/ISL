//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#include <isl/Exception.h>

#include <sstream>

#include "InternalErrorHandler.h"

#if !defined (__ISL_INLINE__)
# include <isl/Exception.i>
#endif


namespace isl {

Error::Error()
{
}

Error::Error(int _status, const char* _func_name,
             const char* _err_msg, const char* _file_name,
             int _line)
{
  this->status = _status;

  this->reason = cvErrorStr(_status);

  this->description = _err_msg;

  this->origin = _func_name;
  this->origin += " (";
  this->origin += _file_name;
  this->origin += "[";
  std::stringstream s;
	s << _line;
	this->origin += s.str();
  this->origin += "])";
}


Error::~Error()
{
}

bool operator < (const class Error&, const class Error&)
{
  return true;
}

bool operator == (const class Error&, const class Error&)
{
  return false;
}




Exception::Exception()
{
}


Exception::~Exception()
{

}

void
Exception::push_error(int status, const char* func_name,
                      const char* err_msg, const char* file_name,
                      int line)
{
  this->errors.push_back( Error(status, func_name, err_msg, file_name, line) );
}

std::string
Exception::to_string() const
{
  std::stringstream s;
  std::vector<Error>::const_iterator it;

  s << "<=====================================" << std::endl;

  for (it = this->errors.begin(); it != this->errors.end(); it++)
  {
    if (it != this->errors.begin())
      s << "--------------------------------------" << std::endl;

    s << "reason      : " << (*it).reason << std::endl;
    s << "description : " << (*it).description << std::endl;
    s << "origin      : " << (*it).origin << std::endl;
  }

  s << "=====================================>" << std::endl;

  return( s.str() );

}

void
Exception::reset()
{
  this->errors.clear();
}




}
