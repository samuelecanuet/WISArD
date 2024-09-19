#include "Wisard_RunManager.hh"
#include "Wisard_Sensor.hh"

#include "G4UImanager.hh"

#include "TH1D.h"
#include "TObjString.h"

#include <iostream>
//----------------------------------------------------------------------

// constructor
Wisard_RunManager::Wisard_RunManager()
{
  cout << "Constructor Wisard_RunManager" << endl;
  
  ////////////// Construct all sensors //////////////////////////////
  cout << "Constructor Wisard_Sensor" << endl;
  wisard_sensor_PlasticScintillator = new Wisard_Sensor;
  for (int i = 0; i < nb_det; i++)
  {
    dic_detector[Detector_Code[i]] = std::make_pair(new Wisard_Sensor, new Wisard_Sensor);
  }
  for (int i : InterDetector_Code)
  {
    dic_interdetector[i] = new Wisard_Sensor;
  }
  wisard_sensor_CatcherMylar_central = new Wisard_Sensor;
  wisard_sensor_CatcherAl1_central = new Wisard_Sensor;
  wisard_sensor_CatcherAl2_central = new Wisard_Sensor;
  wisard_sensor_CatcherMylar_side = new Wisard_Sensor;
  wisard_sensor_CatcherAl1_side = new Wisard_Sensor;
  wisard_sensor_CatcherAl2_side = new Wisard_Sensor;
  ///////////////////////////////////////////////////////////////////

  ////////////// Construct strips coinc/anticoinc HISTOGRAMS ////////
  for (int i = 0; i < nb_det; i++)
  {
    silicon_coinc[i] = new TH1D((Detector_Name[i] + "_coinc").c_str(), (Detector_Name[i] + "_coinc").c_str(), 100000, 0.0, 10000.0);
    silicon_nocoinc[i] = new TH1D((Detector_Name[i] + "_nocoinc").c_str(), (Detector_Name[i] + "_nocoinc").c_str(), 100000, 0.0, 10000.0);
    silicon_single[i] = new TH1D((Detector_Name[i] + "_single").c_str(), (Detector_Name[i] + "_single").c_str(), 100000, 0.0, 10000.0);
  }
  plastic_coinc = new TH1D("plastic_coinc", "plastic_coinc", 120000, 0.0, 12000.0);
  ///////////////////////////////////////////////////////////////////
}

// destructor
Wisard_RunManager::~Wisard_RunManager()
{
  Tree->AutoSave("FlushBaskets");
  cout << "Destructor Wisard_RunManager" << endl;

  f->Close();
  delete f;

  cout << "Destructor Wisard_Sensor" << endl;
  delete wisard_sensor_PlasticScintillator;
  for (int i = 0; i < nb_det; i++)
  {
    delete dic_detector[Detector_Code[i]].first;
    delete dic_detector[Detector_Code[i]].second;
  }
  for (int i : InterDetector_Code)
  {
    delete dic_interdetector[i];
  }
  delete wisard_sensor_CatcherMylar_central;
  delete wisard_sensor_CatcherAl1_central;
  delete wisard_sensor_CatcherAl2_central;
  delete wisard_sensor_CatcherMylar_side;
  delete wisard_sensor_CatcherAl1_side;
  delete wisard_sensor_CatcherAl2_side;

  CloseInput();
}

