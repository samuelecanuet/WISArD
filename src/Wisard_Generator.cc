#include "Wisard_Generator.hh"
#include "G4ParticleGun.hh"
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
  G4ParticleTable *particle_table = G4ParticleTable::GetParticleTable();
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
  G4ParticleDefinition *particle;
  G4ThreeVector dir;
  G4ParticleGun gun;

  if (event->GetEventID() == 0)
  {
    res = Wisard_Generator::GetSRIM_hist();
  }

  if (manager_ptr->GetInput().fail())
  {
    G4Exception("Wisard_Generator::GeneratePrimaries", ("CRADLE file fail on " + to_string(ievent) + " event").c_str(), JustWarning, "");
  }

  else
  {
    if (iev_len == 0)
    {
      manager_ptr->GetInput() >> ievent >> iev_len;
      manager_ptr->GetInput() >> ievent >> isubevent >> isubev_len;
    }

    manager_ptr->GetInput() >> ievent >> isubevent >> isubev_len;

    iev_len = iev_len - isubev_len;

    double x = 10 * mm;
    double y = 10 * mm;
    double z;

    tuple = GetSRIM_data(res.first, res.second);

    while (x >= x_beam + radius || x <= x_beam - radius)
    {
      x = G4RandGauss::shoot(x_beam, radius_std);
    }
    x += get<0>(tuple)*angstrom;

    while (y >= y_beam + radius || y <= y_beam - radius)
    {
      y = G4RandGauss::shoot(y_beam, radius_std);
    }
    y += get<1>(tuple)*angstrom;

    z = position_catcher_z + get<2>(tuple)*angstrom;
    

    for (int i = 0; i < isubev_len; ++i)
    {
      string name;
      double ekin, exc, mom[4], time;
      manager_ptr->GetInput() >> ievent >> time >> name >> exc >> ekin >> mom[0] >> mom[1] >> mom[2] >> mom[3];
      // cout << "\t\t" << ievent << "\t\t" << time << "\t\t" << name << "\t\t" << exc << "\t\t" << ekin << "\t\t" <<  mom[0] << "\t\t" <<  mom[1] << "\t\t" <<  mom[2] << "\t\t" <<  mom[3] << endl;
      ekin = ekin / 1000.;

      int iopt = 1;

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
        iopt = 0;
      }

      double momentum = sqrt(mom[1] * mom[1] + mom[2] * mom[2] + mom[3] * mom[3]);

      dir[0] = mom[1] / momentum;
      dir[1] = mom[2] / momentum;
      dir[2] = mom[3] / momentum;

      if (iopt == 1)
      {
        gun.SetParticleDefinition(particle);
        gun.SetParticlePosition(G4ThreeVector(x, y, z));
        gun.SetParticleMomentumDirection(dir);
        gun.SetParticleEnergy(ekin);
        gun.GeneratePrimaryVertex(event);

        //////FOR TEST/////////
        //  gun.SetParticleDefinition        ( part_geantino );
        //  gun.SetParticlePosition          ( G4ThreeVector (0*mm, 0, -30*mm)  );
        //  gun.SetParticleMomentumDirection ( G4ThreeVector(0.,0.,1.) );
        //  gun.SetParticleEnergy            ( ekin );
        //  gun.GeneratePrimaryVertex        ( event );
      }
    }
  }
}
