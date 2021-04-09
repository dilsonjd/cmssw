#include "EventFilter/RPCRawToDigi/plugins/RPCCPPFDigiToRaw.h"

#include <cstdint>
#include <cstring>
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

#include "CondFormats/RPCObjects/interface/RPCAMCLinkMap.h"
#include "DataFormats/FEDRawData/interface/FEDHeader.h"
#include "DataFormats/FEDRawData/interface/FEDRawData.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/FEDRawData/interface/FEDTrailer.h"
#include "DataFormats/RPCDigi/interface/RPCDigiCollection.h"
#include "EventFilter/RPCRawToDigi/interface/RPCCPPFPacker.h"
#include "EventFilter/RPCRawToDigi/interface/RPCCPPFRecord.h"
#include "EventFilter/RPCRawToDigi/interface/RPCMP7Record.h"
#include "EventFilter/RPCRawToDigi/interface/RPCAMC13Record.h"
#include "DataFormats/GEMDigi/interface/AMC13Event.h"

#include <iostream>
#include <bitset>
#include "DataFormats/RPCDigi/interface/RPCDigi.h"
#include "DataFormats/RPCDigi/interface/RPCDigiCollection.h"

RPCCPPFDigiToRaw::RPCCPPFDigiToRaw(edm::ParameterSet const& config)
    : es_cppf_link_map_br_token_(esConsumes<RPCAMCLinkMap,RPCCPPFLinkMapRcd, edm::Transition::BeginRun>()),
      es_cppf_link_map_token_(esConsumes<RPCInverseAMCLinkMap,RPCInverseCPPFLinkMapRcd>()),
      es_lb_link_map_token_(esConsumes<RPCInverseLBLinkMap,RPCInverseLBLinkMapRcd>()),
      bx_min_(config.getParameter<int>("bxMin")),
      bx_max_(config.getParameter<int>("bxMax")),
      ignore_eod_(config.getParameter<bool>("ignoreEOD")),
      event_type_(config.getParameter<int>("eventType")) {
        produces<FEDRawDataCollection>();
        digi_token_ = consumes<RPCDigiCollection>(config.getParameter<edm::InputTag>("inputTag"));
    }

RPCCPPFDigiToRaw::~RPCCPPFDigiToRaw() {}

void RPCCPPFDigiToRaw::fillDescriptions(edm::ConfigurationDescriptions& descs) {
    edm::ParameterSetDescription desc;
    desc.add<edm::InputTag>("inputTag", edm::InputTag("simMuonRPCDigis", ""));
    desc.add<int>("bxMin", -2);
    desc.add<int>("bxMax", 2);
    desc.add<bool>("ignoreEOD", true);
    desc.add<int>("eventType", 1);
    descs.add("rpcCPPFDigiToRaw", desc);
}

void RPCCPPFDigiToRaw::beginRun(edm::Run const& run, edm::EventSetup const& setup) {
    if (es_cppf_link_map_watcher_.check(setup)) {
        //edm::ESHandle<RPCAMCLinkMap> es_cppf_link_map;
        //setup.get<RPCCPPFLinkMapRcd>().get(es_cppf_link_map);
        edm::ESHandle<RPCAMCLinkMap> es_cppf_link_map = setup.getHandle(es_cppf_link_map_br_token_); 
        fed_amcs_.clear();
        for (auto const& cppf_link : es_cppf_link_map->getMap()) {
            RPCAMCLink amc(cppf_link.first);
            amc.setAMCInput();
            fed_amcs_[amc.getFED()].push_back(amc);
        }
        for (auto& fed_amcs : fed_amcs_) {
            std::sort(fed_amcs.second.begin(), fed_amcs.second.end());
            fed_amcs.second.erase(std::unique(fed_amcs.second.begin(), fed_amcs.second.end()), fed_amcs.second.end());
        }
    }
}

