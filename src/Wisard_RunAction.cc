#include "Wisard_RunAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

atomic<int> Wisard_RunAction::fAcceptedEvents{0};

Wisard_RunAction::Wisard_RunAction(G4String macrofilename) : G4UserRunAction(), MacroFileName(macrofilename)
{
  G4cout << "\033[32m" << "Constructor Wisard_RunAction" << "\033[0m" << G4endl;

  RunMessenger = new G4GenericMessenger(this, "/Run/", "Output Settings");

  RunMessenger->DeclareProperty("File", filename)
      .SetGuidance("Set ROOT Output filename")
      .SetParameterName("Filename", false)
      .SetDefaultValue("output.root");

  RunMessenger->DeclareProperty("Threads", NumberThreads)
      .SetGuidance("Taking value to avoid error")
      .SetParameterName("Thread", false)
      .SetDefaultValue("1");

  RunMessenger->DeclarePropertyWithUnit("Threshold_PlasticScintillator", "keV", threshold)
      .SetGuidance("Set the threshold for the detectors")
      .SetParameterName("Threshold", false)
      .SetDefaultValue("100.");
}
Wisard_RunAction::~Wisard_RunAction() {}

void Wisard_RunAction::BeginOfRunAction(const G4Run *)
{
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  NumberThreads_int = stoi(NumberThreads);
  NumberEvents_int = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();

  G4String fn = filename.substr(0, filename.length() - 5);
  G4int thread = G4Threading::G4GetThreadId();

  f = new TFile((fn + "_" + to_string(thread) + ".root").c_str(), "RECREATE");
  ////////////// Construct Log /////////////////////////////////////
  std::ifstream macrofile(MacroFileName);
  std::string line;
  while (std::getline(macrofile, line))
  {
    if ((line.find("/Geometry") != std::string::npos || line.find("/Input") != std::string::npos || line.find("/Run") != std::string::npos || line.find("/Beam") != std::string::npos) && line.find("#") == std::string::npos)
    {
      char const *num_char = line.c_str();
      TObjString objString(line.c_str());
      f->WriteObject(&objString, num_char);
    }
    f->cd();
  }

  ////////////// Construct Tree ///////////////////////////////////
  Tree = new TTree("Tree", "Informations");
  Tree->Branch("EventID", &EventID, "EventID/I");
  Tree->Branch("Particle_PDG", &Particle_PDG);
  Tree->Branch("x", &x);
  Tree->Branch("y", &y);
  Tree->Branch("z", &z);
  Tree->Branch("px", &px);
  Tree->Branch("py", &py);
  Tree->Branch("pz", &pz);
  Tree->Branch("T0", &T0);
  Tree->Branch("Kinetic_Energy", &Kinetic_Energy);
  Tree->Branch("Catcher_Central_Energy_Deposit", &Catcher_Central_Energy_Deposit);
  Tree->Branch("Catcher_Side_Energy_Deposit", &Catcher_Side_Energy_Deposit);
  Tree->Branch("Catcher_Side_BackScattering", &Catcher_Side_BackScattering);
  Tree->Branch("Catcher_Support_BackScattering", &Catcher_Support_BackScattering);
  Tree->Branch("EntranceCollimator_BackScattering", &EntranceCollimator_BackScattering);
  Tree->Branch("PlasticScintillator_Energy_Deposit", &PlasticScintillator_Energy_Deposit);
  Tree->Branch("PlasticScintillator_Visible_Energy_Deposit", &PlasticScintillator_Visible_Energy_Deposit);
  Tree->Branch("PlasticScintillator_Hit_Position", &PlasticScintillator_Hit_Position);
  Tree->Branch("PlasticScintillator_Hit_Angle", &PlasticScintillator_Hit_Angle);
  Tree->Branch("PlasticScintillator_Hit_Time", &PlasticScintillator_Hit_Time);
  Tree->Branch("PlasticScintillator_BackScattering", &PlasticScintillator_BackScattering);
  Tree->Branch("Silicon_Detector_Energy_Deposit", &Silicon_Detector_Energy_Deposit);
  Tree->Branch("Silicon_Detector_Hit_Position", &Silicon_Detector_Hit_Position);
  Tree->Branch("Silicon_Detector_Hit_DistanceBoundary", &Silicon_Detector_Hit_DistanceBoundary);
  Tree->Branch("Silicon_Detector_Hit_Angle", &Silicon_Detector_Hit_Angle);
  Tree->Branch("Silicon_Detector_Hit_Time", &Silicon_Detector_Hit_Time);
  Tree->Branch("Silicon_Detector_Code", &Silicon_Detector_Code);
  Tree->Branch("Silicon_Detector_DL_Energy_Deposit", &Silicon_Detector_DL_Energy_Deposit);

  ////////////// Construct Histograms ////////////////////////////
  for (int i = 0; i < Wisard_Detector::nb_det; i++)
  {
    G4String name = Detector_Name[i];
    G4int detector_code = Detector_Code[i];
    silicon_coinc[detector_code] = new TH1D((name + "_coinc").c_str(), (name + "_coinc").c_str(), 100000, 0.0, 10000.0);
    silicon_nocoinc[detector_code] = new TH1D((name + "_nocoinc").c_str(), (name + "_nocoinc").c_str(), 100000, 0.0, 10000.0);
    silicon_single[detector_code] = new TH1D((name + "_single").c_str(), (name + "_single").c_str(), 100000, 0.0, 10000.0);
  }
  plastic_coinc = new TH1D("plastic_coinc", "plastic_coinc", 120000, 0.0, 12000.0);
}

