#include "TallinnNtupleProducer/CommonTools/interface/TFileOpenWrapper.h"

#include <TFile.h>    // TFile
#include <TPRegexp.h> // TPRegexp
#include <TString.h>  // TString

#include <cstring>    // std::strcmp()
#include <iostream>   // std::cout

namespace TFileOpenWrapper
{
/**
   * @brief Opens the given path via the hdfs protocol if possible
   *        If not possible, resort to built-in ROOT
   * @param path     URI, the path to open
   * @param option   Options (e.g. in which mode the file is opened)
   * @param ftitle   Title???
   * @param compress Compression level and algorithm
   * @return Pointer to a TFile * instance
   *
   * @note Use TFileOpenWrapper::Close() to close the file
   */
  TFile *
  Open(const char * path,
       Option_t * option,
       const char * ftitle,
       Int_t compress)
  {
    TFile * f = nullptr;
    TString path_str(path);
    TString option_str(option);
    option_str.ToUpper();

    const TString hdfs_protocol_prefix("hdfs://");
    // if the mode is empty or set to read, and the given path starts
    // with /hdfs, we prepend the path suitable for the hdfs protocol
    // (we want to avoid opening files on /hdfs for writing)
    const TString hdfs_path_prefix("/hdfs");
    TPRegexp hdfs_path_regex("^" + hdfs_path_prefix);
    if(path_str(hdfs_path_regex) != "" && (option_str == "" || option_str == "READ"))
    {
      // here we change the path: /hdfs/A/B/C/... -> hdfs:///A/B/C/...
      path_str.Replace(0, hdfs_path_prefix.Length(), hdfs_protocol_prefix);
    }

    f = TFile::Open(path_str, option, ftitle, compress);
    if(f && f -> IsZombie())
    {
      delete f;
      f = nullptr;
    }

    return f;
  }

  /**
   * @brief Closes given TFile
   * @param filePtr Pointer to the TFile instance which will be deleted
   */
  void
  Close(TFile * & filePtr)
  {
    if(filePtr)
    {
      filePtr -> Close();
      delete filePtr;
      filePtr = nullptr;
    }
  }
}
