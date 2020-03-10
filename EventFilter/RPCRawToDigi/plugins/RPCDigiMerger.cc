#include "EventFilter/RPCRawToDigi/plugins/RPCDigiMerger.h"

#include <memory>

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Utilities/interface/CRC16.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/RPCDigi/interface/RPCDigiCollection.h"

using namespace edm;
using namespace std;

RPCDigiMerger::RPCDigiMerger(edm::ParameterSet const& config)
    : bx_minOMTF_(config.getParameter<int>("bxMinOMTF")),
      bx_maxOMTF_(config.getParameter<int>("bxMaxOMTF")) { 
  produces<RPCDigiCollection>();
  // protection against empty InputTag to allow for Data/MC compatibility
  if (not config.getParameter<edm::InputTag>("inputTagTwinMuxDigis").label().empty()) {
    twinMux_token_ = consumes<RPCDigiCollection>(config.getParameter<edm::InputTag>("inputTagTwinMuxDigis"));
  }
  if (not config.getParameter<edm::InputTag>("inputTagOMTFDigis").label().empty()) {
    omtf_token_ = consumes<RPCDigiCollection>(config.getParameter<edm::InputTag>("inputTagOMTFDigis"));
  }
  if (not config.getParameter<edm::InputTag>("inputTagCPPFDigis").label().empty()) {
    cppf_token_ = consumes<RPCDigiCollection>(config.getParameter<edm::InputTag>("inputTagCPPFDigis"));
  }
}

RPCDigiMerger::~RPCDigiMerger() {}

void RPCDigiMerger::fillDescriptions(edm::ConfigurationDescriptions& descs) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("inputTagTwinMuxDigis", edm::InputTag("", ""));
  desc.add<edm::InputTag>("inputTagOMTFDigis", edm::InputTag("", ""));
  desc.add<edm::InputTag>("inputTagCPPFDigis", edm::InputTag("", ""));
  desc.add<edm::InputTag>("InputLabel", edm::InputTag(" "));
  desc.add<int>("bxMinOMTF", -3);
  desc.add<int>("bxMaxOMTF", 4);

  descs.add("rpcDigiMerger", desc);
}

void RPCDigiMerger::beginRun(edm::Run const& run, edm::EventSetup const& setup) {}

void RPCDigiMerger::produce(edm::Event& event, edm::EventSetup const& setup) {
  // Get the digis
  // new RPCDigiCollection
  std::unique_ptr<RPCDigiCollection> rpc_digi_collection(new RPCDigiCollection());

  //Check if its Data
  if (not(cppf_token_.isUninitialized() && omtf_token_.isUninitialized() && twinMux_token_.isUninitialized())) {
    // loop over TwinMux digis
    // protection against empty InputTag to allow for Data/MC compatibility
    if (not twinMux_token_.isUninitialized()) {
      Handle<RPCDigiCollection> TwinMux_digis;
      event.getByToken(twinMux_token_, TwinMux_digis);
      for (const auto&& rpcdgIt : (*TwinMux_digis)) {
        // The layerId
        const RPCDetId& rpcId = rpcdgIt.first;
        // Get the iterators over the digis associated with this LayerId
        const RPCDigiCollection::Range& range = rpcdgIt.second;
        rpc_digi_collection->put(range, rpcId);
      }
    }
    // loop over CPPF digis
    // protection against empty InputTag to allow for Data/MC compatibility
    if (not cppf_token_.isUninitialized()) {
      Handle<RPCDigiCollection> CPPF_digis;
      event.getByToken(cppf_token_, CPPF_digis);
      for (const auto&& rpcdgIt : (*CPPF_digis)) {
        // The layerId
        const RPCDetId& rpcId = rpcdgIt.first;
        // Get the iterators over the digis associated with this LayerId
        const RPCDigiCollection::Range& range = rpcdgIt.second;
        rpc_digi_collection->put(range, rpcId);
      }
    }
    // loop over OMTF digis
    // protection against empty InputTag to allow for Data/MC compatibility
    if (not omtf_token_.isUninitialized()) {
      Handle<RPCDigiCollection> OMTF_digis;
      event.getByToken(omtf_token_, OMTF_digis);
      for (const auto& rpcdgIt : (*OMTF_digis)) {
        // The layerId
        const RPCDetId& rpcId = rpcdgIt.first;
        // Get the iterators over the digis associated with this LayerId
        const RPCDigiCollection::Range& range = rpcdgIt.second;
        // accepts only rings: RE-2_R3 ; RE-1_R3 ; RE+1_R3 ; RE+2_R3 ;
        if (((rpcId.region() == -1 || rpcId.region() == 1) && (rpcId.ring() == 3) &&
             (rpcId.station() == 1 || rpcId.station() == 2))) {
          rpc_digi_collection->put(range, rpcId);
        }
      }
    }
  } 
  event.put(std::move(rpc_digi_collection));
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(RPCDigiMerger);