void RPCCPPFDigiToRaw::produce(edm::Event& event, edm::EventSetup const& setup) {
    // Get EventSetup Electronics Maps
    //edm::ESHandle<RPCInverseAMCLinkMap> es_cppf_link_map_;
    //edm::ESHandle<RPCInverseLBLinkMap> es_lb_link_map;

    //setup.get<RPCInverseCPPFLinkMapRcd>().get(es_cppf_link_map_);
    //setup.get<RPCInverseLBLinkMapRcd>().get(es_lb_link_map);
    //edm::ESHandle<RPCInverseLBLinkMap> es_lb_link_map = setup.getHandle(es_lb_link_map_token_);
    //edm::ESHandle<RPCInverseAMCLinkMap> es_cppf_link_map_ = setup.getHandle(es_cppf_link_map_token_);
    auto es_lb_link_map = setup.getHandle(es_lb_link_map_token_);
    auto es_cppf_link_map_ = setup.getHandle(es_cppf_link_map_token_);
   
    // Get Digi Collection
    edm::Handle<RPCDigiCollection> digi_collection;
    event.getByToken(digi_token_, digi_collection);

    std::unique_ptr<l1t::CPPFDigiCollection> rpc_cppf_digis(new l1t::CPPFDigiCollection()); //cppfdigis

    // Create output
    std::unique_ptr<FEDRawDataCollection> data_collection(new FEDRawDataCollection());
    std::map<RPCAMCLink, std::vector<std::pair<int, rpccppf::RXRecord> > > amc_bx_cppfrecord;
    RPCCPPFPacker::getRPCCPPFRecords(*es_lb_link_map,
            *es_cppf_link_map_,
            bx_min_,
            bx_max_,
            event.bunchCrossing(),
            *digi_collection,
            *rpc_cppf_digis, //cppfdigis
            amc_bx_cppfrecord,
            ignore_eod_);

    // Loop over the FEDs
    for (std::pair<int, std::vector<RPCAMCLink> > const& fed_amcs : fed_amcs_) {
        FEDRawData& rawdata = data_collection->FEDData(fed_amcs.first);
        unsigned int size(0);
        // FED Header + BLOCK Header (1 word + 1 word)
        rawdata.resize((size + 2) * 8);
        // FED Header
        FEDHeader::set(reinterpret_cast<unsigned char *>(rawdata.data()) + size * 8, event_type_, event.id().event(), event.bunchCrossing(), fed_amcs.first);
        ++size;

        rpcamc13::Header amc13header(1, fed_amcs.second.size(), event.eventAuxiliary().orbitNumber()) ;
        std::memcpy(reinterpret_cast<unsigned char *>(rawdata.data()) + size * 8, &amc13header.getRecord(), 8);
        ++size;

        // BLOCK AMC Content - 1 word each
        unsigned int block_size(0);
        rawdata.resize((size + fed_amcs.second.size()) * 8);

        for (RPCAMCLink const& amc : fed_amcs.second) {
            std::map<RPCAMCLink, std::vector<std::pair<int, rpccppf::RXRecord> > >::const_iterator bx_cppfrecord(amc_bx_cppfrecord.find(amc));
            unsigned int block_amc_size(3 + 2 * (bx_cppfrecord == amc_bx_cppfrecord.end() ? 0 : bx_cppfrecord->second.size()));
            rpcamc::Header amc_amc_header(amc.getAMCNumber(), event.id().event(), event.bunchCrossing(), bx_cppfrecord->second.size(), event.eventAuxiliary().orbitNumber(),0);
            if (bx_cppfrecord != amc_bx_cppfrecord.end()) {
                block_size+=block_amc_size; 
                int blknumber(0); 
                rpcamc13::AMCHeader amc13_amc_header; 
                for (std::vector<std::pair<int, rpccppf::RXRecord> >::const_iterator cppfrecord = bx_cppfrecord->second.begin();
                        cppfrecord != bx_cppfrecord->second.end();
                        ++cppfrecord) {

                    amc13_amc_header.setSize(bx_cppfrecord->second.size());
                    amc13_amc_header.setBlockNumber(blknumber); 
                    amc13_amc_header.setAMCNumber(amc.getAMCNumber()); 
                    amc13_amc_header.setBoardId(8);

                    std::memcpy(reinterpret_cast<unsigned char *>(rawdata.data()) + size * 8, &amc13_amc_header.getRecord(), 8);
                    ++blknumber;
                    ++size;
                } 
                block_size+=amc13_amc_header.getSizeInBlock();
                std::memcpy(reinterpret_cast<unsigned char *>(rawdata.data()) + size * 8, amc_amc_header.getRecord(), 16);
                size += amc_amc_header.getDataLength();
            }
        }

        // AMC Payload - 2 words header, 1 word trailer, 2 words per RXRecord
        rawdata.resize((size + block_size) * 8);

        for (RPCAMCLink const& amc : fed_amcs.second) {
            // CPPF Header
            std::map<RPCAMCLink, std::vector<std::pair<int, rpccppf::RXRecord> > >::const_iterator bx_cppfrecord(
                    amc_bx_cppfrecord.find(amc));
            if (bx_cppfrecord != amc_bx_cppfrecord.end()) {
            // BLOCK Header
            rpcmp7::BlockHeader cppf_BlockHeader;
            int length(72), index(0);
            unsigned int cap_id(0x01); 
            bool zs_per_bx = false;
            bool is_zs = false;
            cppf_BlockHeader.setZeroSuppressed(is_zs);
            cppf_BlockHeader.setZeroSuppressionPerBX(zs_per_bx);
            cppf_BlockHeader.setCaptionId(cap_id);
            cppf_BlockHeader.setLength(length);
            for (int j = 0 ; j<3 ; j++) {
                cppf_BlockHeader.setId(index);
                std::memcpy(reinterpret_cast<unsigned char *>(rawdata.data()) + size * 8, &cppf_BlockHeader.getRecord(), 8);
                ++size;
                index +=2;
            }

                for (std::vector<std::pair<int, rpccppf::RXRecord> >::const_iterator cppfrecord = bx_cppfrecord->second.begin();
                        cppfrecord != bx_cppfrecord->second.end();
                        ++cppfrecord) {
                    std::memcpy(reinterpret_cast<unsigned char *>(rawdata.data()) + size * 8, &cppfrecord->second.getRecord(), sizeof(cppfrecord->second.getRecord())); 
                    size += 2;
                }
            }
        }
        // BLOCK Trailer + FED Trailer (1 word + 1 word)
        rawdata.resize((size + 2) * 8);
        // BLOCK Trailer
        rpcamc::Trailer block_trailer(0x0, event.id().event(), event.bunchCrossing());
        std::memcpy(reinterpret_cast<unsigned char *>(rawdata.data()) + size * 8, &block_trailer.getRecord(), 8);
        ++size;
        // CRC32 not calculated (for now)
        // FED Trailer
        ++size;

        FEDTrailer::set(reinterpret_cast<unsigned char *>(rawdata.data()) + (size - 1) * 8, size, 0x0, 0, 0);
        std::uint16_t crc(evf::compute_crc(rawdata.data(), size * 8));
        FEDTrailer::set(reinterpret_cast<unsigned char *>(rawdata.data()) + (size - 1) * 8, size, crc, 0, 0);

    }
    event.put(std::move(data_collection));
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(RPCCPPFDigiToRaw);
