#ifndef WITCH_RUNMANAGER_HH
#define WITCH_RUNMANAGER_HH

#include "../Wisard_Global.hh"
#include "G4RunManager.hh"
#include "Wisard_Sensor.hh"
#include "Wisard_Messenger.hh"
#include "G4AnalysisManager.hh"

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"

//----------------------------------------------------------------------
// This class defines the simulation core (from the base class G4RunManager)
// including information to get the required information
class Wisard_RunManager: public G4RunManager
{


  //------------------------------------------------------------
  // internal variables definition
  protected:

    Wisard_Sensor * wisard_sensor_PlasticScintillator;

    Wisard_Sensor * wisard_sensor_1Up_Strip_1;
    Wisard_Sensor * wisard_sensor_1Up_Strip_2;
    Wisard_Sensor * wisard_sensor_1Up_Strip_3;
    Wisard_Sensor * wisard_sensor_1Up_Strip_4;
    Wisard_Sensor * wisard_sensor_1Up_Strip_5;
    Wisard_Sensor * wisard_sensor_1Up_Support;

    Wisard_Sensor * wisard_sensor_2Up_Strip_1;
    Wisard_Sensor * wisard_sensor_2Up_Strip_2;
    Wisard_Sensor * wisard_sensor_2Up_Strip_3;
    Wisard_Sensor * wisard_sensor_2Up_Strip_4;
    Wisard_Sensor * wisard_sensor_2Up_Strip_5;
    Wisard_Sensor * wisard_sensor_2Up_Support;

    Wisard_Sensor * wisard_sensor_3Up_Strip_1;
    Wisard_Sensor * wisard_sensor_3Up_Strip_2;
    Wisard_Sensor * wisard_sensor_3Up_Strip_3;
    Wisard_Sensor * wisard_sensor_3Up_Strip_4;
    Wisard_Sensor * wisard_sensor_3Up_Strip_5;
    Wisard_Sensor * wisard_sensor_3Up_Support;

    Wisard_Sensor * wisard_sensor_4Up_Strip_1;
    Wisard_Sensor * wisard_sensor_4Up_Strip_2;
    Wisard_Sensor * wisard_sensor_4Up_Strip_3;
    Wisard_Sensor * wisard_sensor_4Up_Strip_4;
    Wisard_Sensor * wisard_sensor_4Up_Strip_5;
    Wisard_Sensor * wisard_sensor_4Up_Support;

    Wisard_Sensor * wisard_sensor_1Down_Strip_1;
    Wisard_Sensor * wisard_sensor_1Down_Strip_2;
    Wisard_Sensor * wisard_sensor_1Down_Strip_3;
    Wisard_Sensor * wisard_sensor_1Down_Strip_4;
    Wisard_Sensor * wisard_sensor_1Down_Strip_5;
    Wisard_Sensor * wisard_sensor_1Down_Support;

    Wisard_Sensor * wisard_sensor_2Down_Strip_1;
    Wisard_Sensor * wisard_sensor_2Down_Strip_2;
    Wisard_Sensor * wisard_sensor_2Down_Strip_3;
    Wisard_Sensor * wisard_sensor_2Down_Strip_4;
    Wisard_Sensor * wisard_sensor_2Down_Strip_5;
    Wisard_Sensor * wisard_sensor_2Down_Support;

    Wisard_Sensor * wisard_sensor_3Down_Strip_1;
    Wisard_Sensor * wisard_sensor_3Down_Strip_2;
    Wisard_Sensor * wisard_sensor_3Down_Strip_3;
    Wisard_Sensor * wisard_sensor_3Down_Strip_4;
    Wisard_Sensor * wisard_sensor_3Down_Strip_5;
    Wisard_Sensor * wisard_sensor_3Down_Support;

    Wisard_Sensor * wisard_sensor_4Down_Strip_1;
    Wisard_Sensor * wisard_sensor_4Down_Strip_2;
    Wisard_Sensor * wisard_sensor_4Down_Strip_3;
    Wisard_Sensor * wisard_sensor_4Down_Strip_4;
    Wisard_Sensor * wisard_sensor_4Down_Strip_5;
    Wisard_Sensor * wisard_sensor_4Down_Support;

    Wisard_Sensor * wisard_sensor_CatcherMylar;
    // Wisard_Sensor * wisard_sensor_CatcherAl1;
    // Wisard_Sensor * wisard_sensor_CatcherAl2;

