
#include "Wisard_Messenger.hh"
#include "Wisard_RunManager.hh"
#include "Wisard_Detector.hh"
#include "Wisard_Generator.hh"

//----------------------------------------------------------------------
// constructor of the messenger: define the commands

Wisard_Messenger::Wisard_Messenger ( Wisard_RunManager * mgr )
{
  cout << "Constructor Wisard_Messenger" << endl;

  manager_ptr = mgr;

  // define the commands
  DefineInputCommands ( );
  DefineResultCommands ( );

}

// destructor: delete all allocated command objects
Wisard_Messenger::~Wisard_Messenger ( )
{
  cout << "Destructor Wisard_Messenger" << endl;

  delete input_cmd_close;
  delete input_cmd_open;
  delete input_dir;

  // delete result_cmd_clear_histo;
  delete result_cmd_save_histo;
  delete result_dir;
}


//----------------------------------------------------------------------
// functions defining the commands


// - for input file
void Wisard_Messenger::DefineInputCommands ( )
{
  // commands directory
  input_dir = new G4UIdirectory("/input/");
  input_dir->SetGuidance("Input file commands directory");

  // commands
  input_cmd_open = new G4UIcmdWithAString("/input/openFile",this);
  input_cmd_open->SetGuidance("Open the input events file.");
  input_cmd_open->SetParameterName("filename",false);
  input_cmd_open->AvailableForStates ( G4State_PreInit, G4State_Idle );

  input_cmd_close = new G4UIcmdWithoutParameter("/input/closeFile",this);
  input_cmd_close->SetGuidance("Close the input file.");
  input_cmd_close->AvailableForStates ( G4State_PreInit, G4State_Idle );

  // input_cmd_implantation = new G4UIcmdWithADouble("/implantation",this);
  // input_cmd_implantation->SetGuidance("Taking of implantation point");
  // input_cmd_implantation->AvailableForStates ( G4State_PreInit, G4State_Idle );
}


// - for simulation results
void Wisard_Messenger::DefineResultCommands ( )
{
  // commands directory
  result_dir = new G4UIdirectory("/result/");
  result_dir->SetGuidance("Simulation results commands directory");
}


//----------------------------------------------------------------------
// function processing the commands
void Wisard_Messenger::SetNewValue ( G4UIcommand * cmd, G4String args )
{

  // input file commands
  if (cmd == input_cmd_close) { manager_ptr->CloseInput ( ); }
  if (cmd == input_cmd_open)  { manager_ptr->OpenInput ( args ); }
  // if (cmd == input_cmd_implantation) {manager_ptr->GetImplantation(); }

  // result commands
  //if (cmd == result_cmd_save_histo)  { manager_ptr->SaveHisto ( ); }
}
