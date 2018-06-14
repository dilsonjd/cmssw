#ifndef myRPNumberingScheme_h
#define myRPNumberingScheme_h

#include "SimG4CMS/PPS/interface/myTotemRPOrganization.h"

class myRPNumberingScheme : public myTotemRPOrganization {

public:
  myRPNumberingScheme(int i);
  ~myRPNumberingScheme();
	 
//  virtual unsigned int GetUnitID(const G4Step* aStep) const ;

};

#endif  //PPS_myRPNumberingScheme_h
