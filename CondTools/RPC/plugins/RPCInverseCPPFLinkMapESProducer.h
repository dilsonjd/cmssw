#ifndef CondTools_RPC_RPCInverseCPPFLinkMapESProducer_h
#define CondTools_RPC_RPCInverseCPPFLinkMapESProducer_h

#include <memory>

#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/ESProductHost.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/ESGetToken.h"
#include "FWCore/Utilities/interface/ReusableObjectHolder.h"

#include "CondFormats/RPCObjects/interface/RPCInverseAMCLinkMap.h"
#include "CondFormats/RPCObjects/interface/RPCAMCLinkMap.h"

namespace edm {
  class ParameterSet;
  class ConfigurationDescriptions;
}  // namespace edm

class RPCCPPFLinkMapRcd;
class RPCInverseCPPFLinkMapRcd;

class RPCInverseCPPFLinkMapESProducer : public edm::ESProducer {
public:
  explicit RPCInverseCPPFLinkMapESProducer(edm::ParameterSet const& _config);

  static void fillDescriptions(edm::ConfigurationDescriptions& _descs);

  std::shared_ptr<RPCInverseAMCLinkMap> produce(RPCInverseCPPFLinkMapRcd const& _rcd);

private:
  using HostType = edm::ESProductHost<RPCInverseAMCLinkMap, RPCCPPFLinkMapRcd>;

  void setupRPCCPPFLinkMap(RPCCPPFLinkMapRcd const&, RPCInverseAMCLinkMap*);

  edm::ReusableObjectHolder<HostType> holder_;
  
  edm::ESGetToken<RPCAMCLinkMap, RPCCPPFLinkMapRcd> es_rpc_cppf_l_map_token_;	//const es_rpc_tm_l_map_token_;

};

#endif  // CondTools_RPC_RPCInverseCPPFLinkMapESProducer_h
