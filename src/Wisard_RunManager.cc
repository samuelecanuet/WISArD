#include "Wisard_RunManager.hh"
#include "CLHEP/Random/RandGauss.h"
#include "G4AnalysisManager.hh"

#include <iostream>

#include "TBranch.h"
#include "TH1D.h"

#include "G4VTrajectoryPoint.hh"
#include "G4VTrajectory.hh"

#include "Wisard_Sensor.hh"

#include <numeric>


//----------------------------------------------------------------------

// constructor
Wisard_RunManager::Wisard_RunManager()
{
  cout << "Constructor Wisard_RunManager" << endl;

    // initialise the data
  counts = 0;

  wisard_sensor_PlasticScintillator = new Wisard_Sensor;

  wisard_sensor_1Up_Strip_1 = new Wisard_Sensor;
  wisard_sensor_1Up_Strip_2 = new Wisard_Sensor;
  wisard_sensor_1Up_Strip_3 = new Wisard_Sensor;
  wisard_sensor_1Up_Strip_4 = new Wisard_Sensor;
  wisard_sensor_1Up_Strip_5 = new Wisard_Sensor;
  wisard_sensor_1Up_Support = new Wisard_Sensor;

  wisard_sensor_2Up_Strip_1 = new Wisard_Sensor;
  wisard_sensor_2Up_Strip_2 = new Wisard_Sensor;
  wisard_sensor_2Up_Strip_3 = new Wisard_Sensor;
  wisard_sensor_2Up_Strip_4 = new Wisard_Sensor;
  wisard_sensor_2Up_Strip_5 = new Wisard_Sensor;
  wisard_sensor_2Up_Support = new Wisard_Sensor;

  wisard_sensor_3Up_Strip_1 = new Wisard_Sensor;
  wisard_sensor_3Up_Strip_2 = new Wisard_Sensor;
  wisard_sensor_3Up_Strip_3 = new Wisard_Sensor;
  wisard_sensor_3Up_Strip_4 = new Wisard_Sensor;
  wisard_sensor_3Up_Strip_5 = new Wisard_Sensor;
  wisard_sensor_3Up_Support = new Wisard_Sensor;

  wisard_sensor_4Up_Strip_1 = new Wisard_Sensor;
  wisard_sensor_4Up_Strip_2 = new Wisard_Sensor;
  wisard_sensor_4Up_Strip_3 = new Wisard_Sensor;
  wisard_sensor_4Up_Strip_4 = new Wisard_Sensor;
  wisard_sensor_4Up_Strip_5 = new Wisard_Sensor;
  wisard_sensor_4Up_Support = new Wisard_Sensor;

  wisard_sensor_1Down_Strip_1 = new Wisard_Sensor;
  wisard_sensor_1Down_Strip_2 = new Wisard_Sensor;
  wisard_sensor_1Down_Strip_3 = new Wisard_Sensor;
  wisard_sensor_1Down_Strip_4 = new Wisard_Sensor;
  wisard_sensor_1Down_Strip_5 = new Wisard_Sensor;
  wisard_sensor_1Down_Support = new Wisard_Sensor;

  wisard_sensor_2Down_Strip_1 = new Wisard_Sensor;
  wisard_sensor_2Down_Strip_2 = new Wisard_Sensor;
  wisard_sensor_2Down_Strip_3 = new Wisard_Sensor;
  wisard_sensor_2Down_Strip_4 = new Wisard_Sensor;
  wisard_sensor_2Down_Strip_5 = new Wisard_Sensor;
  wisard_sensor_2Down_Support = new Wisard_Sensor;

  wisard_sensor_3Down_Strip_1 = new Wisard_Sensor;
  wisard_sensor_3Down_Strip_2 = new Wisard_Sensor;
  wisard_sensor_3Down_Strip_3 = new Wisard_Sensor;
  wisard_sensor_3Down_Strip_4 = new Wisard_Sensor;
  wisard_sensor_3Down_Strip_5 = new Wisard_Sensor;
  wisard_sensor_3Down_Support = new Wisard_Sensor;

  wisard_sensor_4Down_Strip_1 = new Wisard_Sensor;
  wisard_sensor_4Down_Strip_2 = new Wisard_Sensor;
  wisard_sensor_4Down_Strip_3 = new Wisard_Sensor;
  wisard_sensor_4Down_Strip_4 = new Wisard_Sensor;
  wisard_sensor_4Down_Strip_5 = new Wisard_Sensor;
  wisard_sensor_4Down_Support = new Wisard_Sensor;

  wisard_sensor_CatcherMylar = new Wisard_Sensor;
  // wisard_sensor_CatcherAl1 = new Wisard_Sensor;
  // wisard_sensor_CatcherAl2 = new Wisard_Sensor;

  wisard_sensor_1Up_Strip_1_dl = new Wisard_Sensor;
  wisard_sensor_1Up_Strip_2_dl = new Wisard_Sensor;
  wisard_sensor_1Up_Strip_3_dl = new Wisard_Sensor;
  wisard_sensor_1Up_Strip_4_dl = new Wisard_Sensor;
  wisard_sensor_1Up_Strip_5_dl = new Wisard_Sensor;

  wisard_sensor_2Up_Strip_1_dl = new Wisard_Sensor;
  wisard_sensor_2Up_Strip_2_dl = new Wisard_Sensor;
  wisard_sensor_2Up_Strip_3_dl = new Wisard_Sensor;
  wisard_sensor_2Up_Strip_4_dl = new Wisard_Sensor;
  wisard_sensor_2Up_Strip_5_dl = new Wisard_Sensor;

  wisard_sensor_3Up_Strip_1_dl = new Wisard_Sensor;
  wisard_sensor_3Up_Strip_2_dl = new Wisard_Sensor;
  wisard_sensor_3Up_Strip_3_dl = new Wisard_Sensor;
  wisard_sensor_3Up_Strip_4_dl = new Wisard_Sensor;
  wisard_sensor_3Up_Strip_5_dl = new Wisard_Sensor;

  wisard_sensor_4Up_Strip_1_dl = new Wisard_Sensor;
  wisard_sensor_4Up_Strip_2_dl = new Wisard_Sensor;
  wisard_sensor_4Up_Strip_3_dl = new Wisard_Sensor;
  wisard_sensor_4Up_Strip_4_dl = new Wisard_Sensor;
  wisard_sensor_4Up_Strip_5_dl = new Wisard_Sensor;

  wisard_sensor_1Down_Strip_1_dl = new Wisard_Sensor;
  wisard_sensor_1Down_Strip_2_dl = new Wisard_Sensor;
  wisard_sensor_1Down_Strip_3_dl = new Wisard_Sensor;
  wisard_sensor_1Down_Strip_4_dl = new Wisard_Sensor;
  wisard_sensor_1Down_Strip_5_dl = new Wisard_Sensor;

  wisard_sensor_2Down_Strip_1_dl = new Wisard_Sensor;
  wisard_sensor_2Down_Strip_2_dl = new Wisard_Sensor;
  wisard_sensor_2Down_Strip_3_dl = new Wisard_Sensor;
  wisard_sensor_2Down_Strip_4_dl = new Wisard_Sensor;
  wisard_sensor_2Down_Strip_5_dl = new Wisard_Sensor;

  wisard_sensor_3Down_Strip_1_dl = new Wisard_Sensor;
  wisard_sensor_3Down_Strip_2_dl = new Wisard_Sensor;
  wisard_sensor_3Down_Strip_3_dl = new Wisard_Sensor;
  wisard_sensor_3Down_Strip_4_dl = new Wisard_Sensor;
  wisard_sensor_3Down_Strip_5_dl = new Wisard_Sensor;

  wisard_sensor_4Down_Strip_1_dl = new Wisard_Sensor;
  wisard_sensor_4Down_Strip_2_dl = new Wisard_Sensor;
  wisard_sensor_4Down_Strip_3_dl = new Wisard_Sensor;
  wisard_sensor_4Down_Strip_4_dl = new Wisard_Sensor;
  wisard_sensor_4Down_Strip_5_dl = new Wisard_Sensor;


}