void Wisard_RunAction::EndOfRunAction(const G4Run *)
{
  WrittingTree();
  if (Tree_MCP != nullptr)
  {
    Tree_MCP->Write();
    H_MCP->Write();
  }
  f->Close();
}

void Wisard_RunAction::UpdateTree(ParticleInformation *Part_Info, G4int event_id)
{
  // Part_Info->Parse();

  EventID = event_id;

  // TREE VARIABLES //
  
  for (auto &pair : Part_Info->GetInfo())
  {
    Particle particle = pair.second;

    Particle_PDG.push_back(particle.Particle_PDG);
    x.push_back(particle.Pos.x());
    y.push_back(particle.Pos.y());
    z.push_back(particle.Pos.z());
    px.push_back(particle.Dir.x());
    py.push_back(particle.Dir.y());
    pz.push_back(particle.Dir.z());
    T0.push_back(particle.Time);
    Kinetic_Energy.push_back(particle.E0);

    // # Catcher Central #//
    // G4double catcher_central = 0;
    // if (particle.Detectors.find(1) != particle.Detectors.end())
    //   catcher_central += particle.Detectors[1].EnergyDeposit;
    // if (particle.Detectors.find(2) != particle.Detectors.end())
    //   catcher_central += particle.Detectors[2].EnergyDeposit;
    // if (particle.Detectors.find(3) != particle.Detectors.end())
    //   catcher_central += particle.Detectors[3].EnergyDeposit;
    // Catcher_Central_Energy_Deposit.push_back(catcher_central);

    // # Catcher Side #//
    G4double catcher_side = 0;
    G4bool catcher_side_backscattering = false;
    if (particle.Detectors.find(4) != particle.Detectors.end())
    {
      catcher_side += particle.Detectors[4].EnergyDeposit;
      catcher_side_backscattering = particle.Detectors[4].BackScattering;
    }
    if (particle.Detectors.find(5) != particle.Detectors.end())
    {
      catcher_side += particle.Detectors[5].EnergyDeposit;
      catcher_side_backscattering = catcher_side_backscattering == true ? catcher_side_backscattering : particle.Detectors[5].BackScattering;
    }
    if (particle.Detectors.find(6) != particle.Detectors.end())
    {
      catcher_side += particle.Detectors[6].EnergyDeposit;
      catcher_side_backscattering = catcher_side_backscattering == true ? catcher_side_backscattering : particle.Detectors[6].BackScattering;
    }
    Catcher_Side_Energy_Deposit.push_back(catcher_side);
    Catcher_Side_BackScattering.push_back(catcher_side_backscattering);

    // # Collimator Entrance #//
    bool collimator_backscattering = false;
    if (particle.Detectors.find(100) != particle.Detectors.end())
      collimator_backscattering = particle.Detectors[100].BackScattering;
    EntranceCollimator_BackScattering.push_back(collimator_backscattering);

    // # Catcher Support #//
    bool catcher_support_backscattering = false;
    if (particle.Detectors.find(7) != particle.Detectors.end())
      catcher_support_backscattering = particle.Detectors[7].BackScattering;
    if (particle.Detectors.find(8) != particle.Detectors.end())
      catcher_support_backscattering = catcher_support_backscattering == true ? catcher_support_backscattering : particle.Detectors[8].BackScattering;
    Catcher_Support_BackScattering.push_back(catcher_support_backscattering);

    // # Plastic Scintillator #//
    PlasticScintillator_Energy_Deposit.push_back(particle.Detectors[99].EnergyDeposit);
    plastic_coinc->Fill(particle.Detectors[99].EnergyDeposit);
    PlasticScintillator_Visible_Energy_Deposit.push_back(particle.Detectors[99].VisibleEnergyDeposit);
    PlasticScintillator_Hit_Position.push_back(particle.Detectors[99].HitPosition);
    PlasticScintillator_Hit_Angle.push_back(particle.Detectors[99].HitAngle);
    PlasticScintillator_Hit_Time.push_back(particle.Detectors[99].HitTime);
    PlasticScintillator_BackScattering.push_back(particle.Detectors[99].BackScattering);

    // # Silicon Detectors #//
    for (auto Det : particle.Detectors)
    {
      // Strip
      if (Det.first < 87 && Det.first > 10)
      {
        Silicon_Detector_Code_part.push_back(Det.first);
        silicon_single[Det.first]->Fill(Det.second.EnergyDeposit);
        Silicon_Detector_Energy_Deposit_part.push_back(Det.second.EnergyDeposit);
        Silicon_Detector_Hit_Position_part.push_back(Det.second.HitPosition);
        Silicon_Detector_Hit_DistanceBoundary_part.push_back(Det.second.DistanceBoundary);
        Silicon_Detector_Hit_Angle_part.push_back(Det.second.HitAngle);
        Silicon_Detector_Hit_Time_part.push_back(Det.second.HitTime);
      }
      // DL
      else if (Det.first < 1000 && Det.first > 100)
      {
        Silicon_Detector_Code_part.push_back(Det.first);
        Silicon_Detector_DL_Energy_Deposit_part.push_back(Det.second.EnergyDeposit);
      }
    }

    Silicon_Detector_Code.push_back(Silicon_Detector_Code_part);
    Silicon_Detector_Energy_Deposit.push_back(Silicon_Detector_Energy_Deposit_part);
    Silicon_Detector_DL_Energy_Deposit.push_back(Silicon_Detector_DL_Energy_Deposit_part);
    Silicon_Detector_Hit_Position.push_back(Silicon_Detector_Hit_Position_part);
    Silicon_Detector_Hit_DistanceBoundary.push_back(Silicon_Detector_Hit_DistanceBoundary_part);
    Silicon_Detector_Hit_Angle.push_back(Silicon_Detector_Hit_Angle_part);
    Silicon_Detector_Hit_Time.push_back(Silicon_Detector_Hit_Time_part);

    Silicon_Detector_Code_part.clear();
    Silicon_Detector_Energy_Deposit_part.clear();
    Silicon_Detector_DL_Energy_Deposit_part.clear();
    Silicon_Detector_Hit_Position_part.clear();
    Silicon_Detector_Hit_DistanceBoundary_part.clear();
    Silicon_Detector_Hit_Angle_part.clear();
    Silicon_Detector_Hit_Time_part.clear();
  }
  Tree->Fill();

  Particle_PDG.clear();
  x.clear();
  y.clear();
  z.clear();
  px.clear();
  py.clear();
  pz.clear();
  T0.clear();
  Kinetic_Energy.clear();
  Silicon_Detector_Code.clear();
  Silicon_Detector_Energy_Deposit.clear();
  Silicon_Detector_DL_Energy_Deposit.clear();
  Silicon_Detector_Hit_Position.clear();
  Silicon_Detector_Hit_DistanceBoundary.clear();
  Silicon_Detector_Hit_Angle.clear();
  Silicon_Detector_Hit_Time.clear();
  PlasticScintillator_Energy_Deposit.clear();
  PlasticScintillator_Visible_Energy_Deposit.clear();
  PlasticScintillator_Hit_Angle.clear();
  PlasticScintillator_Hit_Position.clear();
  PlasticScintillator_Hit_Time.clear();
  PlasticScintillator_BackScattering.clear();
  Catcher_Central_Energy_Deposit.clear();
  Catcher_Side_Energy_Deposit.clear();
  Catcher_Side_BackScattering.clear();
  EntranceCollimator_BackScattering.clear();
  Catcher_Support_BackScattering.clear(); 

  /// HISTOGRAMS ///
  //  Init
  /*
  for (auto &pair : Part_Info->GetInfo())
  {
    Particle particle = pair.second;
    G4int PDG = particle.Particle_PDG;

    if (PDG > 1000000000)
    {
      PDG = (PDG / 10) * 10;
    }

    if (H_E0.find(PDG) == H_E0.end())
    {
      G4String name = G4ParticleTable::GetParticleTable()->FindParticle(PDG)->GetParticleName();
      if (PDG > 1000000000)
      {
        H_E0[PDG] = new TH1D(("E0_" + name).c_str(), ("E0_" + name).c_str(), 1000, 0.0, 1000);
      }
      else
        H_E0[PDG] = new TH1D(("E0_" + name).c_str(), ("E0_" + name).c_str(), 10000, 0.0, 10000);
    }
    H_E0[PDG]->Fill(particle.E0);
  }

  // Detectors
  int proton_index = -1;
  int positron_index = -1;
  for (auto &pair : Part_Info->GetInfo())
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

  // if (proton_index != -1 && positron_index != -1)
  // {

  
  for (int i = 0; i < Wisard_Detector::nb_det; i++)
  {
    Detector Det = (Part_Info->GetInfo()[proton_index]).Detectors[Detector_Code[i]];
    if (!Det.EnergyDeposit)
      continue;

    if (Part_Info->GetInfo()[positron_index].Detectors[99].EnergyDeposit >= threshold)
    {
      silicon_coinc[i]->Fill(Det.EnergyDeposit);
      plastic_coinc->Fill((Part_Info->GetInfo()[positron_index]).Detectors[99].EnergyDeposit);
    }
    else
    {
      silicon_nocoinc[i]->Fill(Det.EnergyDeposit);
    }
    silicon_single[i]->Fill(Det.EnergyDeposit);
  }
  // }
  */  

  

  G4int divi = 100000;
  G4int EventProcessed = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  if (EventProcessed % divi == 0)
  {
    WrittingTree();
  }
}