    Wisard_Sensor * wisard_sensor_1Up_Strip_1_dl;
    Wisard_Sensor * wisard_sensor_1Up_Strip_2_dl;
    Wisard_Sensor * wisard_sensor_1Up_Strip_3_dl;
    Wisard_Sensor * wisard_sensor_1Up_Strip_4_dl;
    Wisard_Sensor * wisard_sensor_1Up_Strip_5_dl;

    Wisard_Sensor * wisard_sensor_2Up_Strip_1_dl;
    Wisard_Sensor * wisard_sensor_2Up_Strip_2_dl;
    Wisard_Sensor * wisard_sensor_2Up_Strip_3_dl;
    Wisard_Sensor * wisard_sensor_2Up_Strip_4_dl;
    Wisard_Sensor * wisard_sensor_2Up_Strip_5_dl;

    Wisard_Sensor * wisard_sensor_3Up_Strip_1_dl;
    Wisard_Sensor * wisard_sensor_3Up_Strip_2_dl;
    Wisard_Sensor * wisard_sensor_3Up_Strip_3_dl;
    Wisard_Sensor * wisard_sensor_3Up_Strip_4_dl;
    Wisard_Sensor * wisard_sensor_3Up_Strip_5_dl;

    Wisard_Sensor * wisard_sensor_4Up_Strip_1_dl;
    Wisard_Sensor * wisard_sensor_4Up_Strip_2_dl;
    Wisard_Sensor * wisard_sensor_4Up_Strip_3_dl;
    Wisard_Sensor * wisard_sensor_4Up_Strip_4_dl;
    Wisard_Sensor * wisard_sensor_4Up_Strip_5_dl;

    Wisard_Sensor * wisard_sensor_1Down_Strip_1_dl;
    Wisard_Sensor * wisard_sensor_1Down_Strip_2_dl;
    Wisard_Sensor * wisard_sensor_1Down_Strip_3_dl;
    Wisard_Sensor * wisard_sensor_1Down_Strip_4_dl;
    Wisard_Sensor * wisard_sensor_1Down_Strip_5_dl;

    Wisard_Sensor * wisard_sensor_2Down_Strip_1_dl;
    Wisard_Sensor * wisard_sensor_2Down_Strip_2_dl;
    Wisard_Sensor * wisard_sensor_2Down_Strip_3_dl;
    Wisard_Sensor * wisard_sensor_2Down_Strip_4_dl;
    Wisard_Sensor * wisard_sensor_2Down_Strip_5_dl;

    Wisard_Sensor * wisard_sensor_3Down_Strip_1_dl;
    Wisard_Sensor * wisard_sensor_3Down_Strip_2_dl;
    Wisard_Sensor * wisard_sensor_3Down_Strip_3_dl;
    Wisard_Sensor * wisard_sensor_3Down_Strip_4_dl;
    Wisard_Sensor * wisard_sensor_3Down_Strip_5_dl;

    Wisard_Sensor * wisard_sensor_4Down_Strip_1_dl;
    Wisard_Sensor * wisard_sensor_4Down_Strip_2_dl;
    Wisard_Sensor * wisard_sensor_4Down_Strip_3_dl;
    Wisard_Sensor * wisard_sensor_4Down_Strip_4_dl;
    Wisard_Sensor * wisard_sensor_4Down_Strip_5_dl;

    // event counter
    int counts, wisard_counts;
    string sim_num;
    // std::vector<std::string> n_DetProton;
    string n_DetProton = "none";
    double e_DetProton=0;
    double e_Support=0;
    double e_DeadLayer=0;
    double e_Catcher = 0;
    // std::vector<double> e_DetProton;
    double x, y, z, e_proton, e_positron, r_proton, r_positron, p_px, p_py, p_pz, e_px, e_py, e_pz;
    double x_Det = -20;
    double y_Det = -20;
    double z_Det = -20;

    vector<double> x_vec;
    vector<double> y_vec;
    vector<double> z_vec;

    // this array stores the event's germanium energy

          ////--------------------------------------------------
          ////  JG 2013/07/08   - move with protected variable
          ////                    (not important)
          ////                  - added file name variable
   ifstream input;
   string   input_name;
   double   input_implantation;

