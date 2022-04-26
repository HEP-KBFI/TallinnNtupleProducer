#ifndef TallinnNtupleProducer_Framework_TTreeWrapper_h
#define TallinnNtupleProducer_Framework_TTreeWrapper_h

#include <algorithm>   // std::transform()
#include <string>      // std::string
#include <type_traits> // std::is_base_of, std::enable_if
#include <vector>      // std::vector<>

// forward declarations
class ReaderBase;
class TFile;
class TTree;

/**
 * @brief Alternative class to TChain for reading
 */
class TTreeWrapper
{
 private:
  /**
  * @brief Simple constructor for TTreeWrapper
  *
  * @note Initializes its private member variables to default (null) values
  * @note Although private, it will throw for sure
  */
  TTreeWrapper();

 public:
  /**
   * @brief Default constructor for TTreeWrapper
   * @param treeName  The name of the input TTree
   * @param fileNames List of file names to be read
   *
   * @note Will throw if treeName is an empty string or if the vector
   *       fileNames does not contain any elements
   */
  TTreeWrapper(const std::string & treeName,
               const std::vector<std::string> & fileNames,
               long long maxEvents = -1);

  /**
   * @brief Cleanup
   */
  ~TTreeWrapper();

  /**
   * @brief Restart processing of all events from the first event in the first file
   */
  void
  reset();

  /**
   * @brief Return the total number of available files
   * @return The number of files to be read
   */
  int
  getFileCount() const;

  /**
   * @brief Returns the cumulative event index across all files
   * @return The index
   *
   * @note e.g. if we have completed processing all 1000 events in
   *       the first file and currently processing the second file
   *       at index 200, this function would return 1199 as the index
   *       (because the indices start at 0)
   */
  long long
  getCurrentMaxEventIdx() const;

  /**
   * @brief Returns cumulative event count
   * @return The cumulative event count
   *
   * @note By ,,cumulative event count'' we mean the sum of entries
   *       of all files processed. By default, we try to process all events
   *       in all files. But if the number of entries to be processed is
   *       bound by maxEvents_, the total cumulative number of max events
   *       and the total number of processed events will differ. For instance,
   *       let's assume that we have 1000 entries in the first three files, but
   *       maxEvents_ is set to 1500. This means that this function would return
   *       2000, getCurrentMaxEventIdx() would return 1499 (=1500 - 1),
   *       getEventCount() would return 3000 and getCurrentEventIdx would return
   *       499 (=1500 - 1000 - 1).
   */
  long long
  getCumulativeMaxEventCount() const;

  /**
   * @brief Return the number of files that have been processed
   * @return The number of files that have been processed
   *
   * @note The returned number may differ from the total number files
   *       returned by getFileCount() if maxEvents_ is other than -1.
   *       In other words, if we limit the number of events to be processed
   *       by some number, we won't actually analyze all files but only
   *       first few of them.
   */
  int
  getProcessedFileCount() const;

  /**
   * @brief Returns the index of event in currently open file
   * @return The index of event in currently open file
   *
   * @see Comment in getCumulativeMaxEventCount()
   */
  long long
  getCurrentEventIdx() const;

  /**
   * @brief Check if the user can report more detailed information about
   *        the event loop, given report interval
   * @param reportEvery The report interval
   * @return true,  if the user can report information about current event;
   *         false, otherwise
   */
  bool
  canReport(unsigned reportEvery) const;

  /**
   * @brief Checks if there are currently any open files
   * @return true,  if there is an input file open;
   *         false, otherwise
   */
  bool
  isOpen() const;

  /**
   * @brief Returns currently open file name
   * @return Currently open file name
   *
   * @note Use this function in conjunction with isOpen() to ensure that
   *       this class has been properly initialized and the file name is available
   */
  std::string
  getCurrentFileName() const;

