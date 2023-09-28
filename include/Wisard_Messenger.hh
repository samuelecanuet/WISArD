#ifndef WITCH_MESSENGER_HH
#define WITCH_MESSENGER_HH

#include "../Wisard_Global.hh"

class Wisard_RunManager;   // just indicates that this class exists


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


// This class defines new commands to be used in Geant4 interactive session
// 
//
//    /input/openCRADLEFile <file>
//    /input/closeCRADLEFile
//
//
//    /input/openSRIMFile <file>
//    /input/closeSRIMFile


class Wisard_Messenger : public G4UImessenger
{
  //------------------------------------------------------------
  // internal variables definition
    Wisard_RunManager  * manager_ptr;   // pointer to the run manager
                                    // so that the messenger can access
                                    // detector and generator classes

    // user interface new commands and directories

    G4UIdirectory           * input_dir;        // Commands directory for input file

    G4UIcmdWithAString        * input_cmd_open;       // Command to open an input file
    G4UIcmdWithoutParameter        * input_cmd_close;       // Command to open an input file

    G4UIcmdWithAString        * input_cmd_open_SRIM;       // Command to open an input file
    G4UIcmdWithoutParameter        * input_cmd_close_SRIM;       // Command to open an input file

  //------------------------------------------------------------
  // class functions definition
  public:

    // constructor with arguments
    Wisard_Messenger ( Wisard_RunManager * mgr );

    // destructor
    virtual ~Wisard_Messenger();

  //------------------------------------------------------------
  // functions defining the commands
  //    set in different function for easier code reading
    void DefineInputCommands     ( );

  //------------------------------------------------------------
  // this function is pure virtual in the base class: it must be defined
  // to process the commands
    void  SetNewValue ( G4UIcommand * cmd, G4String args );
};


#endif