//----------------------------------------------------------------------
void Wisard_RunManager::AnalyzeEvent(G4Event *event)
{  
  if (event->GetEventID() == 0)
  {
    f = new TFile(GetOutputFilename(), "recreate");
    ////////////// Construct Log /////////////////////////////////////
    TDirectory *dir = f->mkdir("Log");
    dir->cd();
    G4UImanager *uiManager = G4UImanager::GetUIpointer();
    for (int i = 0; i < uiManager->GetNumberOfHistory(); i++)
    {
      string command = uiManager->GetPreviousCommand(i).substr(1);
      if (command.find("run") != 0 && command.find("event") != 0 && command.find("tracking") != 0 && command.find("input") != 0 && command.find("output") != 0)
      {
        char const *num_char = command.c_str();
        dir->WriteObject(&command, num_char);
      }
      f->cd();
    }
    //////////////////////////////////////////////////////////////////

    ////////////// Construct the TREE ////////////////////////////////
    Tree = new TTree("Tree", "Informations");
    Tree->Branch("Particle_PDG", &Particle_PDG);
    Tree->Branch("x", &x);
    Tree->Branch("y", &y);
    Tree->Branch("z", &z);
    Tree->Branch("px", &px);
    Tree->Branch("py", &py);
    Tree->Branch("pz", &pz);
    Tree->Branch("Kinetic_Energy", &Kinetic_Energy);
    Tree->Branch("Catcher_Central_Energy_Deposit", &Catcher_Central_Energy_Deposit);
    Tree->Branch("Catcher_Side_Energy_Deposit", &Catcher_Side_Energy_Deposit);
    Tree->Branch("PlasticScintillator_Energy_Deposit", &PlasticScintillator_Energy_Deposit);
    Tree->Branch("PlasticScintillator_Hit_Position", &PlasticScintillator_Hit_Position);
    Tree->Branch("PlasticScintillator_Hit_Angle", &PlasticScintillator_Hit_Angle);
    Tree->Branch("Silicon_Detector_Energy_Deposit", &Silicon_Detector_Energy_Deposit);
    Tree->Branch("Silicon_Detector_Hit_Position", &Silicon_Detector_Hit_Position);
    Tree->Branch("Silicon_Detector_Hit_Angle", &Silicon_Detector_Hit_Angle);
    Tree->Branch("Silicon_Detector_Code", &Silicon_Detector_Code);
    Tree->Branch("Silicon_Detector_DL_Energy_Deposit", &Silicon_Detector_DL_Energy_Deposit);
    Tree->Branch("Silicon_Detector_InterStrip_Energy_Deposit", &Silicon_Detector_InterStrip_Energy_Deposit);
    Tree->Branch("Silicon_Detector_InterStrip_Hit_Position", &Silicon_Detector_InterStrip_Hit_Position);
    ///////////////////////////////////////////////////////////////////
  }

  G4RunManager::AnalyzeEvent(event);

  /// SAVE GENERATOR INFORMATION ///
  G4PrimaryVertex *PrimaryVertex = event->GetPrimaryVertex();
  if (PrimaryVertex != 0)
  {
    
    for (int part = 0; part < event->GetNumberOfPrimaryVertex(); part++)
    {
      G4PrimaryParticle *Primary = event->GetPrimaryVertex(part)->GetPrimary();
      G4ThreeVector Momentum = Primary->GetMomentumDirection();

      Particle_PDG.push_back(Primary->GetG4code()->GetPDGEncoding());
      x.push_back(PrimaryVertex->GetX0() / um);
      y.push_back(PrimaryVertex->GetY0() / um);
      z.push_back(PrimaryVertex->GetZ0() / nm);
      px.push_back(Momentum.x());
      py.push_back(Momentum.y());
      pz.push_back(Momentum.z());
      Kinetic_Energy.push_back(Primary->GetKineticEnergy() / keV);

      //# CATCHER #//
      Catcher_Central_Energy_Deposit.push_back(wisard_sensor_CatcherMylar_central->GetDictionnary()[part].EnergyDeposit + wisard_sensor_CatcherAl1_central->GetDictionnary()[part].EnergyDeposit + wisard_sensor_CatcherAl2_central->GetDictionnary()[part].EnergyDeposit);
      Catcher_Side_Energy_Deposit.push_back(wisard_sensor_CatcherMylar_side->GetDictionnary()[part].EnergyDeposit + wisard_sensor_CatcherAl1_side->GetDictionnary()[part].EnergyDeposit + wisard_sensor_CatcherAl2_side->GetDictionnary()[part].EnergyDeposit);

      //# PLASTIC SCINTILLATOR #//
      PlasticScintillator_Energy_Deposit.push_back(wisard_sensor_PlasticScintillator->GetDictionnary()[part].EnergyDeposit);
      PlasticScintillator_Hit_Position.push_back(wisard_sensor_PlasticScintillator->GetDictionnary()[part].HitPosition);
      PlasticScintillator_Hit_Angle.push_back(wisard_sensor_PlasticScintillator->GetDictionnary()[part].HitAngle);

      //# SILICON DETECTORS #//
      vector<G4double> Silicon_Detector_Energy_Deposit_part, Silicon_Detector_DL_Energy_Deposit_part, Silicon_Detector_Hit_Angle_part;
      vector<G4ThreeVector> Silicon_Detector_Hit_Position_part;
      vector<G4int> Silicon_Detector_Code_part;
      vector<vector<G4double>> Silicon_Detector_InterStrip_Energy_Deposit_part;
      vector<vector<G4ThreeVector>> Silicon_Detector_InterStrip_Hit_Position_part;
      vector<G4int> Silicon_Detector_InterStrip_Code_part;

      for (int i = 0; i < nb_det; i++)
      {
        if (dic_detector[Detector_Code[i]].first->GetDictionnary()[part].EnergyDeposit != 0 || dic_detector[Detector_Code[i]].second->GetDictionnary()[part].EnergyDeposit != 0)
        {
          Silicon_Detector_Code_part.push_back(Detector_Code[i]);
          Silicon_Detector_Energy_Deposit_part.push_back(dic_detector[Detector_Code[i]].first->GetDictionnary()[part].EnergyDeposit);
          Silicon_Detector_DL_Energy_Deposit_part.push_back(dic_detector[Detector_Code[i]].second->GetDictionnary()[part].EnergyDeposit);
          Silicon_Detector_Hit_Position_part.push_back(dic_detector[Detector_Code[i]].first->GetDictionnary()[part].HitPosition);
          Silicon_Detector_Hit_Angle_part.push_back(dic_detector[Detector_Code[i]].first->GetDictionnary()[part].HitAngle);
        }
      }

      for (int i : InterDetector_Code)
      {
        if (dic_interdetector[i]->GetDictionnary()[part].EnergyDeposit != 0)
        {
          // for reconstruction of energy exchange between strips
          Silicon_Detector_InterStrip_Code_part.push_back(i);
          Silicon_Detector_InterStrip_Energy_Deposit_part.push_back(dic_interdetector[i]->GetDictionnary()[part].InterStrip_EnergyDeposit);
          Silicon_Detector_InterStrip_Hit_Position_part.push_back(dic_interdetector[i]->GetDictionnary()[part].InterStrip_HitPosition);
        }
      }

      Silicon_Detector_Code.push_back(Silicon_Detector_Code_part);
      Silicon_Detector_Energy_Deposit.push_back(Silicon_Detector_Energy_Deposit_part);
      Silicon_Detector_DL_Energy_Deposit.push_back(Silicon_Detector_DL_Energy_Deposit_part);
      Silicon_Detector_Hit_Position.push_back(Silicon_Detector_Hit_Position_part);
      Silicon_Detector_Hit_Angle.push_back(Silicon_Detector_Hit_Angle_part);

      Silicon_Detector_InterStrip_Energy_Deposit.push_back(Silicon_Detector_InterStrip_Energy_Deposit_part);
      Silicon_Detector_InterStrip_Hit_Position.push_back(Silicon_Detector_InterStrip_Hit_Position_part);
    }
  }

  Tree->Fill();

  /// HISTOGRAMS ///

  int proton_index = -1;
  for (int part = 0; part < event->GetNumberOfPrimaryVertex(); part++)
  {
    if (event->GetPrimaryVertex(part)->GetPrimary()->GetG4code()->GetPDGEncoding() == 2212)
    {
      proton_index = part;
    }
  }

  if (proton_index != -1) 
  {
    for (int i = 0; i < nb_det; i++)
    {
      if (dic_detector[Detector_Code[i]].first->GetDictionnary()[proton_index].EnergyDeposit != 0)
      {
        if (wisard_sensor_PlasticScintillator->GetDictionnary()[proton_index].EnergyDeposit >= GetThreshold() / keV)
        {
          silicon_coinc[i]->Fill(dic_detector[Detector_Code[i]].first->GetDictionnary()[proton_index].EnergyDeposit);
          plastic_coinc->Fill(wisard_sensor_PlasticScintillator->GetDictionnary()[proton_index].EnergyDeposit);
        }
        else
        {
          silicon_nocoinc[i]->Fill(dic_detector[Detector_Code[i]].first->GetDictionnary()[proton_index].EnergyDeposit);
        }
        silicon_single[i]->Fill(dic_detector[Detector_Code[i]].first->GetDictionnary()[proton_index].EnergyDeposit);
      }
    }
  }

  int divi = 10000;

  ///// Writing in file ///////////////////////////////////////////
  if (event->GetEventID() % divi == 0)
  {
    Tree->AutoSave("FlushBaskets");

    for (int i = 0; i < nb_det; i++)
    {
      silicon_coinc[i]->Write("", TObject::kOverwrite);
      silicon_nocoinc[i]->Write("", TObject::kOverwrite);
      silicon_single[i]->Write("", TObject::kOverwrite);
    }
    plastic_coinc->Write("", TObject::kOverwrite);
  }

  ///// Reset all dictionnaries of detectors ///////////////////////
  wisard_sensor_PlasticScintillator->ResetDictionnary();
  for (int i = 0; i < nb_det; i++)
  {
    dic_detector[Detector_Code[i]].first->ResetDictionnary();
    dic_detector[Detector_Code[i]].second->ResetDictionnary();
  }
  for (int i : InterDetector_Code)
  {
    dic_interdetector[i]->ResetDictionnary();
  }
  wisard_sensor_CatcherMylar_central->ResetDictionnary();
  wisard_sensor_CatcherAl1_central->ResetDictionnary();
  wisard_sensor_CatcherAl2_central->ResetDictionnary();
  wisard_sensor_CatcherMylar_side->ResetDictionnary();
  wisard_sensor_CatcherAl1_side->ResetDictionnary();
  wisard_sensor_CatcherAl2_side->ResetDictionnary();
  ///////////////////////////////////////////////////////////////////

  ///// Reset all vectors ///////////////////////////////////////////
  Particle_PDG.clear();
  x.clear();
  y.clear();
  z.clear();
  px.clear();
  py.clear();
  pz.clear();
  Kinetic_Energy.clear();
  Silicon_Detector_Code.clear();
  Silicon_Detector_Energy_Deposit.clear();
  Silicon_Detector_DL_Energy_Deposit.clear();
  Silicon_Detector_Hit_Position.clear();
  Silicon_Detector_Hit_Angle.clear();
  Silicon_Detector_InterStrip_Energy_Deposit.clear();
  Silicon_Detector_InterStrip_Hit_Position.clear();
  PlasticScintillator_Energy_Deposit.clear();
  PlasticScintillator_Hit_Angle.clear();
  PlasticScintillator_Hit_Position.clear();
  Catcher_Central_Energy_Deposit.clear();
  Catcher_Side_Energy_Deposit.clear();



  ///////////////////////////////////////////////////////////////////

}

