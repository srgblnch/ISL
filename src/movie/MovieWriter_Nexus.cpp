//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************

#include "MovieWriter_Nexus.h"

#include <isl/Image.h>
#include "InternalErrorHandler.h"

#include <napi.h>

#include <sstream>
#include <iostream>
#include <stdint.h>

#ifdef DEBUG_NEXUS
# define DBGOUT std::cout << "ISL_LOG: "
#else
# define DBGOUT /*if (false)*/ std::cout << "ISL_LOG: "
#endif

#define throw_isl(status, fn, desc) \
    if (true) { \
        isl::Exception exc; \
        isl::Error err(status, fn, desc, __FILE__, __LINE__); \
        exc.errors.push_back(err); \
        throw exc; \
    } else (void)0

#define NXdo_fd(fn, ...) \
    if (true) { \
        DBGOUT << " NX" << #fn << std::flush; \
        if (NX ## fn(__VA_ARGS__) != NX_OK) { \
            DBGOUT << " MEC" << std::endl; \
            throw_isl(0, __FUNCTION__, "Nexus problems running "#fn); \
        } \
        std::cout << " OK" << std::endl; \
    } else (void)0

#define NXdo(fn, ...) NXdo_fd(fn, this->m_nexusHandle, ## __VA_ARGS__)

#define DEPTH 0
#define HEIGHT 1
#define WIDTH 2

namespace isl
{

  MovieWriter_Nexus::~MovieWriter_Nexus()
  {
    DBGOUT << "MovieWriter_Nexus::~MovieWriter_Nexus()" << std::endl;
        NXdo_fd(close, &this->m_nexusHandle);

  }

  MovieWriter_Nexus::MovieWriter_Nexus( const MovieConfig& config )
    : MovieWriter(config),
      m_nexusHandle(0),
      m_frameCount(0),
      m_headWritten(false)
  {
    DBGOUT << "MovieWriter_Nexus::MovieWriter_Nexus()" << std::endl;
  }

std::string get_iso8601_time()
{
    char buf[255];

    DBGOUT << "MovieWriter_Nexus::get_iso8601_time()" << std::endl;
    time_t rawtime;
    time ( &rawtime );
    strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", gmtime(&rawtime));

    return std::string(buf);
}

void MovieWriter_Nexus::write_head(int width, int height)
{
    m_headWritten = true;

    //DBGOUT << "MovieWriter_Nexus::write_head()" << std::endl;
    std::stringstream fname;
    fname << config.file_basename << "." << config.format;
    DBGOUT << "MovieWriter_Nexus::write_head() filename: " << fname.str().c_str() << std::endl;

    if (config.format == "xml")
        NXdo_fd(open, fname.str().c_str(), NXACC_CREATEXML, &this->m_nexusHandle);
    else
        NXdo_fd(open, fname.str().c_str(), NXACC_CREATE5, &this->m_nexusHandle);

    m_shape[DEPTH] = NX_UNLIMITED;
    m_shape[WIDTH] = width;
    m_shape[HEIGHT] = height;

    int shape;
    NXlink data_id;

    /// @todo
    NXdo(putattr, "user_name", (void*)("isl"), 3, NX_CHAR);

    std::string entry_name = "entry1";// @todo if file exist add new entries
    NXdo(makegroup, entry_name.c_str(), "NXentry");
    NXdo(opengroup, entry_name.c_str(), "NXentry");

        NXdo(makegroup, "instrument", "NXinstrument");
        NXdo(opengroup, "instrument", "NXinstrument");

            std::string start_time = get_iso8601_time();
            shape = start_time.size();
            NXdo(makedata, "start_time", NX_CHAR, 1, &shape);
            NXdo(opendata, "start_time");
                NXdo(putdata, (void*)start_time.c_str());
            NXdo(closedata);

            NXdo(makegroup, "basler", "NXdetector"); // @todo this must include
            NXdo(opengroup, "basler", "NXdetector"); // the ccd model
                std::stringstream ss;
                ss << this->config.bit_depth;
                std::string bit_depth = ss.str();
                NXdo(putattr, "bit_depth", (void*)bit_depth.c_str(), bit_depth.size(), NX_CHAR);

                NXdo(makedata, "data", NX_UINT16, 3, m_shape);
                NXdo(opendata, "data");
                    NXdo(getdataID, &data_id);
                NXdo(closedata);
            NXdo(closegroup);
        NXdo(closegroup);
        NXdo(makegroup, "data", "NXdata");
        NXdo(opengroup, "data", "NXdata");
            int32_t signal = 1;
            NXdo(putattr, "signal", &signal, 1, NX_INT32);
            // NXdo(putattr, "axes", &signal, 1, NX_CHAR);
            // NXdo(putattr, "long_name", 
            NXdo(makelink, &data_id);
        NXdo(closegroup);
        NXdo(opengroup, "instrument", "NXinstrument");
            NXdo(opengroup, "basler", "NXdetector");
                NXdo(opendata, "data");
}


void MovieWriter_Nexus::write_frame( const isl::Image& frame )
{

    DBGOUT << "MovieWriter_Nexus::write_frame() #" << this->m_frameCount << std::endl;
    /// Why not just calling write_head() on the constructor?
    /// apparently NeXus ignores the data that is appended from
    /// a thread different to were NXopen was done.
    /// This way it is more likely that it will be the case

    if (!m_headWritten)
        this->write_head(frame.width(), frame.height());

    CV_FUNCNAME( "MovieWriter_Nexus::write_frame" );
    __BEGIN__;

    int start[] = { 0, 0, 0};
    int size[] = { 1, 0, 0 };

    unsigned char* input;
    int input_step;
    
    if (frame.depth() != ISL_STORAGE_USHORT) {
      throw_isl( 0,
                 "MovieWriter_Nexus::write_frame",
                 "Only ISL_STORAGE_USHORT depth for images is supported now.");
    }

    CV_CALL( cvGetRawData(frame.get_ipl_image(), &input, &input_step) );

    start[0] = this->m_frameCount;
    size[2] = frame.width();
    size[1] = frame.height();

    if ( (size[1] != m_shape[HEIGHT]) != (size[2] != m_shape[WIDTH]) ) {
      throw_isl( 0,
                 "MovieWriter_Nexus::write_frame",
                 "Image size should not change while saving video.");
    }

    NXdo(putslab, (void*)input, start, size);

    ++this->m_frameCount;

    __END__;
    __ISL_CHECK_ERROR__;
}

}