          ////--------------------------------------------------
          ////  JG 2013/07/08   - adding user interface commands
  Wisard_Messenger * messenger;       // commands for the simulation
                                  // (there is no need that it is defined in
                                  //  the run manager, but here the run
                                  //  manager class connects all objects...)

  //------------------------------------------------------------
  // class functions definition
  public:


    // constructor and destructor
    Wisard_RunManager();
    ~Wisard_RunManager();

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    G4String filename = analysisManager->GetFileName();

    TFile* f = new TFile(filename, "recreate");

    TTree* MyTree;
    TH1D* histos_coinc[40];
    TH1D* histos_nocoinc[40];

    Wisard_Sensor * GetWisardSensor_PlasticScintillator();

    Wisard_Sensor * GetWisardSensor_1Up_Strip_1();
    Wisard_Sensor * GetWisardSensor_1Up_Strip_2();
    Wisard_Sensor * GetWisardSensor_1Up_Strip_3();
    Wisard_Sensor * GetWisardSensor_1Up_Strip_4();
    Wisard_Sensor * GetWisardSensor_1Up_Strip_5();
    Wisard_Sensor * GetWisardSensor_1Up_Support();

    Wisard_Sensor * GetWisardSensor_2Up_Strip_1();
    Wisard_Sensor * GetWisardSensor_2Up_Strip_2();
    Wisard_Sensor * GetWisardSensor_2Up_Strip_3();
    Wisard_Sensor * GetWisardSensor_2Up_Strip_4();
    Wisard_Sensor * GetWisardSensor_2Up_Strip_5();
    Wisard_Sensor * GetWisardSensor_2Up_Support();

    Wisard_Sensor * GetWisardSensor_3Up_Strip_1();
    Wisard_Sensor * GetWisardSensor_3Up_Strip_2();
    Wisard_Sensor * GetWisardSensor_3Up_Strip_3();
    Wisard_Sensor * GetWisardSensor_3Up_Strip_4();
    Wisard_Sensor * GetWisardSensor_3Up_Strip_5();
    Wisard_Sensor * GetWisardSensor_3Up_Support();

    Wisard_Sensor * GetWisardSensor_4Up_Strip_1();
    Wisard_Sensor * GetWisardSensor_4Up_Strip_2();
    Wisard_Sensor * GetWisardSensor_4Up_Strip_3();
    Wisard_Sensor * GetWisardSensor_4Up_Strip_4();
    Wisard_Sensor * GetWisardSensor_4Up_Strip_5();
    Wisard_Sensor * GetWisardSensor_4Up_Support();

    Wisard_Sensor * GetWisardSensor_1Down_Strip_1();
    Wisard_Sensor * GetWisardSensor_1Down_Strip_2();
    Wisard_Sensor * GetWisardSensor_1Down_Strip_3();
    Wisard_Sensor * GetWisardSensor_1Down_Strip_4();
    Wisard_Sensor * GetWisardSensor_1Down_Strip_5();
    Wisard_Sensor * GetWisardSensor_1Down_Support();

    Wisard_Sensor * GetWisardSensor_2Down_Strip_1();
    Wisard_Sensor * GetWisardSensor_2Down_Strip_2();
    Wisard_Sensor * GetWisardSensor_2Down_Strip_3();
    Wisard_Sensor * GetWisardSensor_2Down_Strip_4();
    Wisard_Sensor * GetWisardSensor_2Down_Strip_5();
    Wisard_Sensor * GetWisardSensor_2Down_Support();

    Wisard_Sensor * GetWisardSensor_3Down_Strip_1();
    Wisard_Sensor * GetWisardSensor_3Down_Strip_2();
    Wisard_Sensor * GetWisardSensor_3Down_Strip_3();
    Wisard_Sensor * GetWisardSensor_3Down_Strip_4();
    Wisard_Sensor * GetWisardSensor_3Down_Strip_5();
    Wisard_Sensor * GetWisardSensor_3Down_Support();

    Wisard_Sensor * GetWisardSensor_4Down_Strip_1();
    Wisard_Sensor * GetWisardSensor_4Down_Strip_2();
    Wisard_Sensor * GetWisardSensor_4Down_Strip_3();
    Wisard_Sensor * GetWisardSensor_4Down_Strip_4();
    Wisard_Sensor * GetWisardSensor_4Down_Strip_5();
    Wisard_Sensor * GetWisardSensor_4Down_Support();

