#include "Wisard_PhysList.hh"

// declare the Geant4 particles definition classes
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"

// declare classes related to physics processes
#include "G4ProcessManager.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmStandardPhysicsGS.hh"

#include "G4TransportationManager.hh"

//----------------------------------------------------------------------
Wisard_PhysList::Wisard_PhysList()
{
  step_max = NULL;
  cout << "Constructor Wisard_PhysList" << endl;
  defaultCutValue = 0.001 * um;
}

Wisard_PhysList::~Wisard_PhysList()
{
  cout << "Destructor Wisard_PhysList" << endl;

  if (step_max != NULL)
    delete step_max;
}

//----------------------------------------------------------------------
// Register the particles that will be handled in the simulation
void Wisard_PhysList::ConstructParticle()
{

  // pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();

  // gamma
  G4Gamma::GammaDefinition();

  // optical photon
  G4OpticalPhoton::OpticalPhotonDefinition();

  // leptons
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();

  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();

  // mesons
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4PionZero::PionZeroDefinition();
  G4Eta::EtaDefinition();
  G4EtaPrime::EtaPrimeDefinition();
  G4KaonPlus::KaonPlusDefinition();
  G4KaonMinus::KaonMinusDefinition();
  G4KaonZero::KaonZeroDefinition();
  G4AntiKaonZero::AntiKaonZeroDefinition();
  G4KaonZeroLong::KaonZeroLongDefinition();
  G4KaonZeroShort::KaonZeroShortDefinition();

  // barions
  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();
  G4Neutron::NeutronDefinition();
  G4AntiNeutron::AntiNeutronDefinition();

  // ions
  G4Deuteron::DeuteronDefinition();
  G4Triton::TritonDefinition();
  G4Alpha::AlphaDefinition();
  G4GenericIon::GenericIonDefinition();
}

//----------------------------------------------------------------------
// Associate selected processes to the particles
void Wisard_PhysList::ConstructProcess()
{
  // define transportation process
  AddTransportation();

  // G4VPhysicsConstructor * emPhysicsList = new G4EmPenelopePhysics(0);
  // G4VPhysicsConstructor * emPhysicsList = new G4EmLivermorePhysics();
  // G4VPhysicsConstructor * emPhysicsList = new G4EmStandardPhysics(1);
  // G4VPhysicsConstructor * emPhysicsList = new G4EmStandardPhysics_option4(0);
  G4VPhysicsConstructor *emPhysicsList = new G4EmStandardPhysicsGS(0);
  emPhysicsList->ConstructProcess();
}

//----------------------------------------------------------------------

void Wisard_PhysList::SetCuts()
{

  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets
  //   the default cut value for all particle types
  SetCutsWithDefault();

  SetCutValue(0.001 * um, "gamma");
  SetCutValue(0.001 * um, "proton");
  SetCutValue(0.001 * um, "e-");
  SetCutValue(0.001 * um, "e+");
  defaultCutValue = 0.001 * um;
}

//----------------------------------------------------------------------

/*! This function defines a StepMax process for the physics list.
 *  The maximum step value must be set using SetStepMax function.
 *  \param  step    maximum step value (length)
 *  \param  flag    bits indicating on which particles to apply it:
 *                  - bit 0 (=1) for Q=0
 *                  - bit 1 (=2) for Q>0
 *                  - bit 2 (=4) for Q<0
 */
void Wisard_PhysList::AddStepMax(G4double step, u_short flag)
{
  if (step_max == NULL)
  {
    cout << "<I> Adding a StepMax process " << endl;

    // Step limitation seen as a process
    step_max = new StepMax;

    auto theParticleIterator = G4ParticleTable::GetParticleTable()->GetIterator();
    theParticleIterator->reset();

    while ((*theParticleIterator)())
    {
      G4ParticleDefinition *particle = theParticleIterator->value();
      G4ProcessManager *pmanager = particle->GetProcessManager();

      // (note that with this condition, it will not be applied
      //  to particles with Q=0 anyway...)
      if (step_max->IsApplicable(*particle) && (pmanager != NULL))
      {
        bool apply = false;
        double qtest = fabs(0.01 * electron_charge);

        if (particle->GetPDGCharge() > qtest) // Q > 0
        {
          if ((flag & 2) != 0)
            apply = true;
        }
        else if (particle->GetPDGCharge() < -qtest) // Q < 0
        {
          if ((flag & 4) != 0)
            apply = true;
        }
        else // Q = 0
        {
          if ((flag & 1) != 0)
            apply = true;
        }

        if (apply)
        {
          // cout << "- step max for " << particle->GetParticleName() << endl;
          pmanager->AddDiscreteProcess(step_max);
        }
      }
    }

    if (step > 1. * nm)
      SetStepMax(step);
  }
  else
  {
    cerr << "<W> Wisard_PhysList::AddStepMax(): StepMax process already defined" << endl;
  }
}

/*! This function sets the maximum step value for the StepMax process.
 *  \param  step    maximum step value (length)
 */
void Wisard_PhysList::SetStepMax(G4double step)
{
  if (step_max == NULL)
    AddStepMax();

  cout << "<I> Setting StepMax to "
       << string(G4BestUnit(step, "Length")) << endl;

  step_max->SetMaxStep(step);
}
