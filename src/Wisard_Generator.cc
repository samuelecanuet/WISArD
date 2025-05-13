#include "Wisard_Generator.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "Randomize.hh"
#include "G4AutoLock.hh"
#include "TROOT.h"

thread_local bool INIT = false;

Wisard_Generator::Wisard_Generator()
{
      

 G4cout << "\033[32m" << "Constructor Wisard_Generator"  << "\033[0m" << G4endl;

  part_proton = particle_table->FindParticle("proton");
  part_alpha = particle_table->FindParticle("alpha");
  part_geantino = particle_table->FindParticle("geantino");
  part_positron = particle_table->FindParticle("e+");
  part_electron = particle_table->FindParticle("e-");
  part_charged_geantino = particle_table->FindParticle("chargedgeantino");


  BeamMessenger = new G4GenericMessenger(this, "/Beam/", "All Beam Settings");
  InputMessenger = new G4GenericMessenger(this, "/Input/", "All Input Settings");

  BeamMessenger->DeclarePropertyWithUnit("X", "mm", X)
      .SetGuidance("Set Beam X offset.")
      .SetParameterName("X", false)
      .SetDefaultValue("0.0 mm");

  BeamMessenger->DeclarePropertyWithUnit("Y", "mm", Y)
      .SetGuidance("Set Beam Y offset.")
      .SetParameterName("Y", false)
      .SetDefaultValue("0.0 mm");

  BeamMessenger->DeclarePropertyWithUnit("Radius", "mm", Radius)
      .SetGuidance("Set Beam Radius.")
      .SetParameterName("Radius", false)
      .SetDefaultValue("2 mm");

  BeamMessenger->DeclarePropertyWithUnit("Sigma_X", "mm", Sigma_X)
      .SetGuidance("Set Beam Sigma_X offset.")
      .SetParameterName("Sigma_X", false)
      .SetDefaultValue("0.3 mm");

  BeamMessenger->DeclarePropertyWithUnit("Sigma_Y", "mm", Sigma_Y)
      .SetGuidance("Set Beam Sigma_Y offset.")
      .SetParameterName("Sigma_Y", false)
      .SetDefaultValue("0.3 mm");

  InputMessenger->DeclareProperty("CRADLE", InputFileName)
      .SetGuidance("Set CRADLE input file.")
      .SetParameterName("CRADLE", false)
      .SetDefaultValue("");

  InputMessenger->DeclareProperty("SRIM", SRIMFileName)
      .SetGuidance("Set SRIM input file.")
      .SetParameterName("SRIM", false)
      .SetDefaultValue("SRIM_data/AlMylar_2021_32Ar.root");

  InputMessenger->DeclareProperty("Ion", nucleus_string)
      .SetGuidance("Set Ion.")
      .SetParameterName("Ion", false)
      .SetDefaultValue("");
    
  InputMessenger->DeclareProperty("Particle", particle_string)
      .SetGuidance("Set particle.")
      .SetParameterName("Particle", false)
      .SetDefaultValue("");

  InputMessenger->DeclarePropertyWithUnit("Energy", "keV", energy)
      .SetGuidance("Set Energy.")
      .SetParameterName("Energy", false)
      .SetDefaultValue("0.0 keV");

  InputMessenger->DeclareProperty("Direction", dir_string)
      .SetGuidance("Set Direction.")
      .SetDefaultValue("0 0 1");

  InputMessenger->DeclareProperty("Position", pos_string)
      .SetGuidance("Set Position.")
      .SetDefaultValue("0 0 0");
  
}

Wisard_Generator::~Wisard_Generator()
{
  
  delete BeamMessenger;
  delete InputMessenger;
 G4cout << "\033[31m" << "Destructor Wisard_Generator"  << "\033[0m" << G4endl;
}

//----------------------------------------------------------------------

void Wisard_Generator::GeneratePrimaries(G4Event *event)
{
    
    if (!INIT)
    {
        SRIM_HISTOGRAM = Wisard_Generator::GetSRIM_hist();
        InitBeam();
        ChooseGENERATOR();
        INIT = true;
    }

    GENERATOR(event);
}

