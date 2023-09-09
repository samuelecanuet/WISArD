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

#include <unordered_map>
#include <utility> // Pour std::pair

//----------------------------------------------------------------------
// This class defines the simulation core (from the base class G4RunManager)
// including information to get the required information
class Wisard_RunManager: public G4RunManager
{


  //------------------------------------------------------------
  // internal variables definition
  protected:

    Wisard_Sensor * wisard_sensor_PlasticScintillator;
    Wisard_Sensor * wisard_sensor_CatcherMylar;
    Wisard_Sensor * wisard_sensor_CatcherAl1;
    Wisard_Sensor * wisard_sensor_CatcherAl2;

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

   ifstream input;
   string   input_name;
   double   input_implantation;

   Wisard_Messenger * messenger;


  //------------------------------------------------------------
  // class functions definition
  public:


    // constructor and destructor
    Wisard_RunManager();
    ~Wisard_RunManager();

    static const int nb_det = 40;

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    G4String filename = analysisManager->GetFileName();

    TFile* f = new TFile(filename, "recreate");

    TTree* MyTree;
    TH1D* histos_coinc[nb_det];
    TH1D* histos_nocoinc[nb_det];

    std::string Detector_Name[nb_det] = {
    "1Up_Strip_1", "1Up_Strip_2", "1Up_Strip_3", "1Up_Strip_4", "1Up_Strip_5",
    "2Up_Strip_1", "2Up_Strip_2", "2Up_Strip_3", "2Up_Strip_4", "2Up_Strip_5",
    "3Up_Strip_1", "3Up_Strip_2", "3Up_Strip_3", "3Up_Strip_4", "3Up_Strip_5",
    "4Up_Strip_1", "4Up_Strip_2", "4Up_Strip_3", "4Up_Strip_4", "4Up_Strip_5",
    "1Down_Strip_1", "1Down_Strip_2", "1Down_Strip_3", "1Down_Strip_4", "1Down_Strip_5",
    "2Down_Strip_1", "2Down_Strip_2", "2Down_Strip_3", "2Down_Strip_4", "2Down_Strip_5",
    "3Down_Strip_1", "3Down_Strip_2", "3Down_Strip_3", "3Down_Strip_4", "3Down_Strip_5",
    "4Down_Strip_1", "4Down_Strip_2", "4Down_Strip_3", "4Down_Strip_4", "4Down_Strip_5"
    };

    std::unordered_map<std::string,  std::pair<Wisard_Sensor *, Wisard_Sensor *>> dic_detector;

  
    Wisard_Sensor * GetWisardSensor_PlasticScintillator();
    Wisard_Sensor * GetWisardSensor_Detector(string name);
    Wisard_Sensor * GetWisardSensor_CatcherMylar();
    Wisard_Sensor * GetWisardSensor_CatcherAl1();
    Wisard_Sensor * GetWisardSensor_CatcherAl2();

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

inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_Detector(string name)
{

  if (name.substr(name.length()-2) == "dl")
    {
      if (dic_detector.find(name.substr(0, name.length()-3)) != dic_detector.end())
      {
        return(dic_detector[name.substr(0,name.length()-3)].second);
      }
      else
      {
        G4Exception("GetWisard_Sensor_GetDetector", ("Detector Name not found : "+name).c_str(), JustWarning, "");
        return nullptr;
      }
    }

  else
    {
      if (dic_detector.find(name) != dic_detector.end())
      {
        return(dic_detector[name].first);
      }
      else
      {
        G4Exception("GetWisard_Sensor_GetDetector", ("Detector Name not found : "+name).c_str(), JustWarning, "");
        return nullptr;
      }
    }
  

}

inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_PlasticScintillator(){return(wisard_sensor_PlasticScintillator);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_CatcherMylar(){return(wisard_sensor_CatcherMylar);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_CatcherAl1(){return(wisard_sensor_CatcherAl1);}
inline Wisard_Sensor * Wisard_RunManager::GetWisardSensor_CatcherAl2(){return(wisard_sensor_CatcherAl2);}

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
