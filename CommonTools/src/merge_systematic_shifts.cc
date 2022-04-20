#include "TallinnNtupleProducer/CommonTools/interface/merge_systematic_shifts.h"

#include "TallinnNtupleProducer/CommonTools/interface/contains.h" // contains()

void
merge_systematic_shifts(std::vector<std::string>& systematic_shifts, const std::vector<std::string>& to_add)
{
  for ( auto systematic_shift : to_add ) 
  {
    if ( !contains(systematic_shifts, systematic_shift) )
    {
      systematic_shifts.push_back(systematic_shift);
    }
  }
}