  /**
   * @brief Register an object reader instance
   * @param reader Pointer to a class instance inheriting from ReaderBase
   * @return Reference to this object
   *
   * @note The reader object must implement setBranchAddresses() function
   * @note Expected usage:
   *         TTreeWrapper wrapper;
   *         wrapper.register(reader1)
   *                .register(reader2).
   *                // ...
   *                .register(readerN)
   *         ;
   * @note This class won't own the pointer since the user may pass a reference
   *       to this function. This implies that the class won't delete the pointers
   *       passed to this function.
   */
  TTreeWrapper &
  registerReader(ReaderBase * reader);

  /**
   * @brief Register object reader instances
   * @param readers Vector of pointers to a class instance inheriting from ReaderBase
   * @return Reference to this object
   *
   * @note The reader object must implement setBranchAddresses() function
   *       (otherwise it won't compile)
   * @note This class won't own the pointer since the user may pass a reference
   *       to this function. This implies that the class won't delete the pointers
   *       passed to this function.
   */
  template <typename ReaderBaseDerivative,
            typename = typename std::enable_if<std::is_base_of<ReaderBase, ReaderBaseDerivative>::value, ReaderBaseDerivative>::type>
  TTreeWrapper &
  registerReader(const std::vector<ReaderBaseDerivative *> & readers)
  {
    for(ReaderBaseDerivative * reader: readers)
    {
      registerReader(reader);
    }
    return *this;
  }

  /**
   * @brief Checks if it is possible to reader next event from the list of files
   *        and, if so, proceeds to read it
   * @return true,  if the next event has been read;
   *         false, if there are no more events to read
   *
   * @note Expected usage:
   *         TTreeWrapper wrapper;
   *         // initialize properly
   *         while(wrapper.hasNextEvent())
   *         {
   *           // the event has been read in; do your work here
   *         }
   *
   * @note The function throws in the following instances:
   *         1) the list of files is empty;
   *         2) at least one of the files proved to be corrupted;
   *         3) any of the files does not contain a TTree with the given name.
   */
  bool
  hasNextEvent(bool getEntry = true);

  /**
   * @brief Returns pointer to TTree object in currently processed file
   */
  TTree *
  getCurrentTree() const;

  /**
   * @brief Set the basket size of all branches
   * @param basketSize Basket size in bytes (default = 16000)
   */
  void
  setBasketSize(int basketSize);

  /**
   * @brief Set the cahce size
   * @param cacheSize Cache size in bytes (default = 16000)
   */
  void
  setCacheSize(int cacheSize);

  /**
   * @brief Set status to 0 of branches that are never read
   * @param flag Enable/disable the feature
   */
  void
  setBranchStatus(bool flag);

 private:
  unsigned currentFileIdx_;             ///< Index of currently open file
  long long currentEventIdx_;           ///< Index of currently read event (per single file)
  long long currentMaxEvents_;          ///< Total nof events in currently open file/tree
  long long currentMaxEventIdx_;        ///< Cumulative index of currently read event (across all files)
  long long maxEvents_;                 ///< Maximum nof events to read
  TFile * currentFilePtr_;              ///< Pointer to currently open TFile
  TTree * currentTreePtr_;              ///< Pointer to currently open TTree
  std::string treeName_;                ///< Name of the input TTree
  std::vector<std::string> fileNames_;  ///< List of input files
  std::vector<ReaderBase *> readers_;   ///< List of pointers to *Reader objects
  unsigned fileCount_;                  ///< Total number of input files
  long long cumulativeMaxEventCount_;   ///< Sum of total nof events across all processed files
  mutable long long eventCount_;        ///< Total number of events across all files
  int basketSize_;                      ///< Basket size of all branches
  int cacheSize_;                       ///< Cache size
  bool setBranchStatus_;                ///< Explicitly set the branch status

  /**
   * @brief Closes a currently open file, if there is any
   */
  void
  close();

  /**
   * @brief Returns the total number of events in the files
   * @return The sum of the number of events across all files
   *
   * @note Calling this function will cause opening all the files
   *       sequentially.
   * @note This function is set to private b/c otherwise we would read all files at once
   *       when called. This would defeat the whole purpose of having this class -- to delay
   *       reading each input file by the amount of time it takes to process all events in
   *       a file.
   */
  long long
  getEventCount() const;
};

#endif // TallinnNtupleProducer_Framework_TTreeWrapper_h
