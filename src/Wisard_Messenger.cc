
#include "Wisard_Messenger.hh"
#include "Wisard_RunManager.hh"

//----------------------------------------------------------------------
// constructor of the messenger: define the commands

Wisard_Messenger::Wisard_Messenger ( Wisard_RunManager * mgr )
{
  cout << "Constructor Wisard_Messenger" << endl;

  manager_ptr = mgr;

  // define the commands
  DefineInputCommands ( );

}

// destructor: delete all allocated command objects
Wisard_Messenger::~Wisard_Messenger ( )
{
  cout << "Destructor Wisard_Messenger" << endl;

  delete input_dir;
  delete input_cmd_close;
  delete input_cmd_open;

  delete input_cmd_close_SRIM;
  delete input_cmd_open_SRIM;
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

  //Open CRADLE++ file event
  input_cmd_open = new G4UIcmdWithAString("/input/Open_CRADLE",this);
  input_cmd_open->SetGuidance("Open the input events file.");
  input_cmd_open->SetParameterName("filename",false);
  input_cmd_open->AvailableForStates ( G4State_PreInit, G4State_Idle );

  //Close CRADLE++ file event
  input_cmd_close = new G4UIcmdWithoutParameter("/input/Close_CRADLE",this);
  input_cmd_close->SetGuidance("Close the input file.");
  input_cmd_close->AvailableForStates ( G4State_PreInit, G4State_Idle );


  //Open SRIM file event
  input_cmd_open_SRIM = new G4UIcmdWithAString("/input/Open_SRIM",this);
  input_cmd_open_SRIM->SetGuidance("Open the input implantation file.");
  input_cmd_open_SRIM->SetParameterName("filename",false);
  input_cmd_open_SRIM->AvailableForStates ( G4State_PreInit, G4State_Idle );

  //Close SRIM file event
  input_cmd_close_SRIM = new G4UIcmdWithoutParameter("/input/Close_SRIM",this);
  input_cmd_close_SRIM->SetGuidance("Close the implantation file.");
  input_cmd_close_SRIM->AvailableForStates ( G4State_PreInit, G4State_Idle );
}

//----------------------------------------------------------------------
// function processing the commands
void Wisard_Messenger::SetNewValue ( G4UIcommand * cmd, G4String args )
{

  // input file commands
  if (cmd == input_cmd_close) { manager_ptr->CloseInput ( ); }
  if (cmd == input_cmd_open)  { manager_ptr->OpenInput ( args ); }

  if (cmd == input_cmd_close_SRIM) { manager_ptr->CloseInputSRIM ( ); }
  if (cmd == input_cmd_open_SRIM)  { manager_ptr->OpenInputSRIM ( args ); }

}
