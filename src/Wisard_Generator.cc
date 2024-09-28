#include "Wisard_Generator.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"

//----------------------------------------------------------------------

// Constructor
////--------------------------------------------------
////  Added pointer to RunManager in constructor
Wisard_Generator::Wisard_Generator(Wisard_RunManager *mgr)
{
  cout << "Constructor Wisard_Generator" << endl;

  manager_ptr = mgr;

  ////--------------------------------------------------
  ////  Memorize particle definitions

  part_gamma = particle_table->FindParticle("gamma");
  part_electron = particle_table->FindParticle("e-");
  part_positron = particle_table->FindParticle("e+");
  part_proton = particle_table->FindParticle("proton");
  part_alpha = particle_table->FindParticle("alpha");
  part_geantino = particle_table->FindParticle("geantino");
}

// Destructor
Wisard_Generator::~Wisard_Generator()
{
  cout << "Destructor Wisard_Generator" << endl;
}

//----------------------------------------------------------------------
// This function creates the primary particles for events
// It is called by run manager the event loop
void Wisard_Generator::GeneratePrimaries(G4Event *event)
{
  // Ouverture du fichier
  if (event->GetEventID() == 0)
  {
    res = Wisard_Generator::GetSRIM_hist();
   

    if (manager_ptr->GetInputName().find(".txt") != std::string::npos)
    {
      GENERATOR = std::bind(&Wisard_Generator::TXT_GENERATOR, this, std::placeholders::_1);
    }
    else if (manager_ptr->GetInputName().find(".root") != std::string::npos)
    {
      root_file = new TFile(manager_ptr->GetInputName().c_str(), "READ");
      if (manager_ptr->GetInputName().find("RMATRIX") != std::string::npos)
      {
        if (manager_ptr->GetInputName().find("32Ar") == std::string::npos && manager_ptr->GetInputName().find("33Ar") == std::string::npos)
        {
          particle = part_alpha;
        }
        else
        {
          particle = part_proton;
        }
        GENERATOR = std::bind(&Wisard_Generator::ROOT_DISTRIBUTION_GENERATOR, this, std::placeholders::_1);
        Energy_Hist = (TH1D*)root_file->Get("histogram");
      }
      else
      {      
        GENERATOR = std::bind(&Wisard_Generator::ROOT_GENERATOR, this, std::placeholders::_1);
        Reader = std::make_unique<TTreeReader>("ParticleTree", root_file);
        code = std::make_unique<TTreeReaderValue<int>>(*Reader, "code");
        eventid = std::make_unique<TTreeReaderValue<int>>(*Reader, "event");
        ekin_ = std::make_unique<TTreeReaderValue<double>>(*Reader, "energy");
        px = std::make_unique<TTreeReaderValue<double>>(*Reader, "px");
        py = std::make_unique<TTreeReaderValue<double>>(*Reader, "py");
        pz = std::make_unique<TTreeReaderValue<double>>(*Reader, "pz");
      }
    }
    else
    {
      G4Exception("Wisard_Generator::GeneratePrimaries", "CRADLE file wrong type (.root or .txt only)", JustWarning, "");
    }
  }

  GENERATOR(event);
}