    Wisard_Sensor * GetWisardSensor_CatcherMylar();
    // Wisard_Sensor * GetWisardSensor_CatcherAl1();
    // Wisard_Sensor * GetWisardSensor_CatcherAl2();

    Wisard_Sensor * GetWisardSensor_1Up_Strip_1_dl();
    Wisard_Sensor * GetWisardSensor_1Up_Strip_2_dl();
    Wisard_Sensor * GetWisardSensor_1Up_Strip_3_dl();
    Wisard_Sensor * GetWisardSensor_1Up_Strip_4_dl();
    Wisard_Sensor * GetWisardSensor_1Up_Strip_5_dl();

    Wisard_Sensor * GetWisardSensor_2Up_Strip_1_dl();
    Wisard_Sensor * GetWisardSensor_2Up_Strip_2_dl();
    Wisard_Sensor * GetWisardSensor_2Up_Strip_3_dl();
    Wisard_Sensor * GetWisardSensor_2Up_Strip_4_dl();
    Wisard_Sensor * GetWisardSensor_2Up_Strip_5_dl();

    Wisard_Sensor * GetWisardSensor_3Up_Strip_1_dl();
    Wisard_Sensor * GetWisardSensor_3Up_Strip_2_dl();
    Wisard_Sensor * GetWisardSensor_3Up_Strip_3_dl();
    Wisard_Sensor * GetWisardSensor_3Up_Strip_4_dl();
    Wisard_Sensor * GetWisardSensor_3Up_Strip_5_dl();

    Wisard_Sensor * GetWisardSensor_4Up_Strip_1_dl();
    Wisard_Sensor * GetWisardSensor_4Up_Strip_2_dl();
    Wisard_Sensor * GetWisardSensor_4Up_Strip_3_dl();
    Wisard_Sensor * GetWisardSensor_4Up_Strip_4_dl();
    Wisard_Sensor * GetWisardSensor_4Up_Strip_5_dl();

    Wisard_Sensor * GetWisardSensor_1Down_Strip_1_dl();
    Wisard_Sensor * GetWisardSensor_1Down_Strip_2_dl();
    Wisard_Sensor * GetWisardSensor_1Down_Strip_3_dl();
    Wisard_Sensor * GetWisardSensor_1Down_Strip_4_dl();
    Wisard_Sensor * GetWisardSensor_1Down_Strip_5_dl();

    Wisard_Sensor * GetWisardSensor_2Down_Strip_1_dl();
    Wisard_Sensor * GetWisardSensor_2Down_Strip_2_dl();
    Wisard_Sensor * GetWisardSensor_2Down_Strip_3_dl();
    Wisard_Sensor * GetWisardSensor_2Down_Strip_4_dl();
    Wisard_Sensor * GetWisardSensor_2Down_Strip_5_dl();

    Wisard_Sensor * GetWisardSensor_3Down_Strip_1_dl();
    Wisard_Sensor * GetWisardSensor_3Down_Strip_2_dl();
    Wisard_Sensor * GetWisardSensor_3Down_Strip_3_dl();
    Wisard_Sensor * GetWisardSensor_3Down_Strip_4_dl();
    Wisard_Sensor * GetWisardSensor_3Down_Strip_5_dl();

    Wisard_Sensor * GetWisardSensor_4Down_Strip_1_dl();
    Wisard_Sensor * GetWisardSensor_4Down_Strip_2_dl();
    Wisard_Sensor * GetWisardSensor_4Down_Strip_3_dl();
    Wisard_Sensor * GetWisardSensor_4Down_Strip_4_dl();
    Wisard_Sensor * GetWisardSensor_4Down_Strip_5_dl();

        //----------------------------------------------------------
    // Commands definitions
          ////--------------------------------------------------
          ////  JG 2013/07/08   U.I. commands creation function
    void  DefineSimulationCommands ( );     // inline


    //----------------------------------------------------------
    // Functions for input file
          ////--------------------------------------------------
          ////  JG 2013/07/08   added input file functions
          ////                  declaration
    int             OpenInput    ( const string & fname );
    void            CloseInput   ( );         // inline
    ifstream &      GetInput     ( );         // inline
    const string &  GetInputName ( ) const;   // inline
    const double &  GetInputImplantation ( ) const; //inline


    //----------------------------------------------------------
    // Functions for events processing and output histogram

    // redefine the function from base class G4RunManager
    void AnalyzeEvent ( G4Event * event );
};


