#include "TallinnNtupleProducer/CommonTools/interface/TypeTraits.h"

#include <Rtypes.h> // *_t

#include <string>   // std::string

#define DEFINE_TYPE_TRAIT(type,name) template <> __attribute__ ((unused)) const char * Traits<type>::TYPE_NAME = name

DEFINE_TYPE_TRAIT(Char_t,      "B");
DEFINE_TYPE_TRAIT(Char_t *,    "B");
DEFINE_TYPE_TRAIT(UChar_t,     "b");
DEFINE_TYPE_TRAIT(UChar_t *,   "b");
DEFINE_TYPE_TRAIT(Short_t,     "S");
DEFINE_TYPE_TRAIT(Short_t *,   "S");
DEFINE_TYPE_TRAIT(UShort_t,    "s");
DEFINE_TYPE_TRAIT(UShort_t *,  "s");
DEFINE_TYPE_TRAIT(Int_t,       "I");
DEFINE_TYPE_TRAIT(Int_t *,     "I");
DEFINE_TYPE_TRAIT(UInt_t,      "i");
DEFINE_TYPE_TRAIT(UInt_t *,    "i");
DEFINE_TYPE_TRAIT(Float_t,     "F");
DEFINE_TYPE_TRAIT(Float_t *,   "F");
DEFINE_TYPE_TRAIT(Double_t,    "D");
DEFINE_TYPE_TRAIT(Double_t *,  "D");
DEFINE_TYPE_TRAIT(Long64_t,    "L");
DEFINE_TYPE_TRAIT(Long64_t *,  "L");
DEFINE_TYPE_TRAIT(ULong64_t,   "l");
DEFINE_TYPE_TRAIT(ULong64_t *, "l");
DEFINE_TYPE_TRAIT(Bool_t,      "O");
DEFINE_TYPE_TRAIT(Bool_t *,    "O");
DEFINE_TYPE_TRAIT(std::string, "");
