#include "EventFilter/RPCRawToDigi/interface/RPCCPPFPacker.h"

#include "CondFormats/RPCObjects/interface/RPCInverseAMCLinkMap.h"
#include "EventFilter/RPCRawToDigi/interface/RPCLBPacker.h"

#include "EventFilter/RPCRawToDigi/interface/RPCTwinMuxRecord.h" 

#include <iostream>
#include <bitset>
void RPCCPPFPacker::getRPCCPPFRecords(
        RPCInverseLBLinkMap const& lb_map,
        RPCInverseAMCLinkMap const& amc_map,
        int min_bx,
        int max_bx,
        unsigned int bcn,
        RPCDigiCollection const& digis,
        l1t::CPPFDigiCollection& cppfdigis, //cppfdigis
        std::map<RPCAMCLink, std::vector<std::pair<int, rpccppf::RXRecord> > >& amc_bx_cppfrecord,
        bool ignore_eod)
{
    std::map<RPCLBLink, std::vector<std::pair<int, RPCLBRecord> > > mlb_bx_lbrecord;
    RPCLBPacker::getRPCLBRecords(lb_map, min_bx, max_bx, bcn, digis, mlb_bx_lbrecord, ignore_eod);

    for (std::map<RPCLBLink, std::vector<std::pair<int, RPCLBRecord> > >::const_iterator mlb_bx_lbrecord_it = mlb_bx_lbrecord.begin();
            mlb_bx_lbrecord_it != mlb_bx_lbrecord.end();
            ++mlb_bx_lbrecord_it) {
        // multimap, but no splitting for CPPF inputs
        RPCInverseAMCLinkMap::map_type::const_iterator amc_it(amc_map.getMap().find(mlb_bx_lbrecord_it->first));
        if (amc_it == amc_map.getMap().end()) {
            continue;
        }

        RPCAMCLink amc_id(amc_it->second);
        int amc_input(amc_id.getAMCInput());
        amc_id.setAMCInput();
        std::vector<std::pair<int, rpccppf::RXRecord> >& bx_cppfrecord(amc_bx_cppfrecord[amc_id]);
        std::vector<std::pair<int, rpccppf::RXRecord> >::iterator cppfrecord_it(bx_cppfrecord.begin());
        for (std::vector<std::pair<int, RPCLBRecord> >::const_iterator bx_lbrecord = mlb_bx_lbrecord_it->second.begin();
                bx_lbrecord != mlb_bx_lbrecord_it->second.end();
                ++bx_lbrecord) {
            // find the first record at this bx for this amc without this input
            for (; cppfrecord_it != bx_cppfrecord.end() && cppfrecord_it->first < bx_lbrecord->first; ++cppfrecord_it) 
                ;
            if (cppfrecord_it == bx_cppfrecord.end() || cppfrecord_it->first != bx_lbrecord->first) {
                cppfrecord_it = bx_cppfrecord.insert(cppfrecord_it, std::pair<int, rpccppf::RXRecord>(bx_lbrecord->first, rpccppf::RXRecord()));
                cppfrecord_it->second.setBXCounterMod(bx_lbrecord->first);
            }

            cppfrecord_it->second.setLink(amc_input);
            cppfrecord_it->second.setAcknowledge(true);
            cppfrecord_it->second.setEOD(bx_lbrecord->second.isEOD());
            cppfrecord_it->second.setDelay(bx_lbrecord->second.getDelay());
            cppfrecord_it->second.setLinkBoard(bx_lbrecord->second.getLinkBoard());
            cppfrecord_it->second.setConnector(bx_lbrecord->second.getConnector());
            cppfrecord_it->second.setPartition(bx_lbrecord->second.getPartition());
            cppfrecord_it->second.setPartitionData(bx_lbrecord->second.getPartitionData());


            // make sure we don't fill this input twice if ignore_eod == true
            ++cppfrecord_it;
        }
    }
}
