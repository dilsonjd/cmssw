#ifndef EventFilter_RPCRawToDigi_RPCCPPFDigiToRaw_h
#define EventFilter_RPCRawToDigi_RPCCPPFDigiToRaw_h

#include <map>
#include <vector>

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ESWatcher.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
 #include "FWCore/Framework/interface/ProducesCollector.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Utilities/interface/ESGetToken.h"

#include "CondFormats/DataRecord/interface/RPCCPPFLinkMapRcd.h"
#include "CondFormats/RPCObjects/interface/RPCAMCLink.h"
#include "DataFormats/RPCDigi/interface/RPCDigiCollection.h"

#include "CondFormats/RPCObjects/interface/RPCAMCLinkMap.h"
#include "CondFormats/RPCObjects/interface/RPCInverseAMCLinkMap.h"
#include "CondFormats/RPCObjects/interface/RPCInverseLBLinkMap.h"
#include "CondFormats/DataRecord/interface/RPCInverseLBLinkMapRcd.h"
#include "CondFormats/DataRecord/interface/RPCInverseCPPFLinkMapRcd.h"

namespace edm {
  class ConfigurationDescriptions;
  class Event;
  class EventSetup;
  class ParameterSet;
  class Run;
}  // namespace edm

class RPCCPPFDigiToRaw : public edm::stream::EDProducer<> {
public:
  RPCCPPFDigiToRaw(edm::ParameterSet const& config);
  ~RPCCPPFDigiToRaw() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descs);

  void beginRun(edm::Run const& run, edm::EventSetup const& setup) override;
  void produce(edm::Event& event, edm::EventSetup const& setup) override;

protected:
  edm::EDGetTokenT<RPCDigiCollection> digi_token_;
 
  edm::ESGetToken<RPCAMCLinkMap,RPCCPPFLinkMapRcd> es_cppf_link_map_br_token_;
  edm::ESGetToken<RPCInverseAMCLinkMap,RPCInverseCPPFLinkMapRcd> es_cppf_link_map_token_;
  edm::ESGetToken<RPCInverseLBLinkMap,RPCInverseLBLinkMapRcd> es_lb_link_map_token_;
  int bx_min_, bx_max_;
  bool ignore_eod_;
  int event_type_;
  unsigned int ufov_;

  edm::ESWatcher<RPCCPPFLinkMapRcd> es_cppf_link_map_watcher_;
  std::map<int, std::vector<RPCAMCLink> > fed_amcs_;

};

#endif  // EventFilter_RPCRawToDigi_RPCCPPFDigiToRaw_h
