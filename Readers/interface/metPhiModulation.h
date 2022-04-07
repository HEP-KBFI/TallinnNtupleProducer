#ifndef TallinnNtupleProducer_Readers_metPhiModulation_h
#define TallinnNtupleProducer_Readers_metPhiModulation_h

#include <utility> // std::pair

/**
 * @brief Auxiliary function to correct for modulation in MET phi variable.
 * @see https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETRun2Corrections#xy_Shift_Correction_MET_phi_modu
 */

// forward declarations
class EventInfo;
class RecoVertex;

enum class Era;

std::pair<double, double>
METXYCorr_Met_MetPhi(const EventInfo * const eventInfo,
                     const RecoVertex * const recoVertex,
                     Era year,
                     bool isUL = false,
                     bool ispuppi = false);

#endif // TallinnNtupleProducer_Readers_metPhiModulation_h
