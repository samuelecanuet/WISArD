#include "Wisard_Generator.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "Randomize.hh"

Wisard_Generator::Wisard_Generator(Wisard_RunManager *mgr)
{
 G4cout << "\033[32m" << "Constructor Wisard_Generator"  << "\033[0m" << G4endl;

  manager_ptr = mgr;

  part_proton = particle_table->FindParticle("proton");
  part_alpha = particle_table->FindParticle("alpha");
  part_geantino = particle_table->FindParticle("geantino");

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
      .SetDefaultValue("SRIM_data/AlMylar_2021_32Ar.txt");

  InputMessenger->DeclareProperty("Ion", Nucleus)
      .SetGuidance("Set Ion nucleus.")
      .SetParameterName("Ion", false)
      .SetDefaultValue("");
}

Wisard_Generator::~Wisard_Generator()
{
  InputROOT->Close();
  SRIMTXT.close();
  delete InputROOT;
  delete BeamMessenger;
  delete InputMessenger;
 G4cout<< "Destructor Wisard_Generator" <<G4endl;
}

//----------------------------------------------------------------------

void Wisard_Generator::GeneratePrimaries(G4Event *event)
{
  // Ouverture du fichier
  if (event->GetEventID() == 0)
  {
    res = Wisard_Generator::GetSRIM_hist();
    ChooseGENERATOR();
  }

  GENERATOR(event);
}

void Wisard_Generator::TXT_GENERATOR(G4Event *event)
{
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

  G4ThreeVector beam = Beam();


  while (Reader->Next() && **eventid == event->GetEventID())
  {
    if (((**code <= 2212 && **code != 12 ) || **code == 1000020040) && **code != 11) //////GAMMA EXCLUSION BUT TAKING ALPHA 
    {
      dir[0] = **px;
      dir[1] = **py;
      dir[2] = **pz;

    //   const G4double c = 299792458;  // speed of light in m/s
    // const G4double MeV_to_keV = 1000.0;  // conversion factor
    // const G4double m_p_MeV = 938.272;  // proton mass in MeV/c^2
    // const G4double m_Cl_MeV = 32.0 * 931.5;  // 32Cl mass in MeV/c^2
    // const G4double K_Cl_lab_keV = 30.0;  // kinetic energy of 32Cl in lab frame (in keV)
    // const G4double K_p_rest_keV = **ekin_;  // proton kinetic energy in rest frame (in keV)

    //G4double Cl_direction[3] = {0.0, 0.0, -1.0};
    //G4double gamma_Cl_lab = 1 + K_Cl_lab_keV / (m_Cl_MeV * MeV_to_keV);
    //G4double beta_Cl_lab = std::sqrt(1.0 - 1.0 / (gamma_Cl_lab * gamma_Cl_lab));
    //G4double dot_product = Cl_direction[0] * dir[0] + Cl_direction[1] * dir[1] + Cl_direction[2] * dir[2];
    //G4double magnitude_dir = std::sqrt(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
    //G4double cos_theta = dot_product / magnitude_dir;
    //G4double E_p_rest_MeV = K_p_rest_keV / MeV_to_keV + m_p_MeV;  // total energy in rest frame (MeV)
    //G4double p_p_rest_MeV = std::sqrt(E_p_rest_MeV * E_p_rest_MeV - m_p_MeV * m_p_MeV);  // momentum in rest frame (MeV/c)
    //G4double E_p_lab_MeV = gamma_Cl_lab * (E_p_rest_MeV + beta_Cl_lab * p_p_rest_MeV * cos_theta);
    //G4double ekin_lab_keV = (E_p_lab_MeV - m_p_MeV) * MeV_to_keV;

    gun.SetParticleDefinition(particle_table->FindParticle(**code));
    gun.SetParticlePosition(beam);
    gun.SetParticleMomentumDirection(dir);
    gun.SetParticleEnergy(**ekin_*keV);
    gun.GeneratePrimaryVertex(event);

    // //////FOR TEST/////////
    //G4double a = -5.65*mm;
    //    gun.SetParticleDefinition        ( part_gamma);
    //    gun.SetParticlePosition          ( G4ThreeVector (0, 0, -6*cm)  );
    //   gun.SetParticlePosition          ( G4ThreeVector (0, a*cos((40.2+90)*degree)*mm, a*cos(40.2*degree)*mm)  );
    //  gun.SetParticlePosition          ( G4ThreeVector (0, -55*mm, 42.7*mm)  ); // measueing space between the strips 1
    //  gun.SetParticlePosition          ( G4ThreeVector (0, -55*mm, 24.1*mm)  ); //measuring space between the strips 5
    //  gun.SetParticleMomentumDirection ( G4ThreeVector(0.,sin(40.2*degree),cos(40.2*degree)) );
    // gun.SetParticleMomentumDirection ( G4ThreeVector(0.,0.,1.) );
    //  gun.SetParticleEnergy            ( **ekin_*keV );
    //  gun.GeneratePrimaryVertex        ( event );
    }
  }
  (*Reader).SetEntry((*Reader).GetCurrentEntry() - 1);
}

void Wisard_Generator::ROOT_DISTRIBUTION_GENERATOR(G4Event *event)
{
  if (event_counter % 10000 == 0)
  {
    event_counter = 0;
    for (int i = 0; i < 10000; i++)
    {
      G4ThreeVector beam = Beam();

      // shoot isotropicaly in the sphere
     G4double phi = G4UniformRand() * 2 * M_PI;
     G4double costheta = 2 * G4UniformRand() - 1;
     G4double theta = acos(costheta);
      dir = G4ThreeVector(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));

      position_array[i] = beam;
      direction_array[i] = dir;
      energy_array[i] = Energy_Hist->GetRandom() * keV;
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
  G4ThreeVector beam = Beam();

  G4ParticleDefinition *ion = G4IonTable::GetIonTable()->GetIon(Z, A, 0. * keV);
  gun.SetParticlePosition(beam);
  gun.SetParticleDefinition(ion);
  gun.SetParticleCharge(0);
  gun.SetParticleEnergy(0);
  gun.GeneratePrimaryVertex(event);
}