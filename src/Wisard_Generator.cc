#include "Wisard_Generator.hh"
#include "G4IonTable.hh"
#include "G4ParticleGun.hh"
#include "G4AnalysisManager.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"
#include <CLHEP/Random/RandGaussQ.h>

  int ievent, iev_len=0, isubevent, isubev_len=0;
//----------------------------------------------------------------------

// Constructor
          ////--------------------------------------------------
          ////  JG 2013/07/08   Added pointer to RunManager
          ////                  in constructor
Wisard_Generator::Wisard_Generator( Wisard_RunManager * mgr, double implantation_, double std_implantation_)
{
  cout << "Constructor Wisard_Generator" << endl;

  manager_ptr = mgr;
  implantation = implantation_;
  std_implantation = std_implantation_;

          ////--------------------------------------------------
          ////  JG 2013/07/08   Memorize particle definitions
  G4ParticleTable * particle_table = G4ParticleTable::GetParticleTable();
  part_gamma    = particle_table->FindParticle("gamma");
  part_electron = particle_table->FindParticle("e-");
  part_positron = particle_table->FindParticle("e+");
  part_proton   = particle_table->FindParticle("proton");
  part_enubar   = particle_table->FindParticle("anti_nu_e");
  part_alpha    = particle_table->FindParticle("alpha");
  part_geantino    = particle_table->FindParticle("geantino");

}

// Destructor
Wisard_Generator::~Wisard_Generator()
{
  cout << "Destructor Wisard_Generator" << endl;
}

