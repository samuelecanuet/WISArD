#ifndef WISARD_KILLER_VACUUM_HH
#define WISARD_KILLER_VACUUM_HH

#include "G4VSensitiveDetector.hh"

class Wisard_Killer_Vacuum : public G4VSensitiveDetector
{
public:
  Wisard_Killer_Vacuum();
  ~Wisard_Killer_Vacuum();

  void Initialize(G4HCofThisEvent *);
  G4bool ProcessHits(G4Step *, G4TouchableHistory *);

private:
  G4Material *Material_Vacuum = G4Material::GetMaterial("G4_Galactic");
};

#endif
