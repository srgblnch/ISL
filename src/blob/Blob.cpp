#include <isl/blob/Blob.h>
#include "InternalErrorHandler.h"

#include <isl/Image.h>

#if !defined (__ISL_INLINE__)
# include <isl/blob/Blob.i>
#endif

#ifdef WIN32
//- ok, i know i'm used this in the initialiser list
#pragma warning( disable:4355)
#endif

namespace isl {


Blob::Blob()
: perimeter_(*this),
  area_(*this),
  ellipse_(*this),
  bounding_rect_(*this)
{
}

Blob::Blob(const CvSeq* _edges)
: perimeter_(*this),
  area_(*this),
  ellipse_(*this),
  bounding_rect_(*this)
{
  CV_FUNCNAME( "Blob::Blob(const CvSeq*)" );
  __BEGIN__;

  ISL_CALL( this->alloc() );
  ISL_CALL( this->copy_edges(_edges) );

  __END__;
  
  if( cvGetErrStatus() < 0 )
    this->release();

  __ISL_CHECK_ERROR__;

}

Blob::Blob( const Blob &_src )
: perimeter_(*this),
  area_(*this),
  ellipse_(*this),
  bounding_rect_(*this)
{
  CV_FUNCNAME( "Blob::Blob(const Blob &)" );
  __BEGIN__;

  ISL_CALL( this->alloc() );
  ISL_CALL( this->copy_edges(_src.edges_) );
  
  this->perimeter_.copy_data(_src.perimeter_);
  this->area_.copy_data(_src.area_);
  this->ellipse_.copy_data(_src.ellipse_);
  this->bounding_rect_.copy_data(_src.bounding_rect_);

  __END__;
  
  if( cvGetErrStatus() < 0 )
    this->release();

  __ISL_CHECK_ERROR__;

}

Blob&
Blob::operator=(const Blob &_src)
{
  CV_FUNCNAME( "Blob::operator=(const Blob &)" );
  __BEGIN__;

	if (this == &_src)
    EXIT;

  this->release();
  ISL_CALL( this->alloc() );
  ISL_CALL( this->copy_edges(_src.edges_) );

  this->perimeter_.copy_data(_src.perimeter_);
  this->area_.copy_data(_src.area_);
  this->ellipse_.copy_data(_src.ellipse_);
  this->bounding_rect_.copy_data(_src.bounding_rect_);

  __END__;
  __ISL_CHECK_ERROR__;
  
  return *this;
}



Blob::~Blob()
{
  this->release();
}



void
Blob::draw(Image& _im, double _intensity, int _thickness , int _line_type) const
{
  CV_FUNCNAME( "Blob::draw" );
  __BEGIN__;

  CV_CALL( cvDrawContours(_im.get_ipl_image(), this->edges_, cvScalar(_intensity), cvScalar(_intensity), 0, _thickness, _line_type) );

  __END__;
  __ISL_CHECK_ERROR__;
}


void
Blob::filter(int x_min, int y_min, int x_max, int y_max)
{
  CvMemStorage* new_storage = 0;
  CvSeq*        new_edges = 0;

  CV_FUNCNAME( "Blob::filter" );
  __BEGIN__;
  
  CV_CALL( new_storage = cvCreateMemStorage(0) );
  CV_CALL( new_edges   = cvCreateSeq(CV_SEQ_ELTYPE_POINT | CV_SEQ_KIND_CURVE, 
                                     sizeof(CvContour), 
                                     sizeof(CvPoint) , 
                                     new_storage) );


	CvSeqReader seq_reader;
	CvSeqWriter seq_writer;
	CvPoint current_edge;
  int i;

	CV_CALL( cvStartReadSeq( this->edges_, &seq_reader) );
	CV_CALL( cvStartAppendToSeq( new_edges, &seq_writer ) );

	for( i = 0; i < this->edges_->total; i++)
	{
		CV_READ_SEQ_ELEM ( current_edge , seq_reader);
    
    if ( current_edge.x >= x_min
         && current_edge.y >= y_min
         && current_edge.x <= x_max
         && current_edge.y <= y_max )
    {
      CV_WRITE_SEQ_ELEM( current_edge , seq_writer);
    }
	}

	CV_CALL( cvEndWriteSeq( &seq_writer ) );
  
  __END__;
  __ISL_CHECK_ERROR__;
}


void
Blob::alloc()
{
  CV_FUNCNAME( "Blob::alloc" );
  __BEGIN__;

  CV_CALL( this->storage_ = cvCreateMemStorage(0) );
  CV_CALL( this->edges_ = cvCreateSeq(CV_SEQ_ELTYPE_POINT | CV_SEQ_KIND_CURVE, sizeof(CvContour), sizeof(CvPoint) , this->storage_) );

  __END__;
  
  if( cvGetErrStatus() < 0 )
    cvReleaseMemStorage(&this->storage_);

  __ISL_CHECK_ERROR__;
}

void
Blob::release()
{
	cvClearSeq(this->edges_);
  cvReleaseMemStorage(&this->storage_);
  this->edges_ = NULL;
}

void
Blob::copy_edges(const CvSeq* _edges)
{
  CV_FUNCNAME( "Blob::copy_edges" );
  __BEGIN__;

	cvClearSeq(this->edges_);
  
  //- copy the given sequence
	CvSeqReader seq_reader;
	CvSeqWriter seq_writer;
	CvPoint current_edge;
  int i;

	CV_CALL( cvStartReadSeq( _edges, &seq_reader) );
	CV_CALL( cvStartAppendToSeq( this->edges_, &seq_writer ) );

	for( i = 0; i < _edges->total; i++)
	{
		CV_READ_SEQ_ELEM ( current_edge , seq_reader);
		CV_WRITE_SEQ_ELEM( current_edge , seq_writer );
	}

	CV_CALL( cvEndWriteSeq( &seq_writer ) );


  __END__;
  __ISL_CHECK_ERROR__;
}

}