void Wisard_RunAction::WrittingTree()
{
  // G4cout << "Writing Tree" << G4endl; 
  f->cd();

  Tree->AutoSave("FlushBaskets");

  for (int i = 0; i < Wisard_Detector::nb_det; i++)
  {
    G4int detector_code = Detector_Code[i];
    // G4cout << "Writing Histograms for " << Detector_Name[i] << "  " << silicon_single[detector_code]->GetEntries() << G4endl;
    // G4cout<< Detector_Name[i] << " : " << silicon_coinc[detector_code]->GetEntries() << " " << silicon_nocoinc[detector_code]->GetEntries() << " " << silicon_single[i]->GetEntries() <<G4endl;
    silicon_coinc[detector_code]->Write("", TObject::kOverwrite);
    silicon_nocoinc[detector_code]->Write("", TObject::kOverwrite);
    silicon_single[detector_code]->Write("", TObject::kOverwrite);
  }
  plastic_coinc->Write("", TObject::kOverwrite);

  for (auto pair : H_E0)
  {
    pair.second->Write("", TObject::kOverwrite);
  }
}

G4String Wisard_RunAction::GetFileName()
{
  return filename;
}

void Wisard_RunAction::FillMCP(G4double x_tree, G4double y_tree)
{

  if (Tree_MCP == nullptr)
  {
    Tree_MCP = new TTree("Tree_MCP", "MCP Positions");
    Tree_MCP->Branch("x_MCP", &x_MCP, "x/D");
    Tree_MCP->Branch("y_MCP", &y_MCP, "y/D");

    H_MCP = new TH2D("H_MCP", "MCP Positions", 1000, -10 * mm, 10 * mm, 1000, -10 * mm, 10 * mm);
    H_MCP->SetXTitle("x (mm)");
    H_MCP->SetYTitle("y (mm)");
  }

  x_MCP = x_tree;
  y_MCP = y_tree;
  Tree_MCP->Fill();
  H_MCP->Fill(x_tree, y_tree);  
}


G4int Wisard_RunAction::GetNumberOfThreads()
{
    return NumberThreads_int;
}

G4int Wisard_RunAction::GetNumberofEvents()
{
    return NumberEvents_int;
}