#ifndef WISARD_MESSENGER_HH
#define WISARD_MESSENGER_HH

#include "Wisard_Global.hh"

// Geant4 include files
#include "G4UImessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"

#include "Wisard_Detector.hh"
#include "Wisard_Generator.hh"



class Wisard_Messenger : public G4UImessenger
{
  //------------------------------------------------------------
  // internal variables definition
  Wisard_Detector *det_ptr;
  Wisard_Generator *gen_ptr; // pointer to the run manager
                             // so that the messenger can access
                             // detector and generator classes

  // user interface new commands and directories

  G4UIdirectory *input_dir;                // Commands directory for input file
  G4UIcmdWithAString *input_cmd_B;
  G4UIcmdWithAString *input_cmd_open_Field;
  G4UIcmdWithAString *input_cmd_dl_thickness;
  G4UIcmdWithAString *input_cmd_catcher_z;
  G4UIcmdWithAString *input_cmd_catcher_theta;
  G4UIcmdWithAString *input_cmd_filename;
  G4UIcmdWithAString *input_cmd_catcher_thickness;
  G4int Version;
  //------------------------------------------------------------
  // class functions definition
public:
  // constructor with arguments
  Wisard_Messenger(Wisard_Detector *det, Wisard_Generator *gen);

  // destructor
  virtual ~Wisard_Messenger();

  //------------------------------------------------------------
  // functions defining the commands
  //    set in different function for easier code reading
  void DefineInputCommands();

  //------------------------------------------------------------
  // this function is pure virtual in the base class: it must be defined
  // to process the commands
  void SetNewValue(G4UIcommand *cmd, G4String args);
};

#endif
