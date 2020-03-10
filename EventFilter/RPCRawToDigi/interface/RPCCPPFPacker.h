#ifndef EventFilter_RPCRawToDigi_RPCCPPFPacker_h
#define EventFilter_RPCRawToDigi_RPCCPPFPacker_h

#include <map>
#include <vector>

#include "CondFormats/RPCObjects/interface/RPCAMCLink.h"
#include "DataFormats/RPCDigi/interface/RPCDigiCollection.h"
#include "EventFilter/RPCRawToDigi/interface/RPCCPPFRecord.h"

class RPCInverseLBLinkMap;
class RPCInverseAMCLinkMap;

class RPCCPPFPacker {
public:
  static void getRPCCPPFRecords(
      RPCInverseLBLinkMap const& lb_map,
      RPCInverseAMCLinkMap const& amc_map,
      int min_bx,
      int max_bx,
      unsigned int bcn,
      RPCDigiCollection const& digis,
      std::map<RPCAMCLink, std::vector<std::pair<int, rpccppf::RXRecord> > >& amc_bx_cppfrecord,
      bool ignore_eod = false);
};

#endif  // EventFilter_RPCRawToDigi_RPCCPPFPacker_h
