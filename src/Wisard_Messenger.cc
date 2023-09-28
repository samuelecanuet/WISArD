#include "Wisard_Messenger.hh"
#include "Wisard_RunManager.hh"

//----------------------------------------------------------------------
// constructor of the messenger: define the commands

Wisard_Messenger::Wisard_Messenger(Wisard_RunManager *mgr, Wisard_Detector *det, Wisard_Generator *gen)
{
  cout << "Constructor Wisard_Messenger" << endl;

  manager_ptr = mgr;
  det_ptr = det;
  gen_ptr = gen;

  // define the commands
  DefineInputCommands();
}

// destructor: delete all allocated command objects
Wisard_Messenger::~Wisard_Messenger()
{
  cout << "Destructor Wisard_Messenger" << endl;

  delete input_dir;
  delete input_cmd_open;
  delete input_cmd_open_SRIM;
  delete input_cmd_B;
  delete input_cmd_th;
  delete input_cmd_open_Field;
}

//----------------------------------------------------------------------
// functions defining the commands

// - for input file
void Wisard_Messenger::DefineInputCommands()
{
  // commands directory
  input_dir = new G4UIdirectory("/input/");
  input_dir->SetGuidance("Input file commands directory");

  // commands

  // Open CRADLE++ file event
  input_cmd_open = new G4UIcmdWithAString("/input/Open_CRADLE", this);
  input_cmd_open->SetGuidance("Open the input events file.");
  input_cmd_open->SetParameterName("filename", false);
  input_cmd_open->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Open SRIM file event
  input_cmd_open_SRIM = new G4UIcmdWithAString("/input/Open_SRIM", this);
  input_cmd_open_SRIM->SetGuidance("Open the input implantation file.");
  input_cmd_open_SRIM->SetParameterName("filename", false);
  input_cmd_open_SRIM->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Open Magnetic Field file
  input_cmd_open_Field = new G4UIcmdWithAString("/input/Open_B", this);
  input_cmd_open_Field->SetGuidance("Open the input magnetic field file.");
  input_cmd_open_Field->SetParameterName("filename", false);
  input_cmd_open_Field->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Set Output root filename
  input_cmd_filename = new G4UIcmdWithAString("/output", this);
  input_cmd_filename->SetGuidance("Set root filename");
  input_cmd_filename->SetParameterName("filename", false);
  input_cmd_filename->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Set B field Value
  input_cmd_B = new G4UIcmdWithAString("/Magnetic_Field", this);
  input_cmd_B->SetGuidance("Set magnetic field value");
  input_cmd_B->SetParameterName("B", false);
  input_cmd_B->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Set Plastic Sintillator threshoold for histograms
  input_cmd_th = new G4UIcmdWithAString("/Threshoold_PlasticScintillator", this);
  input_cmd_th->SetGuidance("Set Threshoold PlasticScintillator");
  input_cmd_th->SetParameterName("threshoold", false);
  input_cmd_th->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Set Ion Beam Size
  input_cmd_beam_size = new G4UIcmdWithAString("/Beam_Size", this);
  input_cmd_beam_size->SetGuidance("Set Beam Size");
  input_cmd_beam_size->SetParameterName("beam_size", false);
  input_cmd_beam_size->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Set Ion Beam position
  input_cmd_beam_position = new G4UIcmdWithAString("/Beam_Position", this);
  input_cmd_beam_position->SetGuidance("Set Beam Position");
  input_cmd_beam_position->SetParameterName("beam_position", false);
  input_cmd_beam_position->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Set Ion Beam position
  input_cmd_dl_thickness = new G4UIcmdWithAString("/SiDeadLayer_Thickness", this);
  input_cmd_dl_thickness->SetGuidance("Set Silicon Dead Layer Thickness");
  input_cmd_dl_thickness->SetParameterName("dl_th", false);
  input_cmd_dl_thickness->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Set Ion Beam position
  input_cmd_catcher_z = new G4UIcmdWithAString("/Catcher_Position_z", this);
  input_cmd_catcher_z->SetGuidance("Set Catcher position");
  input_cmd_catcher_z->SetParameterName("catcher_z", false);
  input_cmd_catcher_z->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Set Catcher Thickness
  input_cmd_catcher_thickness = new G4UIcmdWithAString("/CatcherMylar_Thickness", this);
  input_cmd_catcher_thickness->SetGuidance("Set Catcher thickness");
  input_cmd_catcher_thickness->SetParameterName("catcher_thickness", false);
  input_cmd_catcher_thickness->AvailableForStates(G4State_PreInit, G4State_Idle);
}

//----------------------------------------------------------------------
// function processing the commands
void Wisard_Messenger::SetNewValue(G4UIcommand *cmd, G4String args)
{

  // input file commands

  if (cmd == input_cmd_open)
  {
    manager_ptr->OpenInput(args);
  }
  if (cmd == input_cmd_open_SRIM)
  {
    manager_ptr->OpenInputSRIM(args);
  }
  if (cmd == input_cmd_open_Field)
  {
    det_ptr->OpenInputB(args);
  }
  if (cmd == input_cmd_B)
  {
    G4double value;
    G4String unit;
    std::istringstream iss(args);
    iss >> value >> unit;
    det_ptr->SetBFieldValue(value * G4UnitDefinition::GetValueOf(unit));
  }
  if (cmd == input_cmd_th)
  {
    G4double value;
    G4String unit;
    std::istringstream iss(args);
    iss >> value >> unit;
    manager_ptr->SetThreshoold(value * G4UnitDefinition::GetValueOf(unit));
  }
  if (cmd == input_cmd_beam_size)
  {
    G4double value, value1;
    G4String unit, unit1;
    std::istringstream iss(args);
    iss >> value >> unit >> value1 >> unit1;
    gen_ptr->SetBeamSize(value * G4UnitDefinition::GetValueOf(unit), value1 * G4UnitDefinition::GetValueOf(unit1));
  }
  if (cmd == input_cmd_beam_position)
  {
    G4double value, value1;
    G4String unit, unit1;
    std::istringstream iss(args);
    iss >> value >> unit >> value1 >> unit1;
    gen_ptr->SetBeamPosition(value * G4UnitDefinition::GetValueOf(unit), value1 * G4UnitDefinition::GetValueOf(unit1));
  }
  if (cmd == input_cmd_catcher_z)
  {
    G4double value;
    G4String unit;
    std::istringstream iss(args);
    iss >> value >> unit;
    det_ptr->SetCatcherPosition_z(value * G4UnitDefinition::GetValueOf(unit));
    gen_ptr->SetCatcherPosition_z(value * G4UnitDefinition::GetValueOf(unit));
  }
  if (cmd == input_cmd_filename)
  {
    manager_ptr->SetOutputFilename(args);
  }
  if (cmd == input_cmd_dl_thickness)
  {
    G4double value;
    G4String unit;
    std::istringstream iss(args);
    iss >> value >> unit;
    det_ptr->SetSiDeadLayer_Thickness(value * G4UnitDefinition::GetValueOf(unit));
  }
  if (cmd == input_cmd_catcher_thickness)
  {
    G4double value;
    G4String unit;
    std::istringstream iss(args);
    iss >> value >> unit;
    det_ptr->SetCatcher_Thickness(value * G4UnitDefinition::GetValueOf(unit));
  }
}
