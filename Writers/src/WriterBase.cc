#include "TallinnNtupleProducer/Writers/interface/WriterBase.h"

WriterBase::WriterBase(const edm::ParameterSet & cfg)
{}

WriterBase::~WriterBase()
{}

EDM_REGISTER_PLUGINFACTORY(WriterPluginFactory, "WriterPluginFactory");
