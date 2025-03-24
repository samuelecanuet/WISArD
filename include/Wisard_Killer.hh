#ifndef WISARD_KILLER_HH
#define WISARD_KILLER_HH

#include "G4VSensitiveDetector.hh"

class Wisard_Killer : public G4VSensitiveDetector
{
public:
  Wisard_Killer();
  ~Wisard_Killer();

  void Initialize(G4HCofThisEvent *);
  G4bool ProcessHits(G4Step *, G4TouchableHistory *);
};

#endif