void Wisard_Generator::TXT_GENERATOR(G4Event *event)
{
  int eventNr, totpartNr, currentsubevent, subeventNr;
  int currentpartNr = 0;

  manager_ptr->GetInput_TXT() >> eventNr >> totpartNr;
  //cout << eventNr << "\t" << totpartNr << endl;

  double x = 10 * mm;
  double y = 10 * mm;
  double z;


  tuple = GetSRIM_data(res.first, res.second);

  while (x >= x_beam + radius || x <= x_beam - radius)
  {
    x = G4RandGauss::shoot(x_beam, radius_std);
  }
  x += get<0>(tuple) * angstrom;

  while (y >= y_beam + radius || y <= y_beam - radius)
  {
    y = G4RandGauss::shoot(y_beam, radius_std);
  }
  y += get<1>(tuple) * angstrom;

  z = get<2>(tuple) * angstrom;

  while (currentpartNr < totpartNr)
  {
    manager_ptr->GetInput_TXT() >> eventNr >> currentsubevent >> subeventNr;
    //cout << eventNr << "\t" << currentsubevent << "\t" << subeventNr << endl;

    for (int subindex = 0; subindex < subeventNr; subindex++)
    {
      string name;
      double ekin, exc, mom[4], time;
      manager_ptr->GetInput_TXT() >> ievent >> time >> name >> exc >> ekin >> mom[0] >> dir[0] >> dir[1] >> dir[2];
      //cout << "\t" << ievent << "\t" << time << "\t" << name << "\t" << exc << "\t" << ekin << "\t" <<  mom[0] << "\t" <<  mom[1] << "\t" <<  mom[2] << "\t" <<  mom[3] << endl;
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
        gun.SetParticlePosition(G4ThreeVector(x, y, z));
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
  double x = 10 * mm;
  double y = 10 * mm;
  double z;

  tuple = GetSRIM_data(res.first, res.second);

  while (x >= x_beam + radius || x <= x_beam - radius)
  {
    x = G4RandGauss::shoot(x_beam, radius_std);
  }
  x += get<0>(tuple) * angstrom;

  while (y >= y_beam + radius || y <= y_beam - radius)
  {
    y = G4RandGauss::shoot(y_beam, radius_std);
  }
  y += get<1>(tuple) * angstrom;

  z = position_catcher_z + get<2>(tuple) * angstrom;

  // G4ThreeVector position = GetPosition();
  //     x = position.x();
  //     y = position.y();
  //     z = position.z();

  while (Reader->Next() && **eventid == event->GetEventID())
  {
    if (((**code <= 2212 && **code != 12 ) || **code == 1000020040) && **code != 11) //////GAMMA EXCLUSION BUT TAKING ALPHA 
    {
      dir[0] = **px;
      dir[1] = **py;
      dir[2] = **pz;

      gun.SetParticleDefinition(particle_table->FindParticle(**code));
      gun.SetParticlePosition(G4ThreeVector(x, y, z));
      gun.SetParticleMomentumDirection(dir);
      gun.SetParticleEnergy(**ekin_* keV);
      gun.GeneratePrimaryVertex(event);

      // //////FOR TEST/////////
      // double a = -5.6*mm;
      //    gun.SetParticleDefinition        ( part_geantino );
      //     gun.SetParticlePosition          ( G4ThreeVector (0, a*cos((40.2+90)*degree)*mm, a*cos(40.2*degree)*mm)  ); 
      //   //  gun.SetParticlePosition          ( G4ThreeVector (0, -55*mm, 42.7*mm)  ); // measueing space between the strips 1
      //   //  gun.SetParticlePosition          ( G4ThreeVector (0, -55*mm, 24.1*mm)  ); //measuring space between the strips 5
        //  gun.SetParticleMomentumDirection ( G4ThreeVector(0.,sin(40.2*degree),cos(40.2*degree)) );
        //  gun.SetParticleEnergy            ( **ekin_*MeV );
        //  gun.GeneratePrimaryVertex        ( event );
    }
  }
  (*Reader).SetEntry((*Reader).GetCurrentEntry() - 1);
}


void Wisard_Generator::ROOT_DISTRIBUTION_GENERATOR(G4Event* event)
{ 
  if (event_counter % 10000 == 0)
  {
    event_counter = 0;
    for (int i = 0; i < 10000; i++)
    {
      double x = 0 * mm;
      double y = 0 * mm;
      double z = 0 * mm;

      tuple = GetSRIM_data(res.first, res.second);

      while (x >= x_beam + radius || x <= x_beam - radius)
      {
        x = G4RandGauss::shoot(x_beam, radius_std);
      }
      x += get<0>(tuple) * angstrom;

      while (y >= y_beam + radius || y <= y_beam - radius)
      {
        y = G4RandGauss::shoot(y_beam, radius_std);
      }
      y += get<1>(tuple) * angstrom;

      z = position_catcher_z + get<2>(tuple) * angstrom;

      // G4ThreeVector position = GetPosition();
      // x = position.x();
      // y = position.y();
      // z = position.z();

      
      // shoot isotropicaly in the sphere
      double phi = G4UniformRand() * 2 * M_PI;
      double costheta = 2 * G4UniformRand() - 1;
      double theta = acos(costheta);
      double px = sin(theta) * cos(phi);
      double py = sin(theta) * sin(phi);
      double pz = cos(theta);

      position_array[i] = G4ThreeVector(x, y, z);
      direction_array[i] = G4ThreeVector(px, py, pz);
      energy_array[i] = Energy_Hist->GetRandom()*keV;
    }
  }

  gun.SetParticleDefinition(particle);
  gun.SetParticlePosition(position_array[event_counter]);
  gun.SetParticleMomentumDirection(direction_array[event_counter]);
  gun.SetParticleEnergy(energy_array[event_counter]);
  gun.GeneratePrimaryVertex(event);



  event_counter++;

}
