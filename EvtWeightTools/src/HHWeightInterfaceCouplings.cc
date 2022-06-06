#include "TallinnNtupleProducer/EvtWeightTools/interface/HHWeightInterfaceCouplings.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException(), get_human_line()
#include "TallinnNtupleProducer/CommonTools/interface/contains.h"                 // contains()
#include "TallinnNtupleProducer/CommonTools/interface/format_vT.h"                // format_vstring()
#include "TallinnNtupleProducer/CommonTools/interface/get_fullpath.h"             // get_fullpath()
#include "TallinnNtupleProducer/CommonTools/interface/TFileOpenWrapper.h"         // TFileOpenWrapper
#include "TallinnNtupleProducer/CommonTools/interface/to_string_with_precision.h" // to_string_with_precision()

#include <TFile.h>                                                                // TFile
#include <TH2.h>                                                                  // TH2

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
#if defined(__OPTIMIZE__)
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
#include <boost/algorithm/string.hpp>                                             // boost::split(), boost::is_any_of(), boost::trim_copy()
#pragma GCC diagnostic pop

#include <boost/algorithm/string/replace.hpp>                                     // boost::replace_all()
#include <boost/range/adaptor/map.hpp>                                            // boost::adaptors::map_keys
#include <boost/range/algorithm/copy.hpp>                                         // boost::copy()
#include <boost/algorithm/string/join.hpp>                                        // boost::algorithm::join()

#include <fstream>                                                                // std::ifstream
#include <iostream>                                                               // std::cout

const std::map<std::string, double> HHWeightInterfaceCouplings::couplings_sm_ = {
  { "kl",  1 },
  { "kt",  1 },
  { "c2",  0 },
  { "cg",  0 },
  { "c2g", 0 },
};

TH2 *
HHWeightInterfaceCouplings::loadDenominatorHist(const std::string & fileName,
                                                const std::string & histTitle)
{
  const std::string fileNameFullPath = get_fullpath(fileName);
  TFile * denomFile = TFileOpenWrapper::Open(fileNameFullPath.c_str(), "READ");
  if(! denomFile)
  {
    throw cmsException(__func__, __LINE__)
      << "Could not open file " << fileNameFullPath << " !!\n";
  }
  if(denomFile -> IsZombie())
  {
    throw cmsException(__func__, __LINE__) << "The file '" << fileNameFullPath << "' appears to be a zombie";
  }
  TH2 * denomHist = static_cast<TH2 *>(denomFile -> Get(histTitle.c_str()));
  if(! denomHist)
  {
    throw cmsException(__func__, __LINE__) << "The file '" << fileNameFullPath << "' does not have a TH2 named " << histTitle;
  }
  denomHist->SetDirectory(0);
  denomFile->Close();
  return denomHist;
}

double
HHWeightInterfaceCouplings::getBinContent(const TH2 * const hist,
                                          double mHH,
                                          double cosThetaStar)
{
  int mHH_Bin = hist->GetXaxis()->FindBin(mHH);
  int cosThetaStar_Bin = hist->GetYaxis()->FindBin(std::fabs(cosThetaStar));
  assert ( mHH_Bin > 0 && cosThetaStar_Bin > 0 );
  const double value = hist->GetBinContent(
    mHH_Bin,
    cosThetaStar_Bin
  );
  return value;
}