// destructor
Wisard_RunManager::~Wisard_RunManager()
{
  cout << "Destructor Wisard_RunManager" << endl;

  f->Close();
  delete f;

  delete wisard_sensor_PlasticScintillator;

  delete wisard_sensor_1Up_Strip_1;
  delete wisard_sensor_1Up_Strip_2;
  delete wisard_sensor_1Up_Strip_3;
  delete wisard_sensor_1Up_Strip_4;
  delete wisard_sensor_1Up_Strip_5;
  delete wisard_sensor_1Up_Support;

  delete wisard_sensor_2Up_Strip_1;
  delete wisard_sensor_2Up_Strip_2;
  delete wisard_sensor_2Up_Strip_3;
  delete wisard_sensor_2Up_Strip_4;
  delete wisard_sensor_2Up_Strip_5;
  delete wisard_sensor_2Up_Support;

  delete wisard_sensor_3Up_Strip_1;
  delete wisard_sensor_3Up_Strip_2;
  delete wisard_sensor_3Up_Strip_3;
  delete wisard_sensor_3Up_Strip_4;
  delete wisard_sensor_3Up_Strip_5;
  delete wisard_sensor_3Up_Support;

  delete wisard_sensor_4Up_Strip_1;
  delete wisard_sensor_4Up_Strip_2;
  delete wisard_sensor_4Up_Strip_3;
  delete wisard_sensor_4Up_Strip_4;
  delete wisard_sensor_4Up_Strip_5;
  delete wisard_sensor_4Up_Support;

  delete wisard_sensor_1Down_Strip_1;
  delete wisard_sensor_1Down_Strip_2;
  delete wisard_sensor_1Down_Strip_3;
  delete wisard_sensor_1Down_Strip_4;
  delete wisard_sensor_1Down_Strip_5;
  delete wisard_sensor_1Down_Support;

  delete wisard_sensor_2Down_Strip_1;
  delete wisard_sensor_2Down_Strip_2;
  delete wisard_sensor_2Down_Strip_3;
  delete wisard_sensor_2Down_Strip_4;
  delete wisard_sensor_2Down_Strip_5;
  delete wisard_sensor_2Down_Support;

  delete wisard_sensor_3Down_Strip_1;
  delete wisard_sensor_3Down_Strip_2;
  delete wisard_sensor_3Down_Strip_3;
  delete wisard_sensor_3Down_Strip_4;
  delete wisard_sensor_3Down_Strip_5;
  delete wisard_sensor_3Down_Support;

  delete wisard_sensor_4Down_Strip_1;
  delete wisard_sensor_4Down_Strip_2;
  delete wisard_sensor_4Down_Strip_3;
  delete wisard_sensor_4Down_Strip_4;
  delete wisard_sensor_4Down_Strip_5;
  delete wisard_sensor_4Down_Support;

  delete wisard_sensor_CatcherMylar;
  // delete wisard_sensor_CatcherAl1;
  // delete wisard_sensor_CatcherAl2;

  delete wisard_sensor_1Up_Strip_1_dl;
  delete wisard_sensor_1Up_Strip_2_dl;
  delete wisard_sensor_1Up_Strip_3_dl;
  delete wisard_sensor_1Up_Strip_4_dl;
  delete wisard_sensor_1Up_Strip_5_dl;

  delete wisard_sensor_2Up_Strip_1_dl;
  delete wisard_sensor_2Up_Strip_2_dl;
  delete wisard_sensor_2Up_Strip_3_dl;
  delete wisard_sensor_2Up_Strip_4_dl;
  delete wisard_sensor_2Up_Strip_5_dl;

  delete wisard_sensor_3Up_Strip_1_dl;
  delete wisard_sensor_3Up_Strip_2_dl;
  delete wisard_sensor_3Up_Strip_3_dl;
  delete wisard_sensor_3Up_Strip_4_dl;
  delete wisard_sensor_3Up_Strip_5_dl;

  delete wisard_sensor_4Up_Strip_1_dl;
  delete wisard_sensor_4Up_Strip_2_dl;
  delete wisard_sensor_4Up_Strip_3_dl;
  delete wisard_sensor_4Up_Strip_4_dl;
  delete wisard_sensor_4Up_Strip_5_dl;

  delete wisard_sensor_1Down_Strip_1_dl;
  delete wisard_sensor_1Down_Strip_2_dl;
  delete wisard_sensor_1Down_Strip_3_dl;
  delete wisard_sensor_1Down_Strip_4_dl;
  delete wisard_sensor_1Down_Strip_5_dl;

  delete wisard_sensor_2Down_Strip_1_dl;
  delete wisard_sensor_2Down_Strip_2_dl;
  delete wisard_sensor_2Down_Strip_3_dl;
  delete wisard_sensor_2Down_Strip_4_dl;
  delete wisard_sensor_2Down_Strip_5_dl;

  delete wisard_sensor_3Down_Strip_1_dl;
  delete wisard_sensor_3Down_Strip_2_dl;
  delete wisard_sensor_3Down_Strip_3_dl;
  delete wisard_sensor_3Down_Strip_4_dl;
  delete wisard_sensor_3Down_Strip_5_dl;

  delete wisard_sensor_4Down_Strip_1_dl;
  delete wisard_sensor_4Down_Strip_2_dl;
  delete wisard_sensor_4Down_Strip_3_dl;
  delete wisard_sensor_4Down_Strip_4_dl;
  delete wisard_sensor_4Down_Strip_5_dl;

  delete messenger;

  CloseInput();
}

// The function returns 0 if no error occured
int Wisard_RunManager::OpenInput ( const string & fname )
{
  int error = 0;    // return value

  if (fname != "")
  {
    // close previous output... just in case
    CloseInput();

    // try to open the new file
    input.open ( fname.c_str() );

    if (input.fail())
    {
      error = 2;
      cerr << "<W> OpenInput : error opening file " << fname << endl;
    }
    else
    {
      input_name = fname;
      cout << "<I> Open input file: " << fname << endl;
    }

  }
  else
  {
    error = 1;
    cerr << "<W> OpenInput : empty file name" << endl;
  }

  return (error);
}

