#ifndef TallinnNtupleProducer_Readers_EventInfoReader_h
#define TallinnNtupleProducer_Readers_EventInfoReader_h

#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h" // ReaderBase

// forward declarations
class TTree;
class EventInfo;

class EventInfoReader : public ReaderBase
{
 public:
  explicit EventInfoReader(bool read_genHiggsDecayMode = true,
                           bool read_puWeight = true);
  explicit EventInfoReader(EventInfo * info,
                           bool read_genHiggsDecayMode = true,
                           bool read_puWeight = true);
  ~EventInfoReader() override;

  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  void
  setEventInfo(EventInfo * info);

  void
  setTopPtRwgtBranchName(const std::string & branchName);

  std::string
  getTopPtRwgtBranchName() const;

 protected:
  bool read_genHiggsDecayMode_;
  bool read_puWeight_;

  EventInfo * info_;

  std::string branchName_topPtRwgt;

 public:
  const std::string branchName_run;
  const std::string branchName_lumi;
  const std::string branchName_event;
  const std::string branchName_genHiggsDecayMode;
  const std::string branchName_genWeight;
  const std::string branchName_LHEReweightingWeight;
  const std::string branchName_nLHEReweightingWeight;
  const std::string branchName_gen_mHH;
  const std::string branchName_gen_cosThetaStar;
  const std::string branchName_htxs_pt;
  const std::string branchName_htxs_y;
};

#endif // TallinnNtupleProducer_Readers_EventInfoReader_h
