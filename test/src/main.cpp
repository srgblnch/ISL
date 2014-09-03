//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#include <iostream>

#include <isl/ErrorHandler.h>
#include <isl/Image.h>
#include <isl/Exception.h>
#include <isl/geometry/Point.h>
#include <isl/calib/Calib.h>
#include <isl/statistics/AccurateCentroid.h>
#include <isl/statistics/GaussianFit2D.h>

#include <isl/blob/BlobList.h>
#include <isl/blob/BlobComparator.h>
#include <isl/blob/Criteria.h>
#include <isl/blob/Operator.h>
#include <algorithm>
#include <functional>

#include <isl/statistics/NoiseEstimator.h>

void test_points()
{
  isl::Point2D<double> p1(3,7);
  isl::Point2D<double> p2(9,2);

  std::cout << std::endl << std::endl;
  std::cout << "p1: " << p1.x() << " " << p1.y() << " " << std::endl;
  std::cout << "p2: " << p2.x() << " " << p2.y() << " " << std::endl;

  p1 += p2;

  std::cout << std::endl << std::endl;
  std::cout << "p1: " << p1.x() << " " << p1.y() << " " << std::endl;
  std::cout << "p2: " << p2.x() << " " << p2.y() << " " << std::endl;

  isl::Point2D<double> p5(p1);
  std::cout << "p5: " << p5.x() << " " << p5.y() << " " << std::endl;

  std::cout << "p1 * p2: " << p1 * p2 << std::endl;
  std::cout << "p1.norm_L2(): " << p1.norm_L2() << std::endl;
}


void test_calib(isl::Image& im)
{
  isl::ErrorHandler::init();

  isl::Calib calib;

  calib.config.x_spacing = 10;
  calib.config.y_spacing = 10;
  calib.config.calib_type = isl::ISL_CALIB_PERSP;
  calib.config.roi[0] = isl::Point2D<int>(50,50);
  calib.config.roi[1] = isl::Point2D<int>(570,50);
  calib.config.roi[2] = isl::Point2D<int>(30,424);
  calib.config.roi[3] = isl::Point2D<int>(592,407);


  try
  {
    calib.learn(im);
  }
  catch(isl::Exception& ex)
  {
    std::cout << ex;
    isl::ErrorHandler::reset();

    calib.preprocessed_image().save("preprocessed.pgm");

    isl::Image delaunay_im(im);
    calib.draw_delaunay(delaunay_im, 255);
    calib.draw_blobs(delaunay_im, 255);
    delaunay_im.save("delaunay.pgm");

    return;
  }


  std::cout << "mag factor [mm/pix]: " << calib.mag_factor() << std::endl;
  std::cout << "mean error [pix]   : " << calib.model_error()  << std::endl;


  isl::Image undist_im(im.width(), im.height(), isl::ISL_STORAGE_UCHAR);
  calib.undistort(im,
                  undist_im,
                  isl::ISL_INTERP_AREA);
  undist_im.save("undistorted.pgm");


  calib.preprocessed_image().save("preprocessed.pgm");

  isl::Image delaunay_im(im);
  calib.draw_delaunay(delaunay_im, 255);
  calib.draw_blobs(delaunay_im, 255);
  delaunay_im.save("delaunay.pgm");


  isl::Image error_im(undist_im);
  calib.draw_error_map(error_im, 30, 255);
  error_im.save("error_map.pgm");
 

}


void test_centroid(isl::Image& im)
{

  isl::AccurateCentroid ac;

  isl::Point2D<double> pt = ac.compute(im);
  
  std::cout << "Centroid : " << std::endl;
  std::cout << pt.x() << " , " << pt.y() << std::endl;

}

void test_gauss_fit(isl::Image& im)
{
  isl::GaussianFit2D gfit;

  im.convert(isl::ISL_STORAGE_DOUBLE);
  gfit.nb_iter(100);
  gfit.epsilon(1.0E-6);
  gfit.compute(im,128,128,128,1000,300,500,0);
  std::cout << "end" << std::endl;
}

void test_rotation(isl::Image& im)
{
  {
    isl::Image tmp(im);
    tmp.rotate_flip(isl::ISL_ROTATE_NONE_FLIP_NONE);
    tmp.save("ISL_ROTATE_NONE_FLIP_NONE.pgm");
  }
  {
    isl::Image tmp(im);
    tmp.rotate_flip(isl::ISL_ROTATE_90_FLIP_NONE);
    tmp.save("ISL_ROTATE_90_FLIP_NONE.pgm");
  }
  {
    isl::Image tmp(im);
    tmp.rotate_flip(isl::ISL_ROTATE_180_FLIP_NONE);
    tmp.save("ISL_ROTATE_180_FLIP_NONE.pgm");
  }
  {
    isl::Image tmp(im);
    tmp.rotate_flip(isl::ISL_ROTATE_270_FLIP_NONE);
    tmp.save("ISL_ROTATE_270_FLIP_NONE.pgm");
  }
  {
    isl::Image tmp(im);
    tmp.rotate_flip(isl::ISL_ROTATE_NONE_FLIP_H);
    tmp.save("ISL_ROTATE_NONE_FLIP_H.pgm");
  }
  {
    isl::Image tmp(im);
    tmp.rotate_flip(isl::ISL_ROTATE_90_FLIP_H);
    tmp.save("ISL_ROTATE_90_FLIP_H.pgm");
  }
  {
    isl::Image tmp(im);
    tmp.rotate_flip(isl::ISL_ROTATE_180_FLIP_H);
    tmp.save("ISL_ROTATE_180_FLIP_H.pgm");
  }
  {
    isl::Image tmp(im);
    tmp.rotate_flip(isl::ISL_ROTATE_270_FLIP_H);
    tmp.save("ISL_ROTATE_270_FLIP_H.pgm");
  }
}



void test_blobs(isl::Image& im)
{

  im.threshold(100, 255);
  im.morphology(isl::ISL_MORPHO_DILATE);

  isl::BlobList bl;
  isl::BlobList::iterator it;

  bl.detect(im);

  std::cout << "size : " << bl.size() << std::endl;

  std::cout << "iterator : " << std::endl;
  for (it = bl.begin(); it != bl.end(); it++)
  {
    std::cout << (*it).area() << std::endl;
  }
  /*std::cout << "counter : " << std::endl;
  for (i = 0; i < bl.size(); i++)
  {
    std::cout << (bl[i]).area() << std::endl;
  }*/

  /*
  std::sort(bl.begin(),
            bl.end(),
            bl.end(),
            isl::BlobComparator(isl::Area(), isl::Greater()));
            */

  std::sort(bl.begin(),
            bl.end(),
            isl::BlobComparator(isl::Area(), isl::Greater()));

  std::cout << "iterator : " << std::endl;
  for (it = bl.begin(); it != bl.end(); it++)
  {
    std::cout << (*it).area() << std::endl;
  }
  /*std::cout << "counter : " << std::endl;
  for (i = 0; i < bl.size(); i++)
  {
    std::cout << (bl[i]).area() << std::endl;
  }*/


}


int main( int argc, char **argv)
{
  isl::ErrorHandler::init();

  if (argc != 2)
    return(-1);

  isl::Image im(argv[1]);

  try
  {
    test_calib(im);
  }
  catch(isl::Exception& ex)
  {
    std::cout << ex;
  }

 
  return(0);
}

