//======================================================================
//  Basic example of Geant4 simulation main program
//======================================================================

// declaration of the classes used in the main function
#include "Wisard_RunManager.hh"
#include "Wisard_Detector.hh"
#include "Wisard_PhysList.hh"
#include "Wisard_Generator.hh"

// include files for interactive sessions and for display
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "G4AnalysisManager.hh"
//----------------------------------------------------------------------
// program main function

int main ( int argc,char** argv )
{

  int seedValue = 123456789;
  // G4Random::setTheSeed(seedValue);

  G4String output = "test";
  G4double implantation = 47;
  G4double std_implantation = 14;
  char* endptr1;
  char* endptr2;

  if (argc >= 3)
  {
    output = argv[2];
  }

  if (argc == 5)
  {
    implantation = std::strtod(argv[3], &endptr1);
    std_implantation = std::strtod(argv[4], &endptr2);
  }

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetFileName(output+".root");

  //get run time
  clock_t t1,t2;
  t1=clock();

  //------------------------------------------------------------
  //  Simulation customisation

  // define the core simulation object
  Wisard_RunManager run;

  // create the detector definition
	Wisard_Detector * ptr_det = new Wisard_Detector ( &run ) ;
  run.SetUserInitialization ( ptr_det );

  // create the physics list
  Wisard_PhysList * ptr_phys = new Wisard_PhysList;
  run.SetUserInitialization ( ptr_phys );

  // create the generator for events
          ////--------------------------------------------------
          ////  JG 2013/07/08   Added pointer to RunManager
          ////                  in constructor
  Wisard_Generator * ptr_gene = new Wisard_Generator ( &run , implantation, std_implantation);
  run.SetUserAction ( ptr_gene );


          ////--------------------------------------------------
          ////  JG 2013/07/08   U.I. commands creation
  // create the associated commands
  run.DefineSimulationCommands();


  //------------------------------------------------------------
  //  Session start

  // initialise the simulation
  run.Initialize();

  // the process must be added after initialization (so that particles
  // are defined)
  ptr_phys->AddStepMax ( 1*mm, 0x2 );

  // set the details of Geant4 messages
  G4EventManager::GetEventManager()->SetVerboseLevel(0);
  G4EventManager::GetEventManager()->GetTrackingManager()->SetVerboseLevel(0);


  // visualisation manager (required for displays)
  G4cout << G4endl << "Visu manager creation" << G4endl;
  G4VisManager* visu_manager = new G4VisExecutive;
  visu_manager->Initialize();


  // get the command interpreter pointer
  G4cout << "Get user interface (command interpreter)" << G4endl;
  G4UImanager * UI = G4UImanager::GetUIpointer();


	// execution of a macro given as command-line argument
	if (argc > 1)
	{
  	G4String command = "/control/execute ";
  	G4String fileName = argv[1];
  	G4cout << G4endl << "Reading macro file: " << fileName << G4endl;
  	UI->ApplyCommand(command+fileName);
  }

  else
	// start of an interactive session
  {
  	G4cout << "Starting a tcsh interactive session" << G4endl;
    G4UIExecutive * session = new G4UIExecutive(argc, argv);

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

    // save the data
  //run.SaveHisto ();
  analysisManager->CloseFile();
  //get run tiime
  t2=clock();
  float diff ((float)t2-(float)t1);
  cout << " " << endl;
  cout << "<I>: Run time: " << diff/1.e6 << " s" << endl;
  cout << " " << endl;

  delete visu_manager;

  return (0);
}