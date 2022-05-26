#ifndef TallinnNtupleProducer_Objects_printCollection_h
#define TallinnNtupleProducer_Objects_printCollection_h

#include <iostream> // std::cout
#include <string>   // std::string
#include <vector>   // std::vector

template <typename T,
          typename = typename std::enable_if<! std::is_pointer<T>::value>>
void
printCollection(const std::string & collection_name,
                const std::vector<T> & collection)
{
  std::cout << " (#" << collection_name << " = " << collection.size() << ")\n";
  for(std::size_t idx = 0; idx < collection.size(); ++idx)
  {
    std::cout << collection_name << " #" << idx << ": " << collection[idx] << "\n";
  }
}

template <typename T,
          typename = typename std::enable_if<! std::is_pointer<T>::value>>
void
printCollection(const std::string & collection_name,
                const std::vector<const T *> & collection)
{
  std::cout << " (#" << collection_name << " = " << collection.size() << ")\n";
  for(std::size_t idx = 0; idx < collection.size(); ++idx)
  {
    std::cout << collection_name << " #" << idx << ": " << *(collection[idx]) << "\n";
  }
}

#endif // TallinnNtupleProducer_Objects_printCollection_h
