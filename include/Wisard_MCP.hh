#ifndef WISARD_MCP_HH
#define WISARD_MCP_HH

#include "G4VSensitiveDetector.hh"
#include "Wisard_RunAction.hh"
#include "G4RunManager.hh"

class Wisard_MCP : public G4VSensitiveDetector
{
public:
  Wisard_MCP();
  ~Wisard_MCP();

  void Initialize(G4HCofThisEvent *);
  G4bool ProcessHits(G4Step *, G4TouchableHistory *);

  Wisard_RunAction *RunAction;
};

#endif
