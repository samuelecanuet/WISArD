#include "Wisard_RunManager.hh"
#include "Wisard_Detector.hh"
#include "Wisard_PhysList.hh"
#include "Wisard_Generator.hh"
#include "Wisard_Tracking.hh"
#include "ParticleInformation.hh"

#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "TSystem.h"
#include "TInterpreter.h"

#include "FTFP_BERT.hh"
#include "G4EmStandardPhysicsGS.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "G4EmParameters.hh"
#include "G4LossTableManager.hh"
#include "G4RadioactiveDecay.hh"
#include "G4StepLimiterPhysics.hh"

//----------------------------------------------------------------------
// program main function

int main(int argc, char **argv)
{
  system("Logo/logo.ans");
  // G4Random::setTheSeed(123456789);

  // get run time
  clock_t t1, t2;
  t1 = clock();

  //------------------------------------------------------------

  G4cout << "Generating Dictionnaries" << G4endl;
  gSystem->AddIncludePath("-I/softs/clhep/2.4.6.2/include/");
  gSystem->AddIncludePath("-I/softs/clhep/2.4.6.2/include/CLHEP/Vector/");

  gInterpreter->GenerateDictionary("vector<vector<int> >", "vector");
  gInterpreter->GenerateDictionary("vector<vector<vector<double> > >", "vector");
  gInterpreter->GenerateDictionary("vector<CLHEP::Hep3Vector>", "vector;ThreeVector.h");
  gInterpreter->GenerateDictionary("vector<vector<CLHEP::Hep3Vector>>", "vector;ThreeVector.h");
  gInterpreter->GenerateDictionary("vector<vector<vector<CLHEP::Hep3Vector>>>", "vector;ThreeVector.h");

  //------------------------------------------------------------

  ParticleInformation *particle_info = new ParticleInformation();

  Wisard_RunManager run(particle_info);

  Wisard_Detector *ptr_det = new Wisard_Detector(&run);
  run.SetUserInitialization(ptr_det);

  Wisard_PhysList *ptr_phys = new Wisard_PhysList;
  run.SetUserInitialization(ptr_phys);

  // G4VModularPhysicsList *ptr_phys = new FTFP_BERT();
  // ptr_phys->RemovePhysics(2);
  // ptr_phys->RegisterPhysics( new G4EmStandardPhysicsGS(0));
  // ptr_phys->RegisterPhysics(new G4RadioactiveDecayPhysics());
  // ptr_phys->SetDefaultCutValue(1 * nm);
  // G4EmParameters *emParams = G4EmParameters::Instance();
  // emParams->SetNumberOfBinsPerDecade(200);
  // ptr_phys->RegisterPhysics(new G4StepLimiterPhysics());
  // run.SetUserInitialization(ptr_phys);

  Wisard_Tracking *ptr_track = new Wisard_Tracking(particle_info);
  run.SetUserAction(ptr_track);
  
  
  Wisard_Generator *ptr_gene = new Wisard_Generator(&run);
  run.SetUserAction(ptr_gene);

  run.Initialize();
  

  //------------------------------------------------------------
  //  Session start
  ptr_phys->AddStepMax(1 * mm, 0x2);

  G4EventManager::GetEventManager()->SetVerboseLevel(0);
  G4EventManager::GetEventManager()->GetTrackingManager()->SetVerboseLevel(0);

  G4VisManager *visu_manager = new G4VisExecutive("0");
  visu_manager->Initialize();

  G4UImanager *UI = G4UImanager::GetUIpointer();

  if (argc > 1)
  {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];

    // read the macro file and cout if line begin by /Input/CRADLE or /Input/Ion without the # to see the mode
    std::ifstream file(fileName);
    std::string line;
    G4cout << G4endl;
    while (std::getline(file, line))
    {
      if (line.find("/Input/CRADLE") != std::string::npos || line.find("/Input/Ion") != std::string::npos || line.find("/Input/Particle") != std::string::npos)
      {
        if (line.find("#") == std::string::npos)
        {
          G4cout << "\033[34m" << line << "\033[0m" << G4endl;
        }
      }
    }

    G4cout << G4endl << "Reading macro file: " << fileName << G4endl;
    UI->ApplyCommand("/control/alias currentMacro " + fileName);
    UI->ApplyCommand(command + fileName);  
    
  }
  else
  // start of an interactive session
  {
    G4cout << "Starting a tcsh interactive session" << G4endl;
    G4UIExecutive *session = new G4UIExecutive(argc, argv);

    // start default viewer from macro
    UI->ApplyCommand("/control/execute visu.mac");
    G4cout << "--" << G4endl
           << "Use the following command for a display test" << G4endl
           << "->        /control/execute visu.mac" << G4endl
           << "--" << G4endl;

    // start the interactive session
    session->SessionStart();

    

    //  	G4cout << "End of interactive session" << G4endl << G4endl;
    delete session;
  }

  //------------------------------------------------------------

  t2 = clock();
  float diff((float)t2 - (float)t1);
 G4cout<< " " <<G4endl;
 G4cout<< "<I>: Run time: " << diff / 1.e6 << " s" <<G4endl;
 G4cout<< " " <<G4endl;

  delete visu_manager;

  return (0);
}