HHWeightInterfaceCouplings::HHWeightInterfaceCouplings(const edm::ParameterSet & cfg)
  : nlo_mode_(HHWeightInterfaceNLOMode::none)
  , denominator_file_lo_(cfg.getParameter<std::string>("denominator_file_lo"))
  , denominator_file_nlo_(cfg.getParameter<std::string>("denominator_file_nlo"))
  , histtitle_(cfg.getParameter<std::string>("histtitle"))
{
  const std::string applicationLoadFile_jhep04Scan = cfg.getParameter<std::string>("JHEP04Scan_file");
  const std::string applicationLoadFile_jhep03Scan = cfg.getParameter<std::string>("JHEP03Scan_file");
  const std::string applicationLoadFile_klScan = cfg.getParameter<std::string>("klScan_file");
  const std::string applicationLoadFile_ktScan = cfg.getParameter<std::string>("ktScan_file");
  const std::string applicationLoadFile_c2Scan = cfg.getParameter<std::string>("c2Scan_file");
  const std::string applicationLoadFile_cgScan = cfg.getParameter<std::string>("cgScan_file");
  const std::string applicationLoadFile_c2gScan = cfg.getParameter<std::string>("c2gScan_file");
  const std::string applicationLoadFile_extraScan = cfg.getParameter<std::string>("extraScan_file");

  const std::vector<std::string> scanMode = cfg.getParameter<std::vector<std::string>>("scanMode");
  const bool isDEBUG = cfg.getParameter<bool>("isDEBUG");

  const std::string rwgt_nlo_mode = cfg.getParameter<std::string>("rwgt_nlo_mode");
  if     (rwgt_nlo_mode == "v1") { nlo_mode_ = HHWeightInterfaceNLOMode::v1; }
  else if(rwgt_nlo_mode == "v2") { nlo_mode_ = HHWeightInterfaceNLOMode::v2; }
  else if(rwgt_nlo_mode == "v3") { nlo_mode_ = HHWeightInterfaceNLOMode::v3; }

  couplings_ = { { "SM", HHCoupling() } };

  // Load a file with an specific scan, that we can decide at later stage on the analysis
  // save the closest shape BM to use this value on the evaluation of a BDT
  if(contains(scanMode, "JHEP04") && ! applicationLoadFile_jhep04Scan.empty()){
    const std::string applicationLoadPath_jhep04Scan = get_fullpath(applicationLoadFile_jhep04Scan);
    loadScanFile(applicationLoadPath_jhep04Scan, isDEBUG);
  }
  if(contains(scanMode, "JHEP03") && ! applicationLoadFile_jhep03Scan.empty()){
    const std::string applicationLoadPath_jhep03Scan = get_fullpath(applicationLoadFile_jhep03Scan);
    loadScanFile(applicationLoadPath_jhep03Scan, isDEBUG);
  }
  if(contains(scanMode, "kl") && ! applicationLoadFile_klScan.empty()){
    const std::string applicationLoadPath_klScan = get_fullpath(applicationLoadFile_klScan);
    loadScanFile(applicationLoadPath_klScan, isDEBUG);
  }
  if(contains(scanMode, "kt") && ! applicationLoadFile_ktScan.empty())
  {
    const std::string applicationLoadPath_ktScan = get_fullpath(applicationLoadFile_ktScan);
    loadScanFile(applicationLoadPath_ktScan, isDEBUG);
  }
  if(contains(scanMode, "c2") && ! applicationLoadFile_c2Scan.empty())
  {
    const std::string applicationLoadPath_c2Scan = get_fullpath(applicationLoadFile_c2Scan);
    loadScanFile(applicationLoadPath_c2Scan, isDEBUG);
  }
  if(contains(scanMode, "cg") && ! applicationLoadFile_cgScan.empty())
  {
    const std::string applicationLoadPath_cgScan = get_fullpath(applicationLoadFile_cgScan);
    loadScanFile(applicationLoadPath_cgScan, isDEBUG);
  }
  if(contains(scanMode, "c2g") && ! applicationLoadFile_c2gScan.empty())
  {
    const std::string applicationLoadPath_c2gScan = get_fullpath(applicationLoadFile_c2gScan);
    loadScanFile(applicationLoadPath_c2gScan, isDEBUG);
  }
  if(contains(scanMode, "extra") && ! applicationLoadFile_extraScan.empty())
  {
    const std::string applicationLoadPath_extraScan = get_fullpath(applicationLoadFile_extraScan);
    loadScanFile(applicationLoadPath_extraScan, isDEBUG);
  }

  std::cout
      << get_human_line(this, __func__, __LINE__)
      << "Scanning " << couplings_.size() << " benchmark scenarios: " << format_vstring(get_bm_names())
      << '\n'
  ;
}