//----------------------------------------------------------------------
// This function "overloads" (replaces) the G4RunManager function
void Wisard_RunManager::AnalyzeEvent ( G4Event * event )
{

  // call the base class function (whatever it is supposed to do)
  G4RunManager::AnalyzeEvent ( event );
  double e_PlasticScintillator = wisard_sensor_PlasticScintillator->GetEventEnergy_positron()/keV;

  double e_1Up_Strip_1 = wisard_sensor_1Up_Strip_1->GetEventEnergy()/keV;
  double e_1Up_Strip_2 = wisard_sensor_1Up_Strip_2->GetEventEnergy()/keV;
  double e_1Up_Strip_3 = wisard_sensor_1Up_Strip_3->GetEventEnergy()/keV;
  double e_1Up_Strip_4 = wisard_sensor_1Up_Strip_4->GetEventEnergy()/keV;
  double e_1Up_Strip_5 = wisard_sensor_1Up_Strip_5->GetEventEnergy()/keV;
  double e_1Up_Support = wisard_sensor_1Up_Support->GetEventEnergy()/keV;

  double e_2Up_Strip_1 = wisard_sensor_2Up_Strip_1->GetEventEnergy()/keV;
  double e_2Up_Strip_2 = wisard_sensor_2Up_Strip_2->GetEventEnergy()/keV;
  double e_2Up_Strip_3 = wisard_sensor_2Up_Strip_3->GetEventEnergy()/keV;
  double e_2Up_Strip_4 = wisard_sensor_2Up_Strip_4->GetEventEnergy()/keV;
  double e_2Up_Strip_5 = wisard_sensor_2Up_Strip_5->GetEventEnergy()/keV;
  double e_2Up_Support = wisard_sensor_2Up_Support->GetEventEnergy()/keV;

  double e_3Up_Strip_1 = wisard_sensor_3Up_Strip_1->GetEventEnergy()/keV;
  double e_3Up_Strip_2 = wisard_sensor_3Up_Strip_2->GetEventEnergy()/keV;
  double e_3Up_Strip_3 = wisard_sensor_3Up_Strip_3->GetEventEnergy()/keV;
  double e_3Up_Strip_4 = wisard_sensor_3Up_Strip_4->GetEventEnergy()/keV;
  double e_3Up_Strip_5 = wisard_sensor_3Up_Strip_5->GetEventEnergy()/keV;
  double e_3Up_Support = wisard_sensor_3Up_Support->GetEventEnergy()/keV;

  double e_4Up_Strip_1 = wisard_sensor_4Up_Strip_1->GetEventEnergy()/keV;
  double e_4Up_Strip_2 = wisard_sensor_4Up_Strip_2->GetEventEnergy()/keV;
  double e_4Up_Strip_3 = wisard_sensor_4Up_Strip_3->GetEventEnergy()/keV;
  double e_4Up_Strip_4 = wisard_sensor_4Up_Strip_4->GetEventEnergy()/keV;
  double e_4Up_Strip_5 = wisard_sensor_4Up_Strip_5->GetEventEnergy()/keV;
  double e_4Up_Support = wisard_sensor_4Up_Support->GetEventEnergy()/keV;

  double e_1Down_Strip_1 = wisard_sensor_1Down_Strip_1->GetEventEnergy()/keV;
  double e_1Down_Strip_2 = wisard_sensor_1Down_Strip_2->GetEventEnergy()/keV;
  double e_1Down_Strip_3 = wisard_sensor_1Down_Strip_3->GetEventEnergy()/keV;
  double e_1Down_Strip_4 = wisard_sensor_1Down_Strip_4->GetEventEnergy()/keV;
  double e_1Down_Strip_5 = wisard_sensor_1Down_Strip_5->GetEventEnergy()/keV;
  double e_1Down_Support = wisard_sensor_1Down_Support->GetEventEnergy()/keV;

  double e_2Down_Strip_1 = wisard_sensor_2Down_Strip_1->GetEventEnergy()/keV;
  double e_2Down_Strip_2 = wisard_sensor_2Down_Strip_2->GetEventEnergy()/keV;
  double e_2Down_Strip_3 = wisard_sensor_2Down_Strip_3->GetEventEnergy()/keV;
  double e_2Down_Strip_4 = wisard_sensor_2Down_Strip_4->GetEventEnergy()/keV;
  double e_2Down_Strip_5 = wisard_sensor_2Down_Strip_5->GetEventEnergy()/keV;
  double e_2Down_Support = wisard_sensor_2Down_Support->GetEventEnergy()/keV;

  double e_3Down_Strip_1 = wisard_sensor_3Down_Strip_1->GetEventEnergy()/keV;
  double e_3Down_Strip_2 = wisard_sensor_3Down_Strip_2->GetEventEnergy()/keV;
  double e_3Down_Strip_3 = wisard_sensor_3Down_Strip_3->GetEventEnergy()/keV;
  double e_3Down_Strip_4 = wisard_sensor_3Down_Strip_4->GetEventEnergy()/keV;
  double e_3Down_Strip_5 = wisard_sensor_3Down_Strip_5->GetEventEnergy()/keV;
  double e_3Down_Support = wisard_sensor_3Down_Support->GetEventEnergy()/keV;

  double e_4Down_Strip_1 = wisard_sensor_4Down_Strip_1->GetEventEnergy()/keV;
  double e_4Down_Strip_2 = wisard_sensor_4Down_Strip_2->GetEventEnergy()/keV;
  double e_4Down_Strip_3 = wisard_sensor_4Down_Strip_3->GetEventEnergy()/keV;
  double e_4Down_Strip_4 = wisard_sensor_4Down_Strip_4->GetEventEnergy()/keV;
  double e_4Down_Strip_5 = wisard_sensor_4Down_Strip_5->GetEventEnergy()/keV;
  double e_4Down_Support = wisard_sensor_4Down_Support->GetEventEnergy()/keV;

  double e_CatcherMylar = wisard_sensor_CatcherMylar->GetEventEnergy()/keV;
  // double e_CatcherAl1 = wisard_sensor_CatcherAl1->GetEventEnergy()/keV;
  // double e_CatcherAl2 = wisard_sensor_CatcherAl2->GetEventEnergy()/keV;

  double e_1Up_Strip_1_dl = wisard_sensor_1Up_Strip_1_dl->GetEventEnergy()/keV;
  double e_1Up_Strip_2_dl = wisard_sensor_1Up_Strip_2_dl->GetEventEnergy()/keV;
  double e_1Up_Strip_3_dl = wisard_sensor_1Up_Strip_3_dl->GetEventEnergy()/keV;
  double e_1Up_Strip_4_dl = wisard_sensor_1Up_Strip_4_dl->GetEventEnergy()/keV;
  double e_1Up_Strip_5_dl = wisard_sensor_1Up_Strip_5_dl->GetEventEnergy()/keV;

  double e_2Up_Strip_1_dl = wisard_sensor_2Up_Strip_1_dl->GetEventEnergy()/keV;
  double e_2Up_Strip_2_dl = wisard_sensor_2Up_Strip_2_dl->GetEventEnergy()/keV;
  double e_2Up_Strip_3_dl = wisard_sensor_2Up_Strip_3_dl->GetEventEnergy()/keV;
  double e_2Up_Strip_4_dl = wisard_sensor_2Up_Strip_4_dl->GetEventEnergy()/keV;
  double e_2Up_Strip_5_dl = wisard_sensor_2Up_Strip_5_dl->GetEventEnergy()/keV;

  double e_3Up_Strip_1_dl = wisard_sensor_3Up_Strip_1_dl->GetEventEnergy()/keV;
  double e_3Up_Strip_2_dl = wisard_sensor_3Up_Strip_2_dl->GetEventEnergy()/keV;
  double e_3Up_Strip_3_dl = wisard_sensor_3Up_Strip_3_dl->GetEventEnergy()/keV;
  double e_3Up_Strip_4_dl = wisard_sensor_3Up_Strip_4_dl->GetEventEnergy()/keV;
  double e_3Up_Strip_5_dl = wisard_sensor_3Up_Strip_5_dl->GetEventEnergy()/keV;

  double e_4Up_Strip_1_dl = wisard_sensor_4Up_Strip_1_dl->GetEventEnergy()/keV;
  double e_4Up_Strip_2_dl = wisard_sensor_4Up_Strip_2_dl->GetEventEnergy()/keV;
  double e_4Up_Strip_3_dl = wisard_sensor_4Up_Strip_3_dl->GetEventEnergy()/keV;
  double e_4Up_Strip_4_dl = wisard_sensor_4Up_Strip_4_dl->GetEventEnergy()/keV;
  double e_4Up_Strip_5_dl = wisard_sensor_4Up_Strip_5_dl->GetEventEnergy()/keV;

  double e_1Down_Strip_1_dl = wisard_sensor_1Down_Strip_1_dl->GetEventEnergy()/keV;
  double e_1Down_Strip_2_dl = wisard_sensor_1Down_Strip_2_dl->GetEventEnergy()/keV;
  double e_1Down_Strip_3_dl = wisard_sensor_1Down_Strip_3_dl->GetEventEnergy()/keV;
  double e_1Down_Strip_4_dl = wisard_sensor_1Down_Strip_4_dl->GetEventEnergy()/keV;
  double e_1Down_Strip_5_dl = wisard_sensor_1Down_Strip_5_dl->GetEventEnergy()/keV;

  double e_2Down_Strip_1_dl = wisard_sensor_2Down_Strip_1_dl->GetEventEnergy()/keV;
  double e_2Down_Strip_2_dl = wisard_sensor_2Down_Strip_2_dl->GetEventEnergy()/keV;
  double e_2Down_Strip_3_dl = wisard_sensor_2Down_Strip_3_dl->GetEventEnergy()/keV;
  double e_2Down_Strip_4_dl = wisard_sensor_2Down_Strip_4_dl->GetEventEnergy()/keV;
  double e_2Down_Strip_5_dl = wisard_sensor_2Down_Strip_5_dl->GetEventEnergy()/keV;

  double e_3Down_Strip_1_dl = wisard_sensor_3Down_Strip_1_dl->GetEventEnergy()/keV;
  double e_3Down_Strip_2_dl = wisard_sensor_3Down_Strip_2_dl->GetEventEnergy()/keV;
  double e_3Down_Strip_3_dl = wisard_sensor_3Down_Strip_3_dl->GetEventEnergy()/keV;
  double e_3Down_Strip_4_dl = wisard_sensor_3Down_Strip_4_dl->GetEventEnergy()/keV;
  double e_3Down_Strip_5_dl = wisard_sensor_3Down_Strip_5_dl->GetEventEnergy()/keV;

  double e_4Down_Strip_1_dl = wisard_sensor_4Down_Strip_1_dl->GetEventEnergy()/keV;
  double e_4Down_Strip_2_dl = wisard_sensor_4Down_Strip_2_dl->GetEventEnergy()/keV;
  double e_4Down_Strip_3_dl = wisard_sensor_4Down_Strip_3_dl->GetEventEnergy()/keV;
  double e_4Down_Strip_4_dl = wisard_sensor_4Down_Strip_4_dl->GetEventEnergy()/keV;
  double e_4Down_Strip_5_dl = wisard_sensor_4Down_Strip_5_dl->GetEventEnergy()/keV;

///////////////////////////////////////////////////////////////////////////////////////////
  // G4ThreeVector p_1Up_Strip_1 = wisard_sensor_1Up_Strip_1->GetPositionEnter();
  // G4ThreeVector p_1Up_Strip_2 = wisard_sensor_1Up_Strip_2->GetPositionEnter();
  // G4ThreeVector p_1Up_Strip_3 = wisard_sensor_1Up_Strip_3->GetPositionEnter();
  // G4ThreeVector p_1Up_Strip_4 = wisard_sensor_1Up_Strip_4->GetPositionEnter();
  // G4ThreeVector p_1Up_Strip_5 = wisard_sensor_1Up_Strip_5->GetPositionEnter();
  // G4ThreeVector p_1Up_Support = wisard_sensor_1Up_Support->GetPositionEnter();
  //
  // G4ThreeVector p_2Up_Strip_1 = wisard_sensor_2Up_Strip_1->GetPositionEnter();
  // G4ThreeVector p_2Up_Strip_2 = wisard_sensor_2Up_Strip_2->GetPositionEnter();
  // G4ThreeVector p_2Up_Strip_3 = wisard_sensor_2Up_Strip_3->GetPositionEnter();
  // G4ThreeVector p_2Up_Strip_4 = wisard_sensor_2Up_Strip_4->GetPositionEnter();
  // G4ThreeVector p_2Up_Strip_5 = wisard_sensor_2Up_Strip_5->GetPositionEnter();
  // G4ThreeVector p_2Up_Support = wisard_sensor_2Up_Support->GetPositionEnter();
  //
  // G4ThreeVector p_3Up_Strip_1 = wisard_sensor_3Up_Strip_1->GetPositionEnter();
  // G4ThreeVector p_3Up_Strip_2 = wisard_sensor_3Up_Strip_2->GetPositionEnter();
  // G4ThreeVector p_3Up_Strip_3 = wisard_sensor_3Up_Strip_3->GetPositionEnter();
  // G4ThreeVector p_3Up_Strip_4 = wisard_sensor_3Up_Strip_4->GetPositionEnter();
  // G4ThreeVector p_3Up_Strip_5 = wisard_sensor_3Up_Strip_5->GetPositionEnter();
  // G4ThreeVector p_3Up_Support = wisard_sensor_3Up_Support->GetPositionEnter();
  //
  // G4ThreeVector p_4Up_Strip_1 = wisard_sensor_4Up_Strip_1->GetPositionEnter();
  // G4ThreeVector p_4Up_Strip_2 = wisard_sensor_4Up_Strip_2->GetPositionEnter();
  // G4ThreeVector p_4Up_Strip_3 = wisard_sensor_4Up_Strip_3->GetPositionEnter();
  // G4ThreeVector p_4Up_Strip_4 = wisard_sensor_4Up_Strip_4->GetPositionEnter();
  // G4ThreeVector p_4Up_Strip_5 = wisard_sensor_4Up_Strip_5->GetPositionEnter();
  // G4ThreeVector p_4Up_Support = wisard_sensor_4Up_Support->GetPositionEnter();
  //
  // G4ThreeVector p_1Down_Strip_1 = wisard_sensor_1Down_Strip_1->GetPositionEnter();
  // G4ThreeVector p_1Down_Strip_2 = wisard_sensor_1Down_Strip_2->GetPositionEnter();
  // G4ThreeVector p_1Down_Strip_3 = wisard_sensor_1Down_Strip_3->GetPositionEnter();
  // G4ThreeVector p_1Down_Strip_4 = wisard_sensor_1Down_Strip_4->GetPositionEnter();
  // G4ThreeVector p_1Down_Strip_5 = wisard_sensor_1Down_Strip_5->GetPositionEnter();
  // G4ThreeVector p_1DOwn_Support = wisard_sensor_1Down_Support->GetPositionEnter();
  //
  // G4ThreeVector p_2Down_Strip_1 = wisard_sensor_2Down_Strip_1->GetPositionEnter();
  // G4ThreeVector p_2Down_Strip_2 = wisard_sensor_2Down_Strip_2->GetPositionEnter();
  // G4ThreeVector p_2Down_Strip_3 = wisard_sensor_2Down_Strip_3->GetPositionEnter();
  // G4ThreeVector p_2Down_Strip_4 = wisard_sensor_2Down_Strip_4->GetPositionEnter();
  // G4ThreeVector p_2Down_Strip_5 = wisard_sensor_2Down_Strip_5->GetPositionEnter();
  // G4ThreeVector p_2Down_Support = wisard_sensor_2Down_Support->GetPositionEnter();
  //
  // G4ThreeVector p_3Down_Strip_1 = wisard_sensor_3Down_Strip_1->GetPositionEnter();
  // G4ThreeVector p_3Down_Strip_2 = wisard_sensor_3Down_Strip_2->GetPositionEnter();
  // G4ThreeVector p_3Down_Strip_3 = wisard_sensor_3Down_Strip_3->GetPositionEnter();
  // G4ThreeVector p_3Down_Strip_4 = wisard_sensor_3Down_Strip_4->GetPositionEnter();
  // G4ThreeVector p_3Down_Strip_5 = wisard_sensor_3Down_Strip_5->GetPositionEnter();
  // G4ThreeVector p_3Down_Support = wisard_sensor_3Down_Support->GetPositionEnter();
  //
  // G4ThreeVector p_4Down_Strip_1 = wisard_sensor_4Down_Strip_1->GetPositionEnter();
  // G4ThreeVector p_4Down_Strip_2 = wisard_sensor_4Down_Strip_2->GetPositionEnter();
  // G4ThreeVector p_4Down_Strip_3 = wisard_sensor_4Down_Strip_3->GetPositionEnter();
  // G4ThreeVector p_4Down_Strip_4 = wisard_sensor_4Down_Strip_4->GetPositionEnter();
  // G4ThreeVector p_4Down_Strip_5 = wisard_sensor_4Down_Strip_5->GetPositionEnter();
  // G4ThreeVector p_4Down_Support = wisard_sensor_4Down_Support->GetPositionEnter();

  double value_det[40] = {
    e_1Up_Strip_1, e_1Up_Strip_2, e_1Up_Strip_3, e_1Up_Strip_4, e_1Up_Strip_5,
    e_2Up_Strip_1, e_2Up_Strip_2, e_2Up_Strip_3, e_2Up_Strip_4, e_2Up_Strip_5,
    e_3Up_Strip_1, e_3Up_Strip_2, e_3Up_Strip_3, e_3Up_Strip_4, e_3Up_Strip_5,
    e_4Up_Strip_1, e_4Up_Strip_2, e_4Up_Strip_3, e_4Up_Strip_4, e_4Up_Strip_5,
    e_1Down_Strip_1, e_1Down_Strip_2, e_1Down_Strip_3, e_1Down_Strip_4, e_1Down_Strip_5,
    e_2Down_Strip_1, e_2Down_Strip_2, e_2Down_Strip_3, e_2Down_Strip_4, e_2Down_Strip_5,
    e_3Down_Strip_1, e_3Down_Strip_2, e_3Down_Strip_3, e_3Down_Strip_4, e_3Down_Strip_5,
    e_4Down_Strip_1, e_4Down_Strip_2, e_4Down_Strip_3, e_4Down_Strip_4, e_4Down_Strip_5
    };

  std::string name_det[40] = {
    "1Up_Strip_1", "1Up_Strip_2", "1Up_Strip_3", "1Up_Strip_4", "1Up_Strip_5",
    "2Up_Strip_1", "2Up_Strip_2", "2Up_Strip_3", "2Up_Strip_4", "2Up_Strip_5",
    "3Up_Strip_1", "3Up_Strip_2", "3Up_Strip_3", "3Up_Strip_4", "3Up_Strip_5",
    "4Up_Strip_1", "4Up_Strip_2", "4Up_Strip_3", "4Up_Strip_4", "4Up_Strip_5",
    "1Down_Strip_1", "1Down_Strip_2", "1Down_Strip_3", "1Down_Strip_4", "1Down_Strip_5",
    "2Down_Strip_1", "2Down_Strip_2", "2Down_Strip_3", "2Down_Strip_4", "2Down_Strip_5",
    "3Down_Strip_1", "3Down_Strip_2", "3Down_Strip_3", "3Down_Strip_4", "3Down_Strip_5",
    "4Down_Strip_1", "4Down_Strip_2", "4Down_Strip_3", "4Down_Strip_4", "4Down_Strip_5"
    };

  double value_support[8] = {
    e_1Up_Support, e_2Up_Support, e_3Up_Support, e_4Up_Support,
    e_1Down_Support, e_2Down_Support, e_3Down_Support, e_4Down_Support
  };

  double value_dl[40] = {
    e_1Up_Strip_1_dl, e_1Up_Strip_2_dl, e_1Up_Strip_3_dl, e_1Up_Strip_4_dl, e_1Up_Strip_5_dl,
    e_2Up_Strip_1_dl, e_2Up_Strip_2_dl, e_2Up_Strip_3_dl, e_2Up_Strip_4_dl, e_2Up_Strip_5_dl,
    e_3Up_Strip_1_dl, e_3Up_Strip_2_dl, e_3Up_Strip_3_dl, e_3Up_Strip_4_dl, e_3Up_Strip_5_dl,
    e_4Up_Strip_1_dl, e_4Up_Strip_2_dl, e_4Up_Strip_3_dl, e_4Up_Strip_4_dl, e_4Up_Strip_5_dl,
    e_1Down_Strip_1_dl, e_1Down_Strip_2_dl, e_1Down_Strip_3_dl, e_1Down_Strip_4_dl, e_1Down_Strip_5_dl,
    e_2Down_Strip_1_dl, e_2Down_Strip_2_dl, e_2Down_Strip_3_dl, e_2Down_Strip_4_dl, e_2Down_Strip_5_dl,
    e_3Down_Strip_1_dl, e_3Down_Strip_2_dl, e_3Down_Strip_3_dl, e_3Down_Strip_4_dl, e_3Down_Strip_5_dl,
    e_4Down_Strip_1_dl, e_4Down_Strip_2_dl, e_4Down_Strip_3_dl, e_4Down_Strip_4_dl, e_4Down_Strip_5_dl
    };

  // G4ThreeVector value_p[40] = {
  //   p_1Up_Strip_1, p_1Up_Strip_2, p_1Up_Strip_3, p_1Up_Strip_4, p_1Up_Strip_5,
  //   p_2Up_Strip_1, p_2Up_Strip_2, p_2Up_Strip_3, p_2Up_Strip_4, p_2Up_Strip_5,
  //   p_3Up_Strip_1, p_3Up_Strip_2, p_3Up_Strip_3, p_3Up_Strip_4, p_3Up_Strip_5,
  //   p_4Up_Strip_1, p_4Up_Strip_2, p_4Up_Strip_3, p_4Up_Strip_4, p_4Up_Strip_5,
  //   p_1Down_Strip_1, p_1Down_Strip_2, p_1Down_Strip_3, p_1Down_Strip_4, p_1Down_Strip_5,
  //   p_2Down_Strip_1, p_2Down_Strip_2, p_2Down_Strip_3, p_2Down_Strip_4, p_2Down_Strip_5,
  //   p_3Down_Strip_1, p_3Down_Strip_2, p_3Down_Strip_3, p_3Down_Strip_4, p_3Down_Strip_5,
  //   p_4Down_Strip_1, p_4Down_Strip_2, p_4Down_Strip_3, p_4Down_Strip_4, p_4Down_Strip_5
  //   };

  int size1 = sizeof value_det/sizeof value_det[0];
  for (int i=0; i<=size1-1; i++)
  {
    if (value_det[i] != 0)
    {
      e_DetProton = value_det[i];
      n_DetProton = name_det[i];
      e_DeadLayer = value_dl[i];
      // x_Det = value_p[i].x();
      // y_Det = value_p[i].y();
      // z_Det = value_p[i].z();
      e_Support = value_support[(int)(i/5)];
    }
  }
  if (e_DetProton == 0.){e_DetProton = 0.; n_DetProton = "none"; }//e_DeadLayer = 0.; e_Support = 0., px_DeadLayer = 0., py_DeadLayer = 0., pz_DeadLayer = 0.;}

  e_Catcher = e_CatcherMylar;// + e_CatcherAl2 + e_CatcherAl1;


  int divi = 10000;



  sim_num = GetInputName().substr(GetInputName().length()-5, 1);


  if (counts == 0)
  {
    cout<<"Wisard Thread : "<<sim_num<<" ----> Events "<<to_string(counts)<<endl;

    //Tree
    MyTree = new TTree("MyTree", "Tree_information");
    // TBranch* initial_x = MyTree->Branch("x", &x, "x/D");
    // TBranch* initial_y = MyTree->Branch("y", &y, "y/D");
    // TBranch* initial_z = MyTree->Branch("z", &z, "z/D");
    // TBranch* traj_x = MyTree->Branch("x_vec", "vector<double>", &x_vec);
    // TBranch* traj_y = MyTree->Branch("y_vec", "vector<double>", &y_vec);
    // TBranch* traj_z = MyTree->Branch("z_vec", "vector<double>", &z_vec);
    TBranch* initial_ppx = MyTree->Branch("p_px", &p_px, "p_px/D");
    TBranch* initial_ppy = MyTree->Branch("p_py", &p_py, "p_py/D");
    TBranch* initial_ppz = MyTree->Branch("p_pz", &p_pz, "p_pz/D");
    TBranch* initial_epx = MyTree->Branch("e_px", &e_px, "e_px/D");
    TBranch* initial_epy = MyTree->Branch("e_py", &e_py, "e_py/D");
    TBranch* initial_epz = MyTree->Branch("e_pz", &e_pz, "e_pz/D");
    // TBranch* radius_positron = MyTree->Branch("r_positron", &r_positron, "r_positron/D");
    // TBranch* radius_proton = MyTree->Branch("r_proton", &r_proton, "r_proton/D");
    TBranch* energy_proton = MyTree->Branch("e_proton", &e_proton, "e_proton/D");
    TBranch* energy_positron = MyTree->Branch("e_positron", &e_positron, "e_positron/D");
    TBranch* energy_Catcher = MyTree->Branch("e_Catcher", &e_Catcher, "e_Catcher/D");
    TBranch* energy_DetPositron = MyTree->Branch("e_PlasticScintillator", &e_PlasticScintillator, "e_PlasticScintillator/D");
    TBranch* name_DetProton = MyTree->Branch("n_DetProton", &n_DetProton);
    TBranch* energy_DetProton = MyTree->Branch("e_DetProton", &e_DetProton, "e_DetProton/D");
    TBranch* energy_DeadLayer = MyTree->Branch("e_DeadLayer", &e_DeadLayer, "e_DeadLayer/D");
    // TBranch* Position_x_Det = MyTree->Branch("x_Det", &x_Det, "x_Det/D");
    // TBranch* Position_y_Det = MyTree->Branch("y_Det", &y_Det, "y_Det/D");
    // TBranch* Position_z_Det = MyTree->Branch("z_Det", &z_Det, "z_Det/D");
    //TBranch* energy_Support = MyTree->Branch("e_Support", &e_Support, "e_Support/D");

    // if(initial_x == NULL) {}
    // if(initial_y == NULL) {}
    // if(initial_z == NULL) {}
    // if(initial_ppx == NULL) {}
    // if(initial_ppy == NULL) {}
    // if(initial_ppz == NULL) {}
    // if(initial_epx == NULL) {}
    // if(initial_epy == NULL) {}
    // if(initial_epz == NULL) {}
    if(energy_proton == NULL) {}
    if(energy_positron == NULL) {}
    // if(energy_Catcher == NULL) {}
    if(energy_DetPositron == NULL) {}
    if(name_DetProton == NULL) {}
    if(energy_DetProton == NULL) {}
    if(energy_DeadLayer == NULL) {}
    // if(Position_x_Det == NULL) {}
    // if(Position_y_Det == NULL) {}
    // if(Position_z_Det == NULL) {}
    //if(energy_Support == NULL) {}


    for (int i = 0; i < size1; i++) {
        histos_coinc[i] = new TH1D((name_det[i]+"_coinc").c_str(), (name_det[i]+"_coinc").c_str(), 100000, 0.0, 10000.0);
        histos_nocoinc[i] = new TH1D((name_det[i]+"_nocoinc").c_str(), (name_det[i]+"_nocoinc").c_str(), 100000, 0.0, 10000.0);
    }

  }
  counts++;

  // if (counts%divi==0)
  // {
  //   cout<<"Wisard Thread : "<<sim_num<<" ----> Events "<<to_string(counts)<<"\n"<<endl;
  // }

  if (counts%divi==0)
  {
    MyTree->AutoSave("FlushBaskets");
    for (int i = 0; i < size1; i++)
      {
        histos_coinc[i]->Write("", TObject::kOverwrite);
        histos_nocoinc[i]->Write("", TObject::kOverwrite);
      }
  }




    ////////RADIUS CURVATURE//////////
    // G4TrajectoryContainer* trajcon = event->GetTrajectoryContainer();
    //
    //   for (int i=0; i<trajcon->size(); i++)
    //   {
    //     G4VTrajectory* traj = (*trajcon)[i];
    //     if (traj->GetParentID()==0)
    //     {
    //       vector<double> x_vec;
    //       vector<double> y_vec;
    //       vector<double> r_vec;
    //       for (G4int j=0; j<traj->GetPointEntries(); j++)
    //       {
    //           G4VTrajectoryPoint* point = traj->GetPoint(j);
    //           x_vec.push_back(point->GetPosition().x()/cm);
    //           y_vec.push_back(point->GetPosition().y()/cm);
    //       }
    //
    //       double mean_x_vec = std::accumulate(x_vec.begin(), x_vec.end(), 0.0)/x_vec.size();
    //       double mean_y_vec = std::accumulate(y_vec.begin(), y_vec.end(), 0.0)/y_vec.size();
    //
    //       for (int p=0; p<traj->GetPointEntries();p++)
    //       {
    //         r_vec.push_back(sqrt( pow(x_vec[p]-mean_x_vec, 2.0) + pow(y_vec[p]-mean_y_vec, 2.0) ));
    //       }
    //
    //       if (traj->GetParticleName() == "proton")
    //       {
    //         r_proton = std::accumulate(r_vec.begin(), r_vec.end(), 0.0)/r_vec.size();
    //       }
    //       else
    //       {
    //         r_positron = std::accumulate(r_vec.begin(), r_vec.end(), 0.0)/r_vec.size();
    //       }
    //       //G4cout<<std::accumulate(r.begin(), r.end(), 0.0)/r.size()<<G4endl;
    //     }
    //   }

    if (event->GetPrimaryVertex() != 0)
    {

      x = event->GetPrimaryVertex()->GetX0();
      y = event->GetPrimaryVertex()->GetY0();
      z = event->GetPrimaryVertex()->GetZ0();



      if (event->GetPrimaryVertex(0)->GetPrimary()->GetG4code()->GetParticleName() == "e+")
      {
        e_positron = event->GetPrimaryVertex(0)->GetPrimary()->GetKineticEnergy()/keV;
        e_px = event->GetPrimaryVertex(0)->GetPrimary()->GetMomentumDirection().x();
        e_py = event->GetPrimaryVertex(0)->GetPrimary()->GetMomentumDirection().y();
        e_pz = event->GetPrimaryVertex(0)->GetPrimary()->GetMomentumDirection().z();
        e_proton = event->GetPrimaryVertex(1)->GetPrimary()->GetKineticEnergy()/keV;
        p_px = event->GetPrimaryVertex(1)->GetPrimary()->GetMomentumDirection().x();
        p_py = event->GetPrimaryVertex(1)->GetPrimary()->GetMomentumDirection().y();
        p_pz = event->GetPrimaryVertex(1)->GetPrimary()->GetMomentumDirection().z();
      }
      else
      {
        e_positron = event->GetPrimaryVertex(1)->GetPrimary()->GetKineticEnergy()/keV;
        e_px = event->GetPrimaryVertex(1)->GetPrimary()->GetMomentumDirection().x();
        e_py = event->GetPrimaryVertex(1)->GetPrimary()->GetMomentumDirection().y();
        e_pz = event->GetPrimaryVertex(1)->GetPrimary()->GetMomentumDirection().z();
        e_proton = event->GetPrimaryVertex(0)->GetPrimary()->GetKineticEnergy()/keV;
        p_px = event->GetPrimaryVertex(0)->GetPrimary()->GetMomentumDirection().x();
        p_py = event->GetPrimaryVertex(0)->GetPrimary()->GetMomentumDirection().y();
        p_pz = event->GetPrimaryVertex(0)->GetPrimary()->GetMomentumDirection().z();
      }




      //TRAJECTORY//
      // G4TrajectoryContainer* trajcon = event->GetTrajectoryContainer();
      //
      //
      //   for (long unsigned int i=0; i<trajcon->size(); i++)
      //   {
      //     G4VTrajectory* traj = (*trajcon)[i];
      //
      //     if (traj->GetParticleName() == "proton" && e_DetProton != 0.)
      //     {
      //       for (G4int j=0; j<traj->GetPointEntries(); j++)
      //       {
      //           G4VTrajectoryPoint* point = traj->GetPoint(j);
      //           x_vec.push_back(point->GetPosition().x()/cm);
      //           y_vec.push_back(point->GetPosition().y()/cm);
      //           z_vec.push_back(point->GetPosition().z()/cm);
      //       }
      //     }
      //   }
      }

      MyTree->Fill();
      for (int i = 0; i < size1; i++) {
            if (value_det[i] != 0.)
            {
              if (e_PlasticScintillator > 100.) {histos_coinc[i]->Fill(value_det[i]);}
              else {histos_nocoinc[i]->Fill(value_det[i]);}
            }
        }


  e_PlasticScintillator += CLHEP::RandGauss::shoot(0.,0.0);
  e_PlasticScintillator = max(e_PlasticScintillator, 0.);


  double mean = 0.;
  double std  = 0.;

  e_1Up_Strip_1 += CLHEP::RandGauss::shoot(mean, std);
  e_1Up_Strip_1 = max(e_1Up_Strip_1, 0.);
  e_1Up_Strip_2 += CLHEP::RandGauss::shoot(mean, std);
  e_1Up_Strip_2 = max(e_1Up_Strip_2, 0.);
  e_1Up_Strip_3 += CLHEP::RandGauss::shoot(mean, std);
  e_1Up_Strip_3 = max(e_1Up_Strip_3, 0.);
  e_1Up_Strip_4 += CLHEP::RandGauss::shoot(mean, std);
  e_1Up_Strip_4 = max(e_1Up_Strip_4, 0.);
  e_1Up_Strip_5 += CLHEP::RandGauss::shoot(mean, std);
  e_1Up_Strip_5 = max(e_1Up_Strip_5, 0.);

  e_2Up_Strip_1 += CLHEP::RandGauss::shoot(mean, std);
  e_2Up_Strip_1 = max(e_2Up_Strip_1, 0.);
  e_2Up_Strip_2 += CLHEP::RandGauss::shoot(mean, std);
  e_2Up_Strip_2 = max(e_2Up_Strip_2, 0.);
  e_2Up_Strip_3 += CLHEP::RandGauss::shoot(mean, std);
  e_2Up_Strip_3 = max(e_2Up_Strip_3, 0.);
  e_2Up_Strip_4 += CLHEP::RandGauss::shoot(mean, std);
  e_2Up_Strip_4 = max(e_2Up_Strip_4, 0.);
  e_2Up_Strip_5 += CLHEP::RandGauss::shoot(mean, std);
  e_2Up_Strip_5 = max(e_2Up_Strip_5, 0.);

  e_3Up_Strip_1 += CLHEP::RandGauss::shoot(mean, std);
  e_3Up_Strip_1 = max(e_3Up_Strip_1, 0.);
  e_3Up_Strip_2 += CLHEP::RandGauss::shoot(mean, std);
  e_3Up_Strip_2 = max(e_3Up_Strip_2, 0.);
  e_3Up_Strip_3 += CLHEP::RandGauss::shoot(mean, std);
  e_3Up_Strip_3 = max(e_3Up_Strip_3, 0.);
  e_3Up_Strip_4 += CLHEP::RandGauss::shoot(mean, std);
  e_3Up_Strip_4 = max(e_3Up_Strip_4, 0.);
  e_3Up_Strip_5 += CLHEP::RandGauss::shoot(mean, std);
  e_3Up_Strip_5 = max(e_3Up_Strip_5, 0.);

  e_4Up_Strip_1 += CLHEP::RandGauss::shoot(mean, std);
  e_4Up_Strip_1 = max(e_4Up_Strip_1, 0.);
  e_4Up_Strip_2 += CLHEP::RandGauss::shoot(mean, std);
  e_4Up_Strip_2 = max(e_4Up_Strip_2, 0.);
  e_4Up_Strip_3 += CLHEP::RandGauss::shoot(mean, std);
  e_4Up_Strip_3 = max(e_4Up_Strip_3, 0.);
  e_4Up_Strip_4 += CLHEP::RandGauss::shoot(mean, std);
  e_4Up_Strip_4 = max(e_4Up_Strip_4, 0.);
  e_4Up_Strip_5 += CLHEP::RandGauss::shoot(mean, std);
  e_4Up_Strip_5 = max(e_4Up_Strip_5, 0.);

  e_1Down_Strip_1 += CLHEP::RandGauss::shoot(mean, std);
  e_1Down_Strip_1 = max(e_1Down_Strip_1, 0.);
  e_1Down_Strip_2 += CLHEP::RandGauss::shoot(mean, std);
  e_1Down_Strip_2 = max(e_1Down_Strip_2, 0.);
  e_1Down_Strip_3 += CLHEP::RandGauss::shoot(mean, std);
  e_1Down_Strip_3 = max(e_1Down_Strip_3, 0.);
  e_1Down_Strip_4 += CLHEP::RandGauss::shoot(mean, std);
  e_1Down_Strip_4 = max(e_1Down_Strip_4, 0.);
  e_1Down_Strip_5 += CLHEP::RandGauss::shoot(mean, std);
  e_1Down_Strip_5 = max(e_1Down_Strip_5, 0.);

  e_2Down_Strip_1 += CLHEP::RandGauss::shoot(mean, std);
  e_2Down_Strip_1 = max(e_2Down_Strip_1, 0.);
  e_2Down_Strip_2 += CLHEP::RandGauss::shoot(mean, std);
  e_2Down_Strip_2 = max(e_2Down_Strip_2, 0.);
  e_2Down_Strip_3 += CLHEP::RandGauss::shoot(mean, std);
  e_2Down_Strip_3 = max(e_2Down_Strip_3, 0.);
  e_2Down_Strip_4 += CLHEP::RandGauss::shoot(mean, std);
  e_2Down_Strip_4 = max(e_2Down_Strip_4, 0.);
  e_2Down_Strip_5 += CLHEP::RandGauss::shoot(mean, std);
  e_2Down_Strip_5 = max(e_2Down_Strip_5, 0.);

  e_3Down_Strip_1 += CLHEP::RandGauss::shoot(mean, std);
  e_3Down_Strip_1 = max(e_3Down_Strip_1, 0.);
  e_3Down_Strip_2 += CLHEP::RandGauss::shoot(mean, std);
  e_3Down_Strip_2 = max(e_3Down_Strip_2, 0.);
  e_3Down_Strip_3 += CLHEP::RandGauss::shoot(mean, std);
  e_3Down_Strip_3 = max(e_3Down_Strip_3, 0.);
  e_3Down_Strip_4 += CLHEP::RandGauss::shoot(mean, std);
  e_3Down_Strip_4 = max(e_3Down_Strip_4, 0.);
  e_3Down_Strip_5 += CLHEP::RandGauss::shoot(mean, std);
  e_3Down_Strip_5 = max(e_3Down_Strip_5, 0.);

  e_4Down_Strip_1 += CLHEP::RandGauss::shoot(mean, std);
  e_4Down_Strip_1 = max(e_4Down_Strip_1, 0.);
  e_4Down_Strip_2 += CLHEP::RandGauss::shoot(mean, std);
  e_4Down_Strip_2 = max(e_4Down_Strip_2, 0.);
  e_4Down_Strip_3 += CLHEP::RandGauss::shoot(mean, std);
  e_4Down_Strip_3 = max(e_4Down_Strip_3, 0.);
  e_4Down_Strip_4 += CLHEP::RandGauss::shoot(mean, std);
  e_4Down_Strip_4 = max(e_4Down_Strip_4, 0.);
  e_4Down_Strip_5 += CLHEP::RandGauss::shoot(mean, std);
  e_4Down_Strip_5 = max(e_4Down_Strip_5, 0.);



  wisard_sensor_PlasticScintillator->ResetEventEnergy_positron();

  wisard_sensor_1Up_Strip_1->ResetEventEnergy();
  wisard_sensor_1Up_Strip_2->ResetEventEnergy();
  wisard_sensor_1Up_Strip_3->ResetEventEnergy();
  wisard_sensor_1Up_Strip_4->ResetEventEnergy();
  wisard_sensor_1Up_Strip_5->ResetEventEnergy();
  wisard_sensor_1Up_Support->ResetEventEnergy();

  wisard_sensor_2Up_Strip_1->ResetEventEnergy();
  wisard_sensor_2Up_Strip_2->ResetEventEnergy();
  wisard_sensor_2Up_Strip_3->ResetEventEnergy();
  wisard_sensor_2Up_Strip_4->ResetEventEnergy();
  wisard_sensor_2Up_Strip_5->ResetEventEnergy();
  wisard_sensor_2Up_Support->ResetEventEnergy();

  wisard_sensor_3Up_Strip_1->ResetEventEnergy();
  wisard_sensor_3Up_Strip_2->ResetEventEnergy();
  wisard_sensor_3Up_Strip_3->ResetEventEnergy();
  wisard_sensor_3Up_Strip_4->ResetEventEnergy();
  wisard_sensor_3Up_Strip_5->ResetEventEnergy();
  wisard_sensor_3Up_Support->ResetEventEnergy();

  wisard_sensor_4Up_Strip_1->ResetEventEnergy();
  wisard_sensor_4Up_Strip_2->ResetEventEnergy();
  wisard_sensor_4Up_Strip_3->ResetEventEnergy();
  wisard_sensor_4Up_Strip_4->ResetEventEnergy();
  wisard_sensor_4Up_Strip_5->ResetEventEnergy();
  wisard_sensor_4Up_Support->ResetEventEnergy();

  wisard_sensor_1Down_Strip_1->ResetEventEnergy();
  wisard_sensor_1Down_Strip_2->ResetEventEnergy();
  wisard_sensor_1Down_Strip_3->ResetEventEnergy();
  wisard_sensor_1Down_Strip_4->ResetEventEnergy();
  wisard_sensor_1Down_Strip_5->ResetEventEnergy();
  wisard_sensor_1Down_Support->ResetEventEnergy();

  wisard_sensor_2Down_Strip_1->ResetEventEnergy();
  wisard_sensor_2Down_Strip_2->ResetEventEnergy();
  wisard_sensor_2Down_Strip_3->ResetEventEnergy();
  wisard_sensor_2Down_Strip_4->ResetEventEnergy();
  wisard_sensor_2Down_Strip_5->ResetEventEnergy();
  wisard_sensor_2Down_Support->ResetEventEnergy();

  wisard_sensor_3Down_Strip_1->ResetEventEnergy();
  wisard_sensor_3Down_Strip_2->ResetEventEnergy();
  wisard_sensor_3Down_Strip_3->ResetEventEnergy();
  wisard_sensor_3Down_Strip_4->ResetEventEnergy();
  wisard_sensor_3Down_Strip_5->ResetEventEnergy();
  wisard_sensor_3Down_Support->ResetEventEnergy();

  wisard_sensor_4Down_Strip_1->ResetEventEnergy();
  wisard_sensor_4Down_Strip_2->ResetEventEnergy();
  wisard_sensor_4Down_Strip_3->ResetEventEnergy();
  wisard_sensor_4Down_Strip_4->ResetEventEnergy();
  wisard_sensor_4Down_Strip_5->ResetEventEnergy();
  wisard_sensor_4Down_Support->ResetEventEnergy();

  wisard_sensor_CatcherMylar->ResetEventEnergy();
  // wisard_sensor_CatcherAl1->ResetEventEnergy();
  // wisard_sensor_CatcherAl2->ResetEventEnergy();

  wisard_sensor_1Up_Strip_1_dl->ResetEventEnergy();
  wisard_sensor_1Up_Strip_2_dl->ResetEventEnergy();
  wisard_sensor_1Up_Strip_3_dl->ResetEventEnergy();
  wisard_sensor_1Up_Strip_4_dl->ResetEventEnergy();
  wisard_sensor_1Up_Strip_5_dl->ResetEventEnergy();

  wisard_sensor_2Up_Strip_1_dl->ResetEventEnergy();
  wisard_sensor_2Up_Strip_2_dl->ResetEventEnergy();
  wisard_sensor_2Up_Strip_3_dl->ResetEventEnergy();
  wisard_sensor_2Up_Strip_4_dl->ResetEventEnergy();
  wisard_sensor_2Up_Strip_5_dl->ResetEventEnergy();

  wisard_sensor_3Up_Strip_1_dl->ResetEventEnergy();
  wisard_sensor_3Up_Strip_2_dl->ResetEventEnergy();
  wisard_sensor_3Up_Strip_3_dl->ResetEventEnergy();
  wisard_sensor_3Up_Strip_4_dl->ResetEventEnergy();
  wisard_sensor_3Up_Strip_5_dl->ResetEventEnergy();

  wisard_sensor_4Up_Strip_1_dl->ResetEventEnergy();
  wisard_sensor_4Up_Strip_2_dl->ResetEventEnergy();
  wisard_sensor_4Up_Strip_3_dl->ResetEventEnergy();
  wisard_sensor_4Up_Strip_4_dl->ResetEventEnergy();
  wisard_sensor_4Up_Strip_5_dl->ResetEventEnergy();

  wisard_sensor_1Down_Strip_1_dl->ResetEventEnergy();
  wisard_sensor_1Down_Strip_2_dl->ResetEventEnergy();
  wisard_sensor_1Down_Strip_3_dl->ResetEventEnergy();
  wisard_sensor_1Down_Strip_4_dl->ResetEventEnergy();
  wisard_sensor_1Down_Strip_5_dl->ResetEventEnergy();

  wisard_sensor_2Down_Strip_1_dl->ResetEventEnergy();
  wisard_sensor_2Down_Strip_2_dl->ResetEventEnergy();
  wisard_sensor_2Down_Strip_3_dl->ResetEventEnergy();
  wisard_sensor_2Down_Strip_4_dl->ResetEventEnergy();
  wisard_sensor_2Down_Strip_5_dl->ResetEventEnergy();

  wisard_sensor_3Down_Strip_1_dl->ResetEventEnergy();
  wisard_sensor_3Down_Strip_2_dl->ResetEventEnergy();
  wisard_sensor_3Down_Strip_3_dl->ResetEventEnergy();
  wisard_sensor_3Down_Strip_4_dl->ResetEventEnergy();
  wisard_sensor_3Down_Strip_5_dl->ResetEventEnergy();

  wisard_sensor_4Down_Strip_1_dl->ResetEventEnergy();
  wisard_sensor_4Down_Strip_2_dl->ResetEventEnergy();
  wisard_sensor_4Down_Strip_3_dl->ResetEventEnergy();
  wisard_sensor_4Down_Strip_4_dl->ResetEventEnergy();
  wisard_sensor_4Down_Strip_5_dl->ResetEventEnergy();

  // wisard_sensor_1Up_Strip_1->ResetPositionEnter();
  // wisard_sensor_1Up_Strip_2->ResetPositionEnter();
  // wisard_sensor_1Up_Strip_3->ResetPositionEnter();
  // wisard_sensor_1Up_Strip_4->ResetPositionEnter();
  // wisard_sensor_1Up_Strip_5->ResetPositionEnter();
  //
  // wisard_sensor_2Up_Strip_1->ResetPositionEnter();
  // wisard_sensor_2Up_Strip_2->ResetPositionEnter();
  // wisard_sensor_2Up_Strip_3->ResetPositionEnter();
  // wisard_sensor_2Up_Strip_4->ResetPositionEnter();
  // wisard_sensor_2Up_Strip_5->ResetPositionEnter();
  //
  // wisard_sensor_3Up_Strip_1->ResetPositionEnter();
  // wisard_sensor_3Up_Strip_2->ResetPositionEnter();
  // wisard_sensor_3Up_Strip_3->ResetPositionEnter();
  // wisard_sensor_3Up_Strip_4->ResetPositionEnter();
  // wisard_sensor_3Up_Strip_5->ResetPositionEnter();
  //
  // wisard_sensor_4Up_Strip_1->ResetPositionEnter();
  // wisard_sensor_4Up_Strip_2->ResetPositionEnter();
  // wisard_sensor_4Up_Strip_3->ResetPositionEnter();
  // wisard_sensor_4Up_Strip_4->ResetPositionEnter();
  // wisard_sensor_4Up_Strip_5->ResetPositionEnter();
  //
  // wisard_sensor_1Down_Strip_1->ResetPositionEnter();
  // wisard_sensor_1Down_Strip_2->ResetPositionEnter();
  // wisard_sensor_1Down_Strip_3->ResetPositionEnter();
  // wisard_sensor_1Down_Strip_4->ResetPositionEnter();
  // wisard_sensor_1Down_Strip_5->ResetPositionEnter();
  //
  // wisard_sensor_2Down_Strip_1->ResetPositionEnter();
  // wisard_sensor_2Down_Strip_2->ResetPositionEnter();
  // wisard_sensor_2Down_Strip_3->ResetPositionEnter();
  // wisard_sensor_2Down_Strip_4->ResetPositionEnter();
  // wisard_sensor_2Down_Strip_5->ResetPositionEnter();
  //
  // wisard_sensor_3Down_Strip_1->ResetPositionEnter();
  // wisard_sensor_3Down_Strip_2->ResetPositionEnter();
  // wisard_sensor_3Down_Strip_3->ResetPositionEnter();
  // wisard_sensor_3Down_Strip_4->ResetPositionEnter();
  // wisard_sensor_3Down_Strip_5->ResetPositionEnter();
  //
  // wisard_sensor_4Down_Strip_1->ResetPositionEnter();
  // wisard_sensor_4Down_Strip_2->ResetPositionEnter();
  // wisard_sensor_4Down_Strip_3->ResetPositionEnter();
  // wisard_sensor_4Down_Strip_4->ResetPositionEnter();
  // wisard_sensor_4Down_Strip_5->ResetPositionEnter();

  e_DetProton = 0;
  n_DetProton = "none";
  e_DeadLayer = 0.;
  // e_Catcher = 0;
  e_CatcherMylar = 0;
  // e_CatcherAl2 = 0;
  // e_CatcherAl1 = 0;
  x_Det = 0.;
  y_Det = 0.;
  z_Det = 0.;
  e_Support = 0.;
  x_vec.clear();
  y_vec.clear();
  z_vec.clear();

}