int Wisard_RunManager::OpenInput(const std::string &fname)
{
  int error = 0; // Valeur de retour

  if (fname != "")
  {
    // Fermer la sortie précédente au cas où
    CloseInput();

    // Vérifier l'extension du fichier
    size_t dotPosition = fname.find_last_of('.');
    if (dotPosition != std::string::npos)
    {
      std::string extension = fname.substr(dotPosition + 1);
      input_name = fname;
      if (extension == "root")
      {
        TFile *file = new TFile(fname.c_str(), "READ");
        if (file->IsOpen())
        {
        //     file->Close();
        //     file = new TFile(fname.c_str(), "UPDATE");
        //     TTree *tree = (TTree *)file->Get("ParticleTree");
        //     TTreeReader *Reader = new TTreeReader("ParticleTree", file);
        //     TTreeReaderValue<int> code(*Reader, "code");
        //     TTreeReaderValue<double> energy(*Reader, "energy");
        //     TH1D* hist = new TH1D("histogram", "histogram", 100000, 0.0, 10000.0);
        //     while (Reader->Next())
        //     {
        //       if (*code == 1000020040 || *code == 2212)
        //       {
        //         hist->Fill(*energy);
        //       }
        //     }
        //     hist->Write();  
        //     
          std::cout << "<I> Open input file : " << fname << std::endl;
          file->Close();
          
        }
        else
        {
          std::cerr << "<W> Unable to open CRADLE input file" << std::endl;
          error = 2;
        }
      }
      else if (extension == "txt")
      {
        input_txt.open(fname.c_str());
        if (!input_txt.fail())
        {
          std::cout << "<I> Open input file : " << fname << std::endl;
        }
        else
        {
          std::cerr << "<W> Unable to open CRADLE input file" << std::endl;
          error = 2;
        }
      }
      else
      {
        error = 3;
        std::cerr << "<W> Unrecognized input file extension : " << extension << std::endl;
      }
    }
    else
    {
      error = 4; // Aucune extension de fichier trouvée
      std::cerr << "<W> No file extension found in input file name : " << fname << std::endl;
    }
  }
  else
  {
    error = 1; // Nom de fichier vide
    std::cerr << "<W> Empty input file name" << std::endl;
  }

  return error;
}

int Wisard_RunManager::OpenInputSRIM(const string &fname)
{
  int error = 0; // return value

  if (fname != "")
  {
    // close previous output... just in case
    CloseInputSRIM();

    // try to open the new file
    inputSRIM.open(fname.c_str());

    if (inputSRIM.fail())
    {
      error = 2;
      cerr << "<W> OpenInput : error opening file " << fname << endl;
    }
    else
    {
      input_nameSRIM = fname;
      cout << "<I> Open input file : " << fname << endl;
    }
  }
  else
  {
    error = 1;
    cerr << "<W> OpenInput : empty file name" << endl;
  }

  return (error);
}