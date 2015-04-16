//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#include <isl/statistics/PrincipalAxis.h>
#include "InternalErrorHandler.h"

#if !defined (__ISL_INLINE__)
# include <isl/statistics/PrincipalAxis.i>
#endif


namespace isl {

PrincipalAxis::PrincipalAxis(double mx, double my, double cxx, double cxy, double cyy)
: major_(0)
, minor_(0)
{
  CV_FUNCNAME( "PrincipalAxis::PrincipalAxis" );
  __BEGIN__;

  CV_CALL( this->covariance_    = cvCreateMat( 2, 2, CV_64FC1 ) );
  CV_CALL( this->eigen_vectors_ = cvCreateMat( 2, 2, CV_64FC1 ) );
  CV_CALL( this->eigen_values_  = cvCreateMat( 2, 1, CV_64FC1 ) );

  CV_CALL( cvZero(this->covariance_   ) );
  CV_CALL( cvZero(this->eigen_vectors_) );
  CV_CALL( cvZero(this->eigen_values_ ) );

  this->compute(mx, my, cxx, cxy, cyy);

  __END__;

  if( cvGetErrStatus() < 0 )
  {
    __ISL_DISABLE_ERROR__;
    this->release();
    __ISL_RESTORE_ERROR__;
  }
  __ISL_CHECK_ERROR__;
}

PrincipalAxis::~PrincipalAxis()
{
	if (major_)
	{
		delete major_;
		major_ = 0;
	}

	if (major_)
	{
		delete major_;
		major_ = 0;
	}

  this->release();
}

void PrincipalAxis::release()
{
  cvReleaseMat( &this->covariance_ );
  cvReleaseMat( &this->eigen_vectors_ );
  cvReleaseMat( &this->eigen_values_ );
}

void
PrincipalAxis::compute(double mx, double my, double cxx, double cxy, double cyy)
{
   CV_FUNCNAME( "PrincipalAxis::compute" );
  __BEGIN__;
  
  // compute the center
  this->center_.x = mx;
  this->center_.y = my;

  // fill in covariance matrix
  cvmSet( this->covariance_, 0, 0, cxx );
  cvmSet( this->covariance_, 0, 1, cxy );
  cvmSet( this->covariance_, 1, 0, cxy );
  cvmSet( this->covariance_, 1, 1, cyy );

  // compute the eigen analysis
  CV_CALL( cvSVD( this->covariance_, this->eigen_values_, this->eigen_vectors_) );

  // make the necessary arrangement to have well-oriented eigen vectors
  // we want the coordinates of the first axis to be (+,+) or (+,-)
  // and the second vector to be oriented in the trigonometric direction
  CvPoint2D64f first_axis;
  first_axis.x  = cvmGet(this->eigen_vectors_, 0, 0);
  first_axis.y  = cvmGet(this->eigen_vectors_, 1, 0);

  if (first_axis.x < 0)
  {
    cvmSet(this->eigen_vectors_, 0, 0, - first_axis.x);
    cvmSet(this->eigen_vectors_, 1, 0, - first_axis.y);
  }

  // the second eigen vector is orthogonal to the first (covariance is symetric)
  cvmSet(this->eigen_vectors_, 0, 1, - first_axis.y);
  cvmSet(this->eigen_vectors_, 1, 1, first_axis.x);


  // update angle
  this->angle_ = atan2(cvmGet(this->eigen_vectors_, 1, 0), cvmGet(this->eigen_vectors_, 0, 0));

  __END__;
  __ISL_CHECK_ERROR__;
}


void 
PrincipalAxis::computeMajorMinorPositions(const double center_x, const double center_y, const int width, const int height)
{
	CV_FUNCNAME( "PrincipalAxis::computeMajorMinorPositions" );
	__BEGIN__;

	int p1_x = -1;
	int p1_y = -1;
	int p2_x = -1;
	int p2_y = -1;
	const bool bGetX = false;
	const bool bGetY = true;
	const bool bMajor = true;
	const bool bMinor = false;
	double x = 0.0;
	double y = 0.0;

	centroide_= Point2D<int>(center_x,center_y);
	width_= width;
	height_ = height;

	bool bAxe = bMajor;
	do
	{
		p1_x = -1;
		p1_y = -1;
		p2_x = -1;
		p2_y = -1;

		double y0,yw,x0,xh;
		//Compute P1
		//x=0; y ?
		y0 = getValueOnAxisEllipse(bAxe, bGetY, 0);
		//x=pixel_size_x; y ?
		yw = getValueOnAxisEllipse(bAxe, bGetY, (double)width_);
		//y=0; x ?
		x0 = ceil(getValueOnAxisEllipse(bAxe, bGetX, 0));
		//y=config.pixel_size_y; x ?
		xh = ceil(getValueOnAxisEllipse(bAxe, bGetX, (double)height_));
		
		bool firstPointFound=false;
		bool secondPointFound=false;

		if( y0 >= 0 && y0 <= height_ )
		{
			
				p1_x = 0;
				p1_y = y0;
				firstPointFound = true;		
		}

		if( yw >= 0 && yw <= height_ )
		{
			if (!firstPointFound)
			{
				p1_x = width_;
				p1_y = yw;
				firstPointFound = true;
			}
			else if (!secondPointFound)
			{
				p2_x = width_;
				p2_y = yw;
				secondPointFound = true;
			}
		}

		if( x0 >= 0 && x0 <= width_ )
		{
			if (!firstPointFound)
			{
				p1_x = x0;
				p1_y = 0;
				firstPointFound = true;
			}
			else if (!secondPointFound)
			{
				p2_x = x0;
				p2_y = 0;
				secondPointFound = true;
			}
		}
		if( xh >= 0 && xh <= width_ )
		{
			if (!firstPointFound)
			{
				p1_x = xh;
				p1_y = height_;
				firstPointFound = true;
			}
			else if (!secondPointFound)
			{
				p2_x = xh;
				p2_y = height_;
				secondPointFound = true;
			}
		}


		bool reverse = false;
		if(p1_x > p2_x)
		{
			reverse = true;
		}

		if(bAxe == bMajor )
		{
			if (major_)
			{
				//TODO : Debug printing, remove
				std::cerr << "double allocation of the major axis" << std::endl;
				delete major_;
			}
	
			major_ = new Axis( Point2D<int>(!reverse ? p1_x : p2_x, !reverse ? p1_y : p2_y),
				Point2D<int>(!reverse ? p2_x : p1_x, !reverse ? p2_y : p1_y)); 
		}
		else
		{
			if (minor_)
			{
				//TODO : Debug printing, remove
				std::cerr << "double allocation of the minor axis" << std::endl;
				delete minor_;
			}

			minor_ = new Axis( Point2D<int>(!reverse ? p1_x : p2_x, !reverse ? p1_y : p2_y),
				Point2D<int>(!reverse ? p2_x : p1_x, !reverse ? p2_y : p1_y)); 
		}

		//compute switch axis
		bAxe = !bAxe;
	} 
	while(bAxe == bMinor);

	__END__;
	__ISL_CHECK_ERROR__;

}

double
PrincipalAxis::getValueOnAxisEllipse(const bool isMajor, const bool isGetY, const double  value) const
{
	CV_FUNCNAME( "PrincipalAxis::getValueOnAxisEllipse" );
	__BEGIN__;

	double result = -1;

	if(isMajor)
	{
		if(isGetY)
		{
			// compute Y
			result = tan(angle_) * (value - centroide_.x()) + centroide_.y() ;
		}
		else
		{
			// compute X
			
			if (fabs(angle_)>1E-6)
				result =  centroide_.x() + (value - centroide_.y()) / tan(angle_);
			else
				result = centroide_.y();
		}
	}
	else
	{
		if(isGetY)
		{
			// compute Y
			if (fabs(angle_)>1E-6)
				result = - ( value - centroide_.x()) / tan(angle_) + centroide_.y();
			else
				result =  centroide_.x();
		}
		else
		{
			// compute X
			result = centroide_.x()- tan(angle_) * ( value - centroide_.y() )   ;
		}

	}

	return result;

	__END__;
	__ISL_CHECK_ERROR__;
}


void 
PrincipalAxis::computeMajorAndMinorAxis() 
{
	CV_FUNCNAME( "PrincipalAxis::computeMajorAndMinorAxis" );
	__BEGIN__;

	int leftPoint_x = -1;
	int leftPoint_y = -1;
	int rightPoint_x = -1;
	int rightPoint_y = -1;
	bool bReverse = false;
	const bool bMajor = true;
	const bool bMinor = false;
	const bool bGetY = true;
/*
	const double* major = major_->getValues();
	if(major)
	{
		for (int i = major_.getOrigin().x(); i < major_.getEnd().x(); i++)
		{
			major[i] = getValueOnAxisEllipse(bMajor, bGetY, i);
		}
	}
	double* minor = minor_.getValues();
	if(minor)
	{
		for (int i = minor_.getOrigin().x(); i < minor_.getEnd().x(); i++)
		{
			minor[i] = getValueOnAxisEllipse(bMinor, bGetY, i);
		}
	}

*/
	__END__;
	__ISL_CHECK_ERROR__;
}

}
