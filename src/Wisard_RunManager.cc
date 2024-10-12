#include "Wisard_RunManager.hh"
#include "Wisard_Sensor.hh"


#include "G4UImanager.hh"

#include "TH1D.h"
#include "TObjString.h"

#include <iostream>
//----------------------------------------------------------------------

// constructor
Wisard_RunManager::Wisard_RunManager(ParticleInformation* PartInfos) : PartInfo(PartInfos)
{
  cout << "Constructor Wisard_RunManager" << endl;
  
  ////////////// Construct all sensors //////////////////////////////
  cout << "Constructor Wisard_Sensor" << endl;
  wisard_sensor_PlasticScintillator = new Wisard_Sensor(PartInfo, 99);
  for (int i = 0; i < nb_det; i++)
  {
    dic_detector[Detector_Code[i]] = std::make_pair(new Wisard_Sensor(PartInfo, Detector_Code[i]), new Wisard_Sensor(PartInfo, Detector_Code[i]*10));
  }
  for (int i : InterDetector_Code)
  {
    dic_interdetector[i] = new Wisard_Sensor(PartInfo, i);
  }
  wisard_sensor_CatcherMylar_central = new Wisard_Sensor(PartInfo, 1);
  wisard_sensor_CatcherAl1_central = new Wisard_Sensor(PartInfo, 2);
  wisard_sensor_CatcherAl2_central = new Wisard_Sensor(PartInfo, 3);
  wisard_sensor_CatcherMylar_side = new Wisard_Sensor(PartInfo, 4);
  wisard_sensor_CatcherAl1_side = new Wisard_Sensor(PartInfo, 5);
  wisard_sensor_CatcherAl2_side = new Wisard_Sensor(PartInfo, 6);
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
    Tree->Branch("Silicon_Detector_InterStrip_Code", &Silicon_Detector_InterStrip_Code);
    Tree->Branch("Silicon_Detector_InterStrip_Energy_Deposit", &Silicon_Detector_InterStrip_Energy_Deposit);
    Tree->Branch("Silicon_Detector_InterStrip_Hit_Position", &Silicon_Detector_InterStrip_Hit_Position);
    ///////////////////////////////////////////////////////////////////
  }

  G4RunManager::AnalyzeEvent(event);

  //new 
  for (auto& pair : PartInfo->GetInfo())
  {
    Particle particle = pair.second;

    if (particle.Particle_PDG == 2212)
      cout << particle.E0 << endl;

    Particle_PDG.push_back(particle.Particle_PDG);
    x.push_back(particle.Pos.x());
    y.push_back(particle.Pos.y());
    z.push_back(particle.Pos.z());
    px.push_back(particle.Dir.x());
    py.push_back(particle.Dir.y());
    pz.push_back(particle.Dir.z());     
    Kinetic_Energy.push_back(particle.E0);

    // # Catcher Central #//
    double catcher_central = 0;
    if (particle.Detectors.find(1) != particle.Detectors.end())
      catcher_central += particle.Detectors[1].EnergyDeposit;
    if (particle.Detectors.find(2) != particle.Detectors.end())
      catcher_central += particle.Detectors[2].EnergyDeposit;
    if (particle.Detectors.find(3) != particle.Detectors.end())
      catcher_central += particle.Detectors[3].EnergyDeposit;
    Catcher_Central_Energy_Deposit.push_back(catcher_central);

    // # Catcher Side #//
    double catcher_side = 0;
    if (particle.Detectors.find(4) != particle.Detectors.end())
      catcher_side += particle.Detectors[4].EnergyDeposit;
    if (particle.Detectors.find(5) != particle.Detectors.end())
      catcher_side += particle.Detectors[5].EnergyDeposit;
    if (particle.Detectors.find(6) != particle.Detectors.end())
      catcher_side += particle.Detectors[6].EnergyDeposit;
    Catcher_Side_Energy_Deposit.push_back(catcher_side);
  
    // # Plastic Scintillator #//
    PlasticScintillator_Energy_Deposit.push_back(particle.Detectors[99].EnergyDeposit);
    PlasticScintillator_Hit_Position.push_back(particle.Detectors[99].HitPosition);
    PlasticScintillator_Hit_Angle.push_back(particle.Detectors[99].HitAngle);

    // # Silicon Detectors #//
    for (auto Det : particle.Detectors)
    {
      // Strip
      if (Det.first < 87 && Det.first > 10) 
      {
        Silicon_Detector_Code_part.push_back(Det.first);
        Silicon_Detector_Energy_Deposit_part.push_back(Det.second.EnergyDeposit);
        Silicon_Detector_Hit_Position_part.push_back(Det.second.HitPosition);
        Silicon_Detector_Hit_Angle_part.push_back(Det.second.HitAngle);
      }
      // DL
      else if (Det.first < 1000)
      {
        Silicon_Detector_Code_part.push_back(Det.first);
        Silicon_Detector_DL_Energy_Deposit_part.push_back(Det.second.EnergyDeposit);
      }
      // InterStrip
      for (auto interDet : particle.InterStripDetectors)
      {
        Silicon_Detector_InterStrip_Code_part.push_back(interDet.first);
        Silicon_Detector_InterStrip_Energy_Deposit_part.push_back(interDet.second.EnergyDeposit);
        Silicon_Detector_InterStrip_Hit_Position_part.push_back(interDet.second.HitPosition);
      }
    }

    Silicon_Detector_Code.push_back(Silicon_Detector_Code_part);
    Silicon_Detector_Energy_Deposit.push_back(Silicon_Detector_Energy_Deposit_part);
    Silicon_Detector_DL_Energy_Deposit.push_back(Silicon_Detector_DL_Energy_Deposit_part);
    Silicon_Detector_Hit_Position.push_back(Silicon_Detector_Hit_Position_part);
    Silicon_Detector_Hit_Angle.push_back(Silicon_Detector_Hit_Angle_part);

    Silicon_Detector_InterStrip_Code.push_back(Silicon_Detector_InterStrip_Code_part);
    Silicon_Detector_InterStrip_Energy_Deposit.push_back(Silicon_Detector_InterStrip_Energy_Deposit_part);
    Silicon_Detector_InterStrip_Hit_Position.push_back(Silicon_Detector_InterStrip_Hit_Position_part);

    Silicon_Detector_Code_part.clear();
    Silicon_Detector_Energy_Deposit_part.clear();
    Silicon_Detector_DL_Energy_Deposit_part.clear();
    Silicon_Detector_Hit_Position_part.clear();
    Silicon_Detector_Hit_Angle_part.clear();

    Silicon_Detector_InterStrip_Code_part.clear();
    Silicon_Detector_InterStrip_Energy_Deposit_part.clear();
    Silicon_Detector_InterStrip_Hit_Position_part.clear();
  }
  Tree->Fill();
  // PartInfo->Parse();



  /// HISTOGRAMS ///
  //  Init
  for (auto &pair : PartInfo->GetInfo())
  {
    Particle particle = pair.second;
    G4int PDG = particle.Particle_PDG;

    if (PDG > 1000000000)
    {
      PDG = (PDG / 10) * 10;
    }

    if (H_E0.find(PDG) == H_E0.end())
    {
      if (PDG > 1000000000)
      {
        H_E0[PDG] = new TH1D(("E0_" + G4ParticleTable::GetParticleTable()->FindParticle(PDG)->GetParticleName()).c_str(), ("E0_" + G4ParticleTable::GetParticleTable()->FindParticle(PDG)->GetParticleName()).c_str(), 1000, 0.0, 1000);
      }
      else
        H_E0[PDG] = new TH1D(("E0_" + G4ParticleTable::GetParticleTable()->FindParticle(PDG)->GetParticleName()).c_str(), ("E0_" + G4ParticleTable::GetParticleTable()->FindParticle(PDG)->GetParticleName()).c_str(), 10000, 0.0, 10000);
    }
    H_E0[PDG]->Fill(particle.E0);
  }

  int proton_index = -1;
  int positron_index = -1;
  for (auto& pair : PartInfo->GetInfo())
  {
    if (pair.second.Particle_PDG == 2212 && proton_index == -1)
    {
      proton_index = pair.first;
      break;
    }
    if ((pair.second.Particle_PDG == 11 || pair.second.Particle_PDG == -11) && positron_index == -1)
    {
      positron_index = pair.first;
    }
  }


  if (proton_index != -1 && positron_index != -1)
  {
    for (int i = 0; i < nb_det; i++)
    {
      Detector Det = (PartInfo->GetInfo()[proton_index]).Detectors[Detector_Code[i]];
      if (!Det.EnergyDeposit)
        continue;
      
      if (PartInfo->GetInfo()[positron_index].Detectors[99].EnergyDeposit >= GetThreshold() / keV)
      {
        silicon_coinc[i]->Fill(Det.EnergyDeposit);
        plastic_coinc->Fill((PartInfo->GetInfo()[positron_index]).Detectors[99].EnergyDeposit);
      }
      else
      {
        silicon_nocoinc[i]->Fill(Det.EnergyDeposit);
      }
      silicon_single[i]->Fill(Det.EnergyDeposit);
    }
  }

  int divi = 1000;

  ///// Writing in file ///////////////////////////////////////////
  if (event->GetEventID() % divi == 0)
  {
    Tree->AutoSave("FlushBaskets");

    for (int i = 0; i < nb_det; i++)
    {
      // cout << Detector_Name[i] << " : " << silicon_coinc[i]->GetEntries() << " " << silicon_nocoinc[i]->GetEntries() << " " << silicon_single[i]->GetEntries() << endl;
      silicon_coinc[i]->Write("", TObject::kOverwrite);
      silicon_nocoinc[i]->Write("", TObject::kOverwrite);
      silicon_single[i]->Write("", TObject::kOverwrite);
    }
    plastic_coinc->Write("", TObject::kOverwrite);

    for (auto pair : H_E0)
    {
      pair.second->Write("", TObject::kOverwrite);
    }
  }

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
  Silicon_Detector_InterStrip_Code.clear();
  Silicon_Detector_InterStrip_Energy_Deposit.clear();
  Silicon_Detector_InterStrip_Hit_Position.clear();
  PlasticScintillator_Energy_Deposit.clear();
  PlasticScintillator_Hit_Angle.clear();
  PlasticScintillator_Hit_Position.clear();
  Catcher_Central_Energy_Deposit.clear();
  Catcher_Side_Energy_Deposit.clear();
  ///////////////////////////////////////////////////////////////////
  PartInfo->Clear();
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