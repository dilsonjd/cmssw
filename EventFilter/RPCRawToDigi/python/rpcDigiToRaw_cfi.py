import FWCore.ParameterSet.Config as cms


from EventFilter.RPCRawToDigi.RPCTwinMuxDigiToRaw_cff import *
from EventFilter.RPCRawToDigi.RPCCPPFDigiToRaw_cff import *

rpcDigiToRawTask = cms.Task(RPCTwinMuxDigiToRaw,rpcCPPFDigiToRaw)