void
HHWeightInterfaceCouplings::loadScanFile(const std::string & filePath,
                                         bool isDEBUG)
{
  // CV: read coupling scans from text files
  std::ifstream inFile_scan(filePath);
  if(! inFile_scan)
  {
    throw cmsException(this, __func__, __LINE__) << "Error on opening file " << filePath;
  }
  for (std::string line; std::getline(inFile_scan, line); )
  {
    const std::size_t comment_pos = line.find_first_of("#");
    const std::string line_before_comment = boost::trim_copy(comment_pos != std::string::npos ? line.substr(0, comment_pos) : line);
    if(line_before_comment.empty())
    {
      continue;
    }
    std::vector<std::string> line_split;
    boost::split(line_split, line_before_comment, boost::is_any_of(" "), boost::token_compress_on);

    std::string name = "";
    std::string training = "SM"; // use SM training by default
    std::map<std::string, double> couplings = couplings_sm_;

    for(const std::string & line_part: line_split)
    {
      std::vector<std::string> part_split;
      boost::split(part_split, line_part, boost::is_any_of("="));
      if(part_split.size() != 2)
      {
        throw cmsException(this, __func__, __LINE__) << "Invalid number of '=' characters in: " << line_part;
      }
      const std::string attr = part_split[0];
      if(attr == "name")
      {
        name = part_split[1];
      }
      else if(attr == "training")
      {
        training = part_split[1];
      }
      else
      {
        if(! couplings.count(attr))
        {
          throw cmsException(this, __func__, __LINE__) << "Invalid coupling: " << attr;
        }
        const std::string coupling_val = part_split[1];
        const std::size_t nof_div_ops = std::count(coupling_val.cbegin(), coupling_val.cend(), '/');
        if(nof_div_ops == 0)
        {
          // no division operators
          couplings[attr] = std::stod(coupling_val);
        }
        else if(nof_div_ops == 1)
        {
          // one division
          std::vector<std::string> coupling_val_split;
          boost::split(coupling_val_split, coupling_val, boost::is_any_of("/"));
          assert(coupling_val_split.size() == 2);
          const double num = std::stod(coupling_val_split[0]);
          const double denom = std::stod(coupling_val_split[1]);
          if(std::fpclassify(denom) == FP_ZERO)
          {
            throw cmsException(this, __func__, __LINE__) << "Division by zero in: " << coupling_val;
          }
          couplings[attr] = num / denom;
        }
        else
        {
          throw cmsException(this, __func__, __LINE__) << "Invalid expression for coupling " << attr << ": " << coupling_val;
        }
      }
    }
    if(name.empty())
    {
      std::vector<std::string> parts;
      for(const std::string & coupling: { "kl", "kt", "c2", "cg", "c2g" })
      {
        const double & coupling_val = couplings.at(coupling);
        if(coupling_val == couplings_sm_.at(coupling))
        {
          continue; // SM value
        }
        std::string name_part = coupling + "_" + to_string_with_precision(coupling_val);
        boost::replace_all(name_part, "-", "m");
        boost::replace_all(name_part, ".", "p");
        parts.push_back(name_part);
      }
      name = parts.empty() ? "SM" : boost::algorithm::join(parts, "_");
    }

    assert(! couplings_.count(name));
    couplings_[name] = { couplings, name, training };
    std::cout << "Loaded coupling: " << couplings_.at(name) << '\n';
  }
}

void
HHWeightInterfaceCouplings::add(const HHCoupling & coupling)
{
  if(couplings_.count(coupling.name()))
  {
    throw cmsException(this, __func__, __LINE__) << "The coupling name has already been booked: " << coupling;
  }
  couplings_[coupling.name()] = coupling;
}

const std::map<std::string, HHCoupling> &
HHWeightInterfaceCouplings::getCouplings() const
{
  return couplings_;
}

const HHCoupling &
HHWeightInterfaceCouplings::getCoupling(const std::string & name) const
{
  if(! couplings_.count(name))
  {
    throw cmsException(this, __func__, __LINE__) << "No such coupling booked: " << name;
  }
  const HHCoupling & coupling = couplings_.at(name);
  assert(name == coupling.name());
  return coupling;
}

std::vector<std::string>
HHWeightInterfaceCouplings::get_bm_names() const
{
  std::vector<std::string> bmNames;
  boost::copy(couplings_ | boost::adaptors::map_keys, std::back_inserter(bmNames));
  return bmNames;
}

std::vector<std::string>
HHWeightInterfaceCouplings::get_weight_names() const
{
  std::vector<std::string> weightNames;
  std::transform(
    couplings_.cbegin(), couplings_.cend(), std::back_inserter(weightNames),
    [](const std::pair<std::string, HHCoupling> & kv) -> std::string
    {
      return kv.second.weightName();
    }
  );
  return weightNames;
}

const HHWeightInterfaceNLOMode &
HHWeightInterfaceCouplings::nlo_mode() const
{
  return nlo_mode_;
}

const std::string &
HHWeightInterfaceCouplings::denominator_file_lo() const
{
  return denominator_file_lo_;
}

const std::string &
HHWeightInterfaceCouplings::denominator_file_nlo() const
{
  return denominator_file_nlo_;
}

const std::string &
HHWeightInterfaceCouplings::histtitle() const
{
  return histtitle_;
}
