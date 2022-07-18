#include "TallinnNtupleProducer/Objects/interface/printCollection.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // cmsException()
#include "TallinnNtupleProducer/Objects/interface/RecoElectron.h"     // RecoElectron
#include "TallinnNtupleProducer/Objects/interface/RecoLepton.h"       // RecoLepton
#include "TallinnNtupleProducer/Objects/interface/RecoMuon.h"         // RecoMuon

template<>
void
printCollection<RecoLepton>(const std::string & collection_name,
                            const std::vector<const RecoLepton *> & leptons)
{
  std::cout << " (#" << collection_name << " = " << leptons.size() << ")\n";
  for(std::size_t idxLepton = 0; idxLepton < leptons.size(); ++idxLepton)
  {
    std::cout << collection_name << " #" << idxLepton << ": ";
    const RecoLepton * const lepton = leptons[idxLepton];
    if(! lepton)
    {
      throw cmsException(__func__, __LINE__) << "Encountered a nullptr";
    }

    const RecoElectron * const electron = dynamic_cast<const RecoElectron *>(lepton);
    if(electron)
    {
      std::cout << (*electron) << '\n';
      continue;
    }

    const RecoMuon * const muon = dynamic_cast<const RecoMuon *>(lepton);
    if(muon)
    {
      std::cout << (*muon) << '\n';
      continue;
    }
  }
}
