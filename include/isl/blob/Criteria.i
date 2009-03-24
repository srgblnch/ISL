namespace isl {

ISL_INLINE
Criteria::operator Criteria*() const
{
  return (Criteria*)this;
}

}