//----------------------------------------------------------------------
// inline functions for the class

inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_PlasticScintillator(){return(wisard_sensor_PlasticScintillator);}

inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_1Up_Strip_1(){return(wisard_sensor_1Up_Strip_1);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_1Up_Strip_2(){return(wisard_sensor_1Up_Strip_2);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_1Up_Strip_3(){return(wisard_sensor_1Up_Strip_3);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_1Up_Strip_4(){return(wisard_sensor_1Up_Strip_4);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_1Up_Strip_5(){return(wisard_sensor_1Up_Strip_5);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_1Up_Support(){return(wisard_sensor_1Up_Support);}

inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_2Up_Strip_1(){return(wisard_sensor_2Up_Strip_1);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_2Up_Strip_2(){return(wisard_sensor_2Up_Strip_2);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_2Up_Strip_3(){return(wisard_sensor_2Up_Strip_3);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_2Up_Strip_4(){return(wisard_sensor_2Up_Strip_4);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_2Up_Strip_5(){return(wisard_sensor_2Up_Strip_5);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_2Up_Support(){return(wisard_sensor_2Up_Support);}

inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_3Up_Strip_1(){return(wisard_sensor_3Up_Strip_1);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_3Up_Strip_2(){return(wisard_sensor_3Up_Strip_2);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_3Up_Strip_3(){return(wisard_sensor_3Up_Strip_3);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_3Up_Strip_4(){return(wisard_sensor_3Up_Strip_4);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_3Up_Strip_5(){return(wisard_sensor_3Up_Strip_5);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_3Up_Support(){return(wisard_sensor_3Up_Support);}

inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_4Up_Strip_1(){return(wisard_sensor_4Up_Strip_1);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_4Up_Strip_2(){return(wisard_sensor_4Up_Strip_2);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_4Up_Strip_3(){return(wisard_sensor_4Up_Strip_3);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_4Up_Strip_4(){return(wisard_sensor_4Up_Strip_4);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_4Up_Strip_5(){return(wisard_sensor_4Up_Strip_5);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_4Up_Support(){return(wisard_sensor_4Up_Support);}

inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_1Down_Strip_1(){return(wisard_sensor_1Down_Strip_1);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_1Down_Strip_2(){return(wisard_sensor_1Down_Strip_2);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_1Down_Strip_3(){return(wisard_sensor_1Down_Strip_3);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_1Down_Strip_4(){return(wisard_sensor_1Down_Strip_4);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_1Down_Strip_5(){return(wisard_sensor_1Down_Strip_5);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_1Down_Support(){return(wisard_sensor_1Down_Support);}

inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_2Down_Strip_1(){return(wisard_sensor_2Down_Strip_1);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_2Down_Strip_2(){return(wisard_sensor_2Down_Strip_2);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_2Down_Strip_3(){return(wisard_sensor_2Down_Strip_3);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_2Down_Strip_4(){return(wisard_sensor_2Down_Strip_4);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_2Down_Strip_5(){return(wisard_sensor_2Down_Strip_5);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_2Down_Support(){return(wisard_sensor_2Down_Support);}

inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_3Down_Strip_1(){return(wisard_sensor_3Down_Strip_1);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_3Down_Strip_2(){return(wisard_sensor_3Down_Strip_2);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_3Down_Strip_3(){return(wisard_sensor_3Down_Strip_3);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_3Down_Strip_4(){return(wisard_sensor_3Down_Strip_4);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_3Down_Strip_5(){return(wisard_sensor_3Down_Strip_5);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_3Down_Support(){return(wisard_sensor_3Down_Support);}

inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_4Down_Strip_1(){return(wisard_sensor_4Down_Strip_1);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_4Down_Strip_2(){return(wisard_sensor_4Down_Strip_2);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_4Down_Strip_3(){return(wisard_sensor_4Down_Strip_3);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_4Down_Strip_4(){return(wisard_sensor_4Down_Strip_4);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_4Down_Strip_5(){return(wisard_sensor_4Down_Strip_5);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_4Down_Support(){return(wisard_sensor_4Down_Support);}
//
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_CatcherMylar(){return(wisard_sensor_CatcherMylar);}
// inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_CatcherAl1(){return(wisard_sensor_CatcherAl1);}
// inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_CatcherAl2(){return(wisard_sensor_CatcherAl2);}

inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_1Up_Strip_1_dl(){return(wisard_sensor_1Up_Strip_1_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_1Up_Strip_2_dl(){return(wisard_sensor_1Up_Strip_2_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_1Up_Strip_3_dl(){return(wisard_sensor_1Up_Strip_3_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_1Up_Strip_4_dl(){return(wisard_sensor_1Up_Strip_4_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_1Up_Strip_5_dl(){return(wisard_sensor_1Up_Strip_5_dl);}

inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_2Up_Strip_1_dl(){return(wisard_sensor_2Up_Strip_1_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_2Up_Strip_2_dl(){return(wisard_sensor_2Up_Strip_2_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_2Up_Strip_3_dl(){return(wisard_sensor_2Up_Strip_3_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_2Up_Strip_4_dl(){return(wisard_sensor_2Up_Strip_4_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_2Up_Strip_5_dl(){return(wisard_sensor_2Up_Strip_5_dl);}

inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_3Up_Strip_1_dl(){return(wisard_sensor_3Up_Strip_1_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_3Up_Strip_2_dl(){return(wisard_sensor_3Up_Strip_2_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_3Up_Strip_3_dl(){return(wisard_sensor_3Up_Strip_3_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_3Up_Strip_4_dl(){return(wisard_sensor_3Up_Strip_4_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_3Up_Strip_5_dl(){return(wisard_sensor_3Up_Strip_5_dl);}

inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_4Up_Strip_1_dl(){return(wisard_sensor_4Up_Strip_1_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_4Up_Strip_2_dl(){return(wisard_sensor_4Up_Strip_2_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_4Up_Strip_3_dl(){return(wisard_sensor_4Up_Strip_3_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_4Up_Strip_4_dl(){return(wisard_sensor_4Up_Strip_4_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_4Up_Strip_5_dl(){return(wisard_sensor_4Up_Strip_5_dl);}

inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_1Down_Strip_1_dl(){return(wisard_sensor_1Down_Strip_1_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_1Down_Strip_2_dl(){return(wisard_sensor_1Down_Strip_2_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_1Down_Strip_3_dl(){return(wisard_sensor_1Down_Strip_3_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_1Down_Strip_4_dl(){return(wisard_sensor_1Down_Strip_4_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_1Down_Strip_5_dl(){return(wisard_sensor_1Down_Strip_5_dl);}

inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_2Down_Strip_1_dl(){return(wisard_sensor_2Down_Strip_1_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_2Down_Strip_2_dl(){return(wisard_sensor_2Down_Strip_2_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_2Down_Strip_3_dl(){return(wisard_sensor_2Down_Strip_3_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_2Down_Strip_4_dl(){return(wisard_sensor_2Down_Strip_4_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_2Down_Strip_5_dl(){return(wisard_sensor_2Down_Strip_5_dl);}

inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_3Down_Strip_1_dl(){return(wisard_sensor_3Down_Strip_1_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_3Down_Strip_2_dl(){return(wisard_sensor_3Down_Strip_2_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_3Down_Strip_3_dl(){return(wisard_sensor_3Down_Strip_3_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_3Down_Strip_4_dl(){return(wisard_sensor_3Down_Strip_4_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_3Down_Strip_5_dl(){return(wisard_sensor_3Down_Strip_5_dl);}

inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_4Down_Strip_1_dl(){return(wisard_sensor_4Down_Strip_1_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_4Down_Strip_2_dl(){return(wisard_sensor_4Down_Strip_2_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_4Down_Strip_3_dl(){return(wisard_sensor_4Down_Strip_3_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_4Down_Strip_4_dl(){return(wisard_sensor_4Down_Strip_4_dl);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_4Down_Strip_5_dl(){return(wisard_sensor_4Down_Strip_5_dl);}

// Close the input file
inline void Wisard_RunManager::CloseInput ( )
  { input.close(); input_name = ""; }

// Get the input file stream information
inline ifstream & Wisard_RunManager::GetInput ( )
  { return ( input ); }

// Get the input file name
inline const string & Wisard_RunManager::GetInputName ( ) const
  { return ( input_name ); }

// create the commands for user interface
inline void Wisard_RunManager::DefineSimulationCommands ( )
  { messenger = new Wisard_Messenger ( this ); }

inline const double & Wisard_RunManager::GetInputImplantation ( ) const
  { return ( input_implantation); }

#endif
