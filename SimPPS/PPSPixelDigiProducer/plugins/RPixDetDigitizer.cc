#include <vector>
#include <iostream>
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimPPS/PPSPixelDigiProducer/interface/RPixDetDigitizer.h"
#include "Geometry/VeryForwardGeometry/interface/CTPPSPixelTopology.h"

RPixDetDigitizer::RPixDetDigitizer(const edm::ParameterSet &params, CLHEP::HepRandomEngine& eng, uint32_t det_id, const edm::EventSetup& iSetup)
  : params_(params), det_id_(det_id)
{
  verbosity_ = params.getParameter<int>("RPixVerbosity");
  numPixels = CTPPSPixelTopology().detPixelNo();
  theNoiseInElectrons = params.getParameter<double>("RPixEquivalentNoiseCharge");
  thePixelThresholdInE = params.getParameter<double>("RPixDummyROCThreshold");
  noNoise = params.getParameter<bool>("RPixNoNoise");

  _links_persistence = params.getParameter<bool>("CTPPSPixelDigiSimHitRelationsPersistence");

  theRPixPileUpSignals = new RPixPileUpSignals(params_, det_id_);
  theRPixDummyROCSimulator = new RPixDummyROCSimulator(params_, det_id_);
  theRPixHitChargeConverter = new RPixHitChargeConverter(params_, eng, det_id_);
}

RPixDetDigitizer::~RPixDetDigitizer()
{
  delete theRPixDummyROCSimulator;
  delete theRPixHitChargeConverter;
}

void RPixDetDigitizer::run(const std::vector<PSimHit> &input, const std::vector<int> &input_links, 
			   std::vector<CTPPSPixelDigi> &output_digi,
			   std::vector<std::vector<std::pair<int, double> > >  &output_digi_links,
                           const CTPPSPixelGainCalibrations * pcalibrations
			   )
{
  if(verbosity_)
    edm::LogInfo("RPixDetDigitizer")<<det_id_<<" received input.size()="<<input.size();
  theRPixPileUpSignals->reset();
  bool links_persistence_checked = _links_persistence && input_links.size()==input.size();
  int input_size = input.size();
  for (int i=0; i<input_size; ++i)
    {
      std::map<unsigned short, double, std::less<unsigned short> >  the_pixel_charge_map;
      the_pixel_charge_map = theRPixHitChargeConverter->processHit(input[i]);
      
      if(verbosity_)
	edm::LogInfo("RPixDetDigitizer")<<det_id_<<" returned hits="<<the_pixel_charge_map.size();
      if(links_persistence_checked)
	theRPixPileUpSignals->add(the_pixel_charge_map, input_links[i]);
      else
	theRPixPileUpSignals->add(the_pixel_charge_map, 0);
    }
  const std::map<unsigned short, double, std::less<unsigned short> >  &theSignal = theRPixPileUpSignals->dumpSignal();
  std::map<unsigned short, std::vector< std::pair<int, double> > >  &theSignalProvenance = theRPixPileUpSignals->dumpLinks();
  std::map<unsigned short, double, std::less<unsigned short> >  afterNoise;
  afterNoise = theSignal;
  theRPixDummyROCSimulator->ConvertChargeToHits(afterNoise, theSignalProvenance, 
						output_digi,  output_digi_links, pcalibrations);
}
