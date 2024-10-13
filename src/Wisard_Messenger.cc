#include "Wisard_Messenger.hh"

Wisard_Messenger::Wisard_Messenger(Wisard_Detector *det, Wisard_Generator *gen)
{
 G4cout<< "Constructor Wisard_Messenger" <<G4endl;

  det_ptr = det;
  gen_ptr = gen;

  DefineInputCommands();
}

Wisard_Messenger::~Wisard_Messenger()
{
 G4cout<< "Destructor Wisard_Messenger" <<G4endl;

  delete input_dir;
  delete input_cmd_B;
  delete input_cmd_open_Field;
}



void Wisard_Messenger::DefineInputCommands()
{
  // commands directory
  input_dir = new G4UIdirectory("/Input/");
  input_dir->SetGuidance("Input file commands directory");
  input_dir = new G4UIdirectory("/Geometry/");
  input_dir->SetGuidance("Geometry commands directory");

  // Open Magnetic Field file
  input_cmd_open_Field = new G4UIcmdWithAString("/Input/Open_B", this);
  input_cmd_open_Field->SetGuidance("Open the input magnetic field file.");
  input_cmd_open_Field->SetParameterName("filename", false);
  input_cmd_open_Field->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Set B field Value
  input_cmd_B = new G4UIcmdWithAString("/Geometry/Magnetic_Field", this);
  input_cmd_B->SetGuidance("Set magnetic field value");
  input_cmd_B->SetParameterName("B", false);
  input_cmd_B->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Set Dead Layer Thickness
  input_cmd_dl_thickness = new G4UIcmdWithAString("/Geometry/SiDeadLayer_Thickness", this);
  input_cmd_dl_thickness->SetGuidance("Set Silicon Dead Layer Thickness");
  input_cmd_dl_thickness->SetParameterName("dl_th", false);
  input_cmd_dl_thickness->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Set Catcher position z
  input_cmd_catcher_z = new G4UIcmdWithAString("/Geometry/Catcher_Position_z", this);
  input_cmd_catcher_z->SetGuidance("Set Catcher position");
  input_cmd_catcher_z->SetParameterName("catcher_z", false);
  input_cmd_catcher_z->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Set Catcher position theta
  input_cmd_catcher_theta = new G4UIcmdWithAString("/Geometry/Catcher_Position", this);
  input_cmd_catcher_theta->SetGuidance("Set Catcher position");
  input_cmd_catcher_theta->SetParameterName("catcher_z", false);
  input_cmd_catcher_theta->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Set Catcher Thickness
  input_cmd_catcher_thickness = new G4UIcmdWithAString("/Geometry/Catcher_Thickness", this);
  input_cmd_catcher_thickness->SetGuidance("Set Catcher thickness");
  input_cmd_catcher_thickness->SetParameterName("catcher_thickness", false);
  input_cmd_catcher_thickness->AvailableForStates(G4State_PreInit, G4State_Idle);
}

void Wisard_Messenger::SetNewValue(G4UIcommand *cmd, G4String args)
{

  // input file commands
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
  if (cmd == input_cmd_catcher_z)
  {
    G4double value;
    G4String unit;
    std::istringstream iss(args);
    iss >> value >> unit;
    
    gen_ptr->SetCatcherPosition_z(value * G4UnitDefinition::GetValueOf(unit));
    det_ptr->SetCatcherPosition_z(value * G4UnitDefinition::GetValueOf(unit));
  }
  if (cmd == input_cmd_catcher_theta)
  {
    G4double value1;
    G4String value, unit1;
    std::istringstream iss(args);
    iss >> value >> value1 >> unit1;

    det_ptr->SetCatcherPosition_theta(value, value1 * G4UnitDefinition::GetValueOf(unit1));
    
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
    G4double value, value1, value2;
    G4String unit;
    std::istringstream iss(args);
    iss >> value >> value1 >> value2 >> unit;
    
    if (value == 0.)
    {
      value = -1 / G4UnitDefinition::GetValueOf(unit);
    }
    if (value1 == 0.)
    {
      value1 = -1 / G4UnitDefinition::GetValueOf(unit);
    }
    if (value2 == 0.)
    {
      value2 = -1 / G4UnitDefinition::GetValueOf(unit);
    }
    det_ptr->SetCatcher_Thickness(value * G4UnitDefinition::GetValueOf(unit), value1 * G4UnitDefinition::GetValueOf(unit), value2 * G4UnitDefinition::GetValueOf(unit));
  }
  
}
