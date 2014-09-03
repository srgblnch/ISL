//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#ifndef __INCLUDE_BLOBLIST_H__
#define __INCLUDE_BLOBLIST_H__

#include <isl/blob/Blob.h>
#include <isl/blob/Filter.h>
#include <isl/blob/Criteria.h>
#include <isl/blob/Operator.h>
#include <isl/blob/BlobComparator.h>

#include <deque>

namespace isl {

class Image;

/**
 *  \addtogroup blob
 *  @{
 */

#ifdef WIN32
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif

/**
 *  \brief Class used hold a list of Blob
 *
 *  This class inherits std::deque<Blob> and thus provide all the functionnalities
 *  of a std::deque ([] operator, iterators,...).
 *  
 *  STL algorithms are also supported. Predicates for these algorithms can be formed
 *  with the BlobComparator class together with a Criteria (see \ref blob_criteria) 
 *  and an Operator (see \ref blob_operators).
 *
 *  \author Julien Malik
 *  \date 08-2006
 */
class ISL_DECL BlobList : public std::deque<Blob>
{
  public:

    /**
     *  \brief Constructor
     */
    BlobList();

    /**
     *  \brief Constructor with initialization from an image
     *
     *  This function constructs the BlobList and calls BlobList::detect
     *
     *  \exception Exception
     */
    BlobList(Image& image);

    /**
     *  \brief Destructor
     */
    ~BlobList();

    /**
     *  \brief Draws all blobs of the list in the image
     *
     *  \exception Exception
     */
    void draw(Image& im, double intensity, int thickness = 1 , int line_type = ISL_LINETYPE_4) const;

    /**
     *  \brief Main function for detecting the blobs of a 8-bit binary image 
     *         (non-zero pixels are treated as 1's, zero pixels remains 0's)
     *
     *  \exception Exception
     *  \warning The function modifies the source image content
     */
    void detect(Image& image);

    /**
     *  \brief Keep only the blobs satisfying a Filter (see \ref blob_comparison)
     *
     *  Look at the code of BeamBox::compute and PatternAnalyzer::extract_blobs 
     *  to see an example of use of this function
     */
    void filter(Filter*, BlobList&);
    
    /**
     *  \brief Keep only the blobs satisfying a Filter (see \ref blob_comparison)
     *
     *  Look at the code of BeamBox::compute and PatternAnalyzer::extract_blobs 
     *  to see an example of use of this function
     */
    void filter(Filter*);

    /**
     *  \brief Keep only the blobs satisfying a Criteria and an Operator (see \ref blob_criteria and \ref blob_operators)
     *
     *  Look at the code of BeamBox::compute and PatternAnalyzer::extract_blobs 
     *  to see an example of use of this function
     */
    void filter(Criteria*, Operator*, BlobList&);
    
    /**
     *  \brief Keep only the blobs satisfying a Criteria and an Operator (see \ref blob_criteria and \ref blob_operators)
     *
     *  Look at the code of BeamBox::compute and PatternAnalyzer::extract_blobs 
     *  to see an example of use of this function
     */
    void filter(Criteria*, Operator*);
};
/** @} */

#ifdef WIN32
#pragma warning( pop )
#endif

}

#endif
