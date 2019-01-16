import FWCore.ParameterSet.Config as cms

# modify CTPPS 2016 raw-to-digi modules

from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import *

ctppsRawToDigi = cms.Sequence(totemRPRawToDigi*totemTriggerRawToDigi*ctppsPixelDigis)
