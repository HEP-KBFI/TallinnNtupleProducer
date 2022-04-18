#ifndef TallinnNtupleProducer_CommonTools_isHigherPt_h
#define TallinnNtupleProducer_CommonTools_isHigherPt_h

/**
   * @brief Auxiliary function used for sorting hadronic taus and jets by decreasing pT
   * @param Given pair of either hadronic taus or jets
   * @return True, if first particle has higher pT; false if second particle has higher pT
   */
template <typename T>
bool
isHigherPt(const T * particle1, const T * particle2)
{
  return particle1->pt() > particle2->pt();
}

#endif // TallinnNtupleProducer_CommonTools_isHigherPt_h

