#include "Wisard_PhysList.hh"

#include "G4VUserPhysicsList.hh"
#include "G4RadioactiveDecay.hh"
#include "G4PhotonEvaporation.hh"
#include "G4ProcessManager.hh"
#include "G4GenericIon.hh"
#include "G4PhysicsListHelper.hh"
#include "G4ParticleTypes.hh"

#include "G4EmParameters.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4ParticlePropertyData.hh"
#include "G4IonTable.hh"
#include "G4IonPhysics.hh"

#include "G4Scintillation.hh"
#include "G4EmSaturation.hh"

//----------------------------------------------------------------------
Wisard_PhysList::Wisard_PhysList()
{
  step_max = NULL;
  G4cout << "\033[32m" << "Constructor Wisard_PhysList" << "\033[0m" << G4endl;
  // defaultCutValue = 0.001 * um;
}

Wisard_PhysList::~Wisard_PhysList()
{
  G4cout << "\033[31m" << "Destructor Wisard_PhysicList" << "\033[0m" << G4endl;
}

//----------------------------------------------------------------------
// Register the particles that will be handled in the simulation
void Wisard_PhysList::ConstructParticle()
{
  G4DeexPrecoParameters *deex =
      G4NuclearLevelData::GetInstance()->GetParameters();
  deex->SetStoreAllLevels(true);
  deex->SetStoreICLevelData(true);
  deex->SetInternalConversionFlag(true);

  // pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();

  // gamma
  G4Gamma::GammaDefinition();

  // optical photon
  // G4OpticalPhoton::OpticalPhotonDefinition();

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
  G4KaonPlus::KaonPlusDefinition();
  G4KaonMinus::KaonMinusDefinition();
  G4Lambda::LambdaDefinition();
  G4AntiLambda::AntiLambdaDefinition();


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
  // AddStepMax(1 * cm, 0x2);
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
  emPhysicsList = new G4EmStandardPhysics_option4(0);
  emPhysicsList->ConstructProcess();

  G4EmParameters *emParams = G4EmParameters::Instance();
  emParams->SetNumberOfBinsPerDecade(200);

  G4StepLimiterPhysics *process = new G4StepLimiterPhysics();
  process->ConstructProcess();

  G4RadioactiveDecayPhysics *radioactiveDecay = new G4RadioactiveDecayPhysics();
  radioactiveDecay->ConstructProcess();

}

//----------------------------------------------------------------------

void Wisard_PhysList::SetCuts()
{

  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets
  //   the default cut value for all particle types
  SetCutsWithDefault();

  // SetCutValue(0.001 * um, "gamma");
  // SetCutValue(0.001 * um, "proton");
  // SetCutValue(0.001 * um, "e-");
  // SetCutValue(0.001 * um, "e+");
  // defaultCutValue = 0.001 * um;
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
        G4double qtest = fabs(0.01 * electron_charge);

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
          // G4cout<< "- step max for " << particle->GetParticleName() <<G4endl;
          pmanager->AddDiscreteProcess(step_max);
        }
      }
    }

    if (step > 1. * nm)
      SetStepMax(step);
  }
  else
  {
    cerr << "<W> Wisard_PhysList::AddStepMax(): StepMax process already defined" << G4endl;
  }
}

/*! This function sets the maximum step value for the StepMax process.
 *  \param  step    maximum step value (length)
 */
void Wisard_PhysList::SetStepMax(G4double step)
{
  if (step_max == NULL)
    AddStepMax();

  G4cout << "\033[34m" << "StepMax = "
         << G4String(G4BestUnit(step, "Length")) << "\033[0m" << G4endl;

  step_max->SetMaxStep(step);
}
