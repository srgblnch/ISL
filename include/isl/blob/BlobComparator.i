namespace isl {

ISL_INLINE
bool
BlobComparator::operator()(Blob& _b1, Blob& _b2)
{
  return( (*op_)( (*crit_)(_b1), (*crit_)(_b2) ) );
}

ISL_INLINE
BlobComparator::operator BlobComparator*() const
{
  return (BlobComparator*)this;
}

}