//----------------------------------------------------------------------
// This function creates the primary particles for events
// It is called by run manager the event loop
void Wisard_Generator::GeneratePrimaries ( G4Event * event)
{
  // find the pointer of the "gamma" particle in the registered particles
  G4ParticleTable       * particle_table = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition  * particle = particle_table->FindParticle("gamma");
  G4ThreeVector           dir;

  // define a "particle gun": a very simple way to create simple
  // primary vertexes
  G4ParticleGun gun;

          ////--------------------------------------------------
          ////  JG 2013/07/08   Modified read of data file
          ////
//  cout << " " << endl;
//  cout << "first:" << "iev_len =" << iev_len << " " << "isubev_len =" << isubev_len << endl;
//   if( iev_len == 0 )
//   {
//   manager_ptr->GetInput() >> ievent >> iev_len;
//   //cout << "hallo " << "ievent =" << ievent << " " << "iev_len =" << iev_len << endl;
//   }
// //  cout << "iev1 = " << ievent << " " << iev_len << endl;
// //  if ( ! manager_ptr->GetInput().fail() ) cout << "iev = " << ievent << " " << iev_len << endl;;
//
//   if ( manager_ptr->GetInput().fail() )
//   {
//     // rewind
//     manager_ptr->GetInput().clear();
//     manager_ptr->GetInput().seekg (0, ios::beg );
//     //cout << " " << endl;
//     cout << " file rewinded " << endl;
//     //cout << " file rewinded  EOF = " << manager_ptr->GetInput().eof() << endl;
//     //cout << " file rewinded  BAD = " << manager_ptr->GetInput().bad() << endl;
//     //cout << " file rewinded  FAIL = " << manager_ptr->GetInput().fail() << endl;
//
//     //cout << " ievlen = " << iev_len << " " << "isubev_len =" << isubev_len << endl;
//     if( iev_len - isubev_len <= 0 ) manager_ptr->GetInput() >> ievent >> iev_len;
//     //cout << "ievent = " << ievent << " " << "iev_len =" << iev_len << endl;;
//     manager_ptr->GetInput() >> ievent >> isubevent >> isubev_len;
//     //cout << "ievent = " << ievent << " " << "isubevent =" << isubevent << " " << "isubev_len =" << isubev_len << endl;
//     iev_len = iev_len - isubev_len;
//   }
//   else
//   {
//     manager_ptr->GetInput() >> ievent >> isubevent >> isubev_len;
//     iev_len = iev_len - isubev_len;
//     //cout << "iev = " << ievent << " " << isubevent << " " << isubev_len << endl;;
//   }
//   // manager_ptr->GetInput() >> ievent >> isubevent >> isubev_len;
//   // iev_len = iev_len - isubev_len;
//   //cout << "treating particle " << endl;



  if (! manager_ptr->GetInput().fail())
  {

    double x = 10*mm;
    double y = 10*mm;
    double z = -1;

     while (x >= 2*mm || x <= -2*mm)
     {
       x = G4RandGauss::shoot(0, 300*um);
     }

     while (y >= 2*mm || y <= -2*mm)
     {
       y = G4RandGauss::shoot(0, 300*um);
     }

     while (z  < 0)
     {
       z = G4RandGauss::shoot(implantation*nm, std_implantation*nm);
     }




    for (int i = 0; i <=3; ++i)
    {
      //cout << "sub event number = " << isubev_len << " " << i <<endl;
      string    name;
      double  ekin, exc, mom[4], time;
      manager_ptr->GetInput() >> ievent >> time >> name >> exc >> ekin >>  mom[0] >>  mom[1] >>  mom[2] >>  mom[3] ;
      //cout << "hi " << ievent << " " << time << " " << name << " " << exc << " " << ekin << endl;

      ekin = ekin / 1000.;

      int iopt = 1;

      if (name.compare(0,2,"e-") == 0)
        particle = part_electron;
      else if (name.compare(0,2,"e+") == 0)
      {
        particle = part_positron;
      }
      else if (name.compare(0,5,"gamma") == 0)
      {
        particle = part_gamma;
      }
      else if (name.compare(0,1,"p") == 0 )
      {
        particle = part_proton;
      }
      else if (name.compare(0,5,"alpha") == 0 )
      {
        particle = part_alpha;
      }
      else
      {
        iopt = 0;
      }



      double momentum = sqrt(mom[1]*mom[1] + mom[2]*mom[2] + mom[3]*mom[3]);

      dir[0] = mom[1] / momentum;
      dir[1] = mom[2] / momentum;
      dir[2] = mom[3] / momentum;

      //// For checks
//       cerr << "********" << endl;
//       cerr << name << " iopt = " << iopt << endl;
//       cerr << "   E= " << ekin/MeV << " MeV"
//                        << " mom =" << dir << endl;


      if ( iopt == 1 )
      {
      // cout << "iopt = " << iopt << " " << i << endl;

      //G4cout<<name<<"  "<<x<<"   "<<y<<"   "<<z<<G4endl;

       gun.SetParticleDefinition        ( particle );
       gun.SetParticlePosition          ( G4ThreeVector (x,y,47*nm-3*um)  );
       gun.SetParticleMomentumDirection ( dir );
       gun.SetParticleEnergy            ( ekin );
       gun.GeneratePrimaryVertex        ( event );

      //  gun.SetParticleDefinition        ( part_geantino );
      //  gun.SetParticlePosition          ( G4ThreeVector (x,y,47*nm-3*um)  );
      //gun.SetParticlePosition          ( G4ThreeVector (0,4.5*cm,-10*cm)  );
      //  gun.SetParticleMomentumDirection ( G4ThreeVector(0.,0.,1.) );
      //  gun.SetParticleEnergy            ( ekin );
      //  gun.GeneratePrimaryVertex        ( event );
      }
//      cout << "sub event number = " << isubev_len << " " << i <<endl;
    }

  }
  else

  {

    // treat simple events defined here

    gun.SetParticleDefinition        ( part_geantino);
    gun.SetParticlePosition          ( G4ThreeVector (2*cm,0., -10*cm));
    gun.SetParticleEnergy            ( 3.356*MeV );

//  generate directional distribution
    G4double u,v,w,r;
    G4ThreeVector vector;
//    do{
     r=G4UniformRand();
     w=-2.*r+1;
//    } while( w < 0.866025 );
    r=G4UniformRand();
    u=(std::sqrt(1.-w*w)*std::cos(2.*3.14159265*r));
    v=(std::sqrt(1.-w*w)*std::sin(2.*3.14159265*r));
    gun.SetParticleMomentumDirection(G4ThreeVector(u,v,w));


//  cout << "<I>: dir 1-3 = " << u << v << w << endl;

//  add the vertex to the event
    gun.GeneratePrimaryVertex        ( event );
}
}
