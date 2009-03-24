namespace isl {


ISL_INLINE
double
Moments1D::m0() const
{
  return(cvGetSpatialMoment(const_cast<CvMoments*>(&this->cv_moments_), 0, 0));
}

ISL_INLINE
double
Moments1D::m1() const
{
  return(cvGetSpatialMoment(const_cast<CvMoments*>(&this->cv_moments_), 1, 0));
}

ISL_INLINE
double
Moments1D::m2() const
{
  return(cvGetSpatialMoment(const_cast<CvMoments*>(&this->cv_moments_), 2, 0));
}

ISL_INLINE
double
Moments1D::m3() const
{
  return(cvGetSpatialMoment(const_cast<CvMoments*>(&this->cv_moments_), 3, 0));
}

ISL_INLINE
double
Moments1D::mu0() const
{
  return(cvGetCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 0, 0));
}

ISL_INLINE
double
Moments1D::mu1() const
{
  return(cvGetCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 1, 0));
}

ISL_INLINE
double
Moments1D::mu2() const
{
  return(cvGetCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 2, 0));
}

ISL_INLINE
double
Moments1D::mu3() const
{
  return(cvGetCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 3, 0));
}

ISL_INLINE
double
Moments1D::eta0() const
{
  return(cvGetNormalizedCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 0, 0));
}

ISL_INLINE
double
Moments1D::eta1() const
{
  return(cvGetNormalizedCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 1, 0));
}

ISL_INLINE
double
Moments1D::eta2() const
{
  return(cvGetNormalizedCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 2, 0));
}

ISL_INLINE
double
Moments1D::eta3() const
{
  return(cvGetNormalizedCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 3, 0));
}

ISL_INLINE
double
Moments1D::get_spatial_moment(int order) const
{
  return(cvGetSpatialMoment(const_cast<CvMoments*>(&this->cv_moments_), order, 0));
}


ISL_INLINE
double
Moments1D::get_central_moment(int order) const
{
  return(cvGetCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), order, 0));
}


ISL_INLINE
double
Moments1D::get_normalized_central_moment(int order) const
{
  return(cvGetNormalizedCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), order, 0));
}










ISL_INLINE
double
Moments2D::m00() const
{
  return(cvGetSpatialMoment(const_cast<CvMoments*>(&this->cv_moments_), 0, 0));
}

ISL_INLINE
double
Moments2D::m01() const
{
  return(cvGetSpatialMoment(const_cast<CvMoments*>(&this->cv_moments_), 0, 1));
}

ISL_INLINE
double
Moments2D::m10() const
{
  return(cvGetSpatialMoment(const_cast<CvMoments*>(&this->cv_moments_), 1, 0));
}

ISL_INLINE
double
Moments2D::m20() const
{
  return(cvGetSpatialMoment(const_cast<CvMoments*>(&this->cv_moments_), 2, 0));
}

ISL_INLINE
double
Moments2D::m11() const
{
  return(cvGetSpatialMoment(const_cast<CvMoments*>(&this->cv_moments_), 1, 1));
}

ISL_INLINE
double
Moments2D::m02() const
{
  return(cvGetSpatialMoment(const_cast<CvMoments*>(&this->cv_moments_), 0, 2));
}

ISL_INLINE
double
Moments2D::m30() const
{
  return(cvGetSpatialMoment(const_cast<CvMoments*>(&this->cv_moments_), 3, 0));
}
ISL_INLINE
double
Moments2D::m21() const
{
  return(cvGetSpatialMoment(const_cast<CvMoments*>(&this->cv_moments_), 2, 1));
}

ISL_INLINE
double
Moments2D::m12() const
{
  return(cvGetSpatialMoment(const_cast<CvMoments*>(&this->cv_moments_), 1, 2));
}

ISL_INLINE
double
Moments2D::m03() const
{
  return(cvGetSpatialMoment(const_cast<CvMoments*>(&this->cv_moments_), 0, 3));
}

ISL_INLINE
double
Moments2D::mu00() const
{
  return(cvGetCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 0, 0));
}

ISL_INLINE
double
Moments2D::mu01() const
{
  return(cvGetCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 0, 1));
}

ISL_INLINE
double
Moments2D::mu10() const
{
  return(cvGetCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 1, 0));
}

ISL_INLINE
double
Moments2D::mu20() const
{
  return(cvGetCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 2, 0));
}

ISL_INLINE
double
Moments2D::mu11() const
{
  return(cvGetCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 1, 1));
}

ISL_INLINE
double
Moments2D::mu02() const
{
  return(cvGetCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 0, 2));
}

ISL_INLINE
double
Moments2D::mu30() const
{
  return(cvGetCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 3, 0));
}
ISL_INLINE
double
Moments2D::mu21() const
{
  return(cvGetCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 2, 1));
}

ISL_INLINE
double
Moments2D::mu12() const
{
  return(cvGetCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 1, 2));
}

ISL_INLINE
double
Moments2D::mu03() const
{
  return(cvGetCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 0, 3));
}


ISL_INLINE
double
Moments2D::eta00() const
{
  return(cvGetNormalizedCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 0, 0));
}

ISL_INLINE
double
Moments2D::eta01() const
{
  return(cvGetNormalizedCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 0, 1));
}

ISL_INLINE
double
Moments2D::eta10() const
{
  return(cvGetNormalizedCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 1, 0));
}

ISL_INLINE
double
Moments2D::eta20() const
{
  return(cvGetNormalizedCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 2, 0));
}

ISL_INLINE
double
Moments2D::eta11() const
{
  return(cvGetNormalizedCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 1, 1));
}

ISL_INLINE
double
Moments2D::eta02() const
{
  return(cvGetNormalizedCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 0, 2));
}

ISL_INLINE
double
Moments2D::eta30() const
{
  return(cvGetNormalizedCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 3, 0));
}
ISL_INLINE
double
Moments2D::eta21() const
{
  return(cvGetNormalizedCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 2, 1));
}

ISL_INLINE
double
Moments2D::eta12() const
{
  return(cvGetNormalizedCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 1, 2));
}

ISL_INLINE
double
Moments2D::eta03() const
{
  return(cvGetNormalizedCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), 0, 3));
}

ISL_INLINE
double
Moments2D::get_spatial_moment(int x_order, int y_order) const
{
  return(cvGetSpatialMoment(const_cast<CvMoments*>(&this->cv_moments_), x_order, y_order));
}

ISL_INLINE
double
Moments2D::get_central_moment(int x_order, int y_order) const
{
  return(cvGetCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), x_order, y_order));
}


ISL_INLINE
double
Moments2D::get_normalized_central_moment(int x_order, int y_order) const
{
  return(cvGetNormalizedCentralMoment(const_cast<CvMoments*>(&this->cv_moments_), x_order, y_order));
}


}