void Wisard_Generator::TXT_GENERATOR(G4Event *event)
{
  if (G4Threading::IsMasterThread())
    G4Exception("Wisard_Generator::TXT_GENERATOR", "Not implemented in MT mode", FatalException, "");  

  int eventNr, totpartNr, currentsubevent, subeventNr;
  int currentpartNr = 0;

  InputTXT >> eventNr >> totpartNr;
  //cout << eventNr << "\t" << totpartNr <<G4endl;

  G4ThreeVector beam = Beam();

  while (currentpartNr < totpartNr)
  {
    InputTXT >> eventNr >> currentsubevent >> subeventNr;
    //cout << eventNr << "\t" << currentsubevent << "\t" << subeventNr <<G4endl;

    for (int subindex = 0; subindex < subeventNr; subindex++)
    {
      G4String name;
     G4double ekin, exc, mom[4], time;
      InputTXT >> ievent >> time >> name >> exc >> ekin >> mom[0] >> dir[0] >> dir[1] >> dir[2];
      //cout << "\t" << ievent << "\t" << time << "\t" << name << "\t" << exc << "\t" << ekin << "\t" <<  mom[0] << "\t" <<  mom[1] << "\t" <<  mom[2] << "\t" <<  mom[3] <<G4endl;
      currentpartNr++;

      if (name.compare(0, 2, "e-") == 0)
        particle = part_electron;
      else if (name.compare(0, 2, "e+") == 0)
      {
        particle = part_positron;
      }
      else if (name.compare(0, 5, "gamma") == 0)
      {
        particle = part_gamma;
      }
      else if (name.compare(0, 1, "p") == 0)
      {
        particle = part_proton;
      }
      else if (name.compare(0, 5, "alpha") == 0 || name.compare(0, 3, "4He") == 0)
      {
        particle = part_alpha;
      }
      else
      {
        continue;
      }

        gun.SetParticleDefinition(particle);
        gun.SetParticlePosition(beam);
        gun.SetParticleMomentumDirection(dir);
        gun.SetParticleEnergy(ekin*keV);
        gun.GeneratePrimaryVertex(event);

        ////FOR TEST/////////
        //  gun.SetParticleDefinition        ( part_geantino );
        //  gun.SetParticlePosition          ( G4ThreeVector (-4.5*cm, 0, -50*mm)  );
        //  gun.SetParticleMomentumDirection ( G4ThreeVector(0.,0.,1.) );
        //  gun.SetParticleEnergy            ( ekin*keV );
        //  gun.GeneratePrimaryVertex        ( event );
      
    }
  }
}

void Wisard_Generator::ROOT_GENERATOR(G4Event *event)
{
  // G4cout << "ROOT GENERATOR" << G4endl;
  G4ThreeVector beam = Beam();
  G4ThreeVector catcher_implementation = Catcher_Implementation();

  Reader->SetEntry(event->GetEventID());

  for (long unsigned int ipar = 0; ipar < (*code).GetSize(); ipar++)
  {  
    dir = G4ThreeVector((*px)[ipar], (*py)[ipar], (*pz)[ipar]);
    gun.SetParticleDefinition(particle_table->FindParticle((*code)[ipar]));
    gun.SetParticlePosition(beam+catcher_implementation);
    gun.SetParticleMomentumDirection(dir);
    gun.SetParticleEnergy((*ekin_)[ipar] * keV);
    gun.SetParticleTime((*time_)[ipar] * ns);
    gun.GeneratePrimaryVertex(event);

      // //////FOR TEST/////////
      // G4double a = -4.92 * mm;
      // a = -10 * mm;
      // gun.SetParticleDefinition(part_geantino);
      // gun.SetParticleDefinition(part_proton);
      //  gun.SetParticlePosition          ( G4ThreeVector (0, 0, -6*cm)  );
      // gun.SetParticlePosition(G4ThreeVector(15 * mm, a * cos((49.8) * degree) * mm, a * sin(49.8 * degree) * mm));
      //  gun.SetParticlePosition          ( G4ThreeVector (0, -55*mm, 42.7*mm)  ); // measueing space between the strips 1
      //  gun.SetParticlePosition          ( G4ThreeVector (0, -55*mm, 24.1*mm)  ); //measuring space between the strips 5
      // gun.SetParticleMomentumDirection(G4ThreeVector(0., cos(49.8 * degree), -sin(49.8 * degree)));
      // gun.SetParticleMomentumDirection ( G4ThreeVector(0.,0.,1.) );
      // gun.SetParticleEnergy(**ekin_ * keV);
      // gun.GeneratePrimaryVertex(event);
  }    
}

void Wisard_Generator::ROOT_DISTRIBUTION_GENERATOR(G4Event *event)
{
  if (event_counter % 10000 == 0)
  {
    event_counter = 0;
    for (int i = 0; i < 10000; i++)
    {
      G4ThreeVector beam = Beam();
      G4ThreeVector catcher_implentation = Catcher_Implementation();

      // shoot isotropicaly in the sphere
      G4double phi = G4UniformRand() * 2 * M_PI;
      G4double costheta = 2 * G4UniformRand() - 1;
      G4double theta = acos(costheta);
      dir = G4ThreeVector(sin(theta) * cos(phi), sin(theta) * sin(phi), -abs(cos(theta)));

      position_array[i] = beam+catcher_implentation;
      direction_array[i] = dir;
      // energy_array[i] = 200*keV;
    }
  }

  gun.SetParticleDefinition(particle);
  gun.SetParticlePosition(position_array[event_counter]);
  gun.SetParticleMomentumDirection(direction_array[event_counter]);
  gun.SetParticleEnergy(energy_array[event_counter]);
  gun.GeneratePrimaryVertex(event);

  event_counter++;
}

void Wisard_Generator::ION_GENERATOR(G4Event *event)
{
  // G4cout << "ION GENERATOR" << G4endl;
  G4ThreeVector beam = Beam();

  gun.SetParticlePosition(pos+beam);
  gun.SetParticleDefinition(Gun_Particle);
  gun.SetParticleCharge(0);
  gun.SetParticleEnergy(energy);
  gun.SetParticleMomentumDirection(GetDirection(dir));
  gun.GeneratePrimaryVertex(event);
}