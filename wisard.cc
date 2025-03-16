#include "Wisard_Detector.hh"
#include "Wisard_PhysList.hh"
#include "Wisard_Generator.hh"
#include "Wisard_Tracking.hh"
#include "Wisard_ActionInitialization.hh"
#include "ParticleInformation.hh"

#include <cstdlib>
#include <iostream>
#include <sys/stat.h>

#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4MTRunManager.hh"

#include "TSystem.h"
#include "TInterpreter.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1D.h"
#include "TFile.h"
#include "TKey.h"

//----------------------------------------------------------------------
// program main function

int main(int argc, char **argv)
{
  system("Logo/logo.ans");
  // G4Random::setTheSeed(123456789);

  // get run time
  clock_t t1, t2;
  t1 = clock();

  //------------------------------------------------------------

  G4cout << "Generating Dictionnaries" << G4endl;

  G4String clheppath;
  std::string paths[] = {
      "/softs/clhep/2.4.6.2/include/",
      "/admin/local/clhep/2.4.6.2/include/"};

  for (const auto &path : paths)
  {
    struct stat info;
    if ((stat(path.c_str(), &info) == 0 && S_ISDIR(info.st_mode)))
    {
      clheppath = path;
    }
  }

  std::string clhepVectorPath = clheppath + "/CLHEP/Vector/";
  gSystem->AddIncludePath(("-I" + clheppath).c_str());
  gSystem->AddIncludePath(("-I" + clhepVectorPath).c_str());

  gInterpreter->GenerateDictionary("vector<vector<int> >", "vector");
  gInterpreter->GenerateDictionary("vector<vector<vector<double> > >", "vector");
  gInterpreter->GenerateDictionary("vector<CLHEP::Hep3Vector>", "vector;ThreeVector.h");
  gInterpreter->GenerateDictionary("vector<vector<CLHEP::Hep3Vector>>", "vector;ThreeVector.h");
  gInterpreter->GenerateDictionary("vector<vector<vector<CLHEP::Hep3Vector>>>", "vector;ThreeVector.h");

  //------------------------------------------------------------

  G4int THREAD = 3;
  G4String macroName;
  if (argc > 1)
  {
    macroName = argv[1];
  }
  else
  {
    macroName = "visu.mac";
  }
  std::ifstream macrofile(macroName);
  std::string line;
  while (std::getline(macrofile, line))
  {
    if (line.find("/Run/Threads") != std::string::npos)
    {
      if (line.find("#") == std::string::npos)
      {
        THREAD = std::stoi(line.substr(line.find(" ") + 1));
      }
    }
  }

  G4MTRunManager *ptr_run = new G4MTRunManager();
  ptr_run->SetNumberOfThreads(THREAD);

  Wisard_Detector *ptr_det = new Wisard_Detector();
  ptr_run->SetUserInitialization(ptr_det);

  Wisard_PhysList *ptr_phys = new Wisard_PhysList;
  ptr_run->SetUserInitialization(ptr_phys);

  Wisard_ActionInitialization *ptr_act = new Wisard_ActionInitialization(macroName);
  ptr_run->SetUserInitialization(ptr_act);

  ptr_run->Initialize();

  //------------------------------------------------------------

  G4VisManager *visu_manager = new G4VisExecutive("0");
  visu_manager->Initialize();

  G4UImanager *UI = G4UImanager::GetUIpointer();

  if (argc > 1)
  {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];

    // read the macro file and cout if line begin by /Input/CRADLE or /Input/Ion without the # to see the mode
    std::ifstream file(fileName);
    G4cout << G4endl;
    while (std::getline(file, line))
    {
      if (line.find("/Input/CRADLE") != std::string::npos || line.find("/Input/Ion") != std::string::npos || line.find("/Input/Particle") != std::string::npos)
      {
        if (line.find("#") == std::string::npos)
        {
          G4cout << "\033[34m" << line << "\033[0m" << G4endl;
        }
      }
    }

    G4cout << "\033[34m" << "Number of Threads used : " << THREAD << "\033[0m" << G4endl;
    G4cout << "\033[34m" << "Reading macro file: " << fileName << "\033[0m" << G4endl;
    G4cout << G4endl;
    UI->ApplyCommand("/control/alias currentMacro " + fileName);
    UI->ApplyCommand(command + fileName);
  }
  else
  // start of an interactive session
  {
    G4cout << "Starting a tcsh interactive session" << G4endl;
    G4UIExecutive *session = new G4UIExecutive(argc, argv);

    // start default viewer from macro
    UI->ApplyCommand("/control/execute visu.mac");
    G4cout << "--" << G4endl
           << "Use the following command for a display test" << G4endl
           << "->        /control/execute visu.mac" << G4endl
           << "--" << G4endl;

    // start the interactive session
    session->SessionStart();

    //  	G4cout << "End of interactive session" << G4endl << G4endl;
    delete session;
  }

  //------------------------------------------------------------

  // merge the ROOT files

  G4String outputFile = ptr_act->GetFileName().substr(0, ptr_act->GetFileName().length() - 5);

  TChain chain("Tree");
  map<G4String, TH1D *> H = {};
  map<G4String, TObject *> O = {};
  vector<TFile*> f(THREAD, nullptr);
  for (int i = 0; i < THREAD; i++)
  {
    G4String filename = outputFile + "_" + to_string(i) + ".root";
    if (!gSystem->AccessPathName(filename))
    {
      f[THREAD] = new TFile(filename.c_str(), "READ");
      if (!f[THREAD]->IsZombie())
      {
        chain.Add(filename);

        // loop on all the TH1D of the file
        TList *list = f[THREAD]->GetListOfKeys();
        TIter next(list);
        TKey *key;
        while ((key = (TKey *)next()))
        {
          TObject *obj = key->ReadObj();
          if (obj->InheritsFrom("TH1D"))
          {
            TH1D *h = (TH1D *)obj;
            if (H.find(h->GetName()) == H.end())
            {
              H[h->GetName()] = (TH1D*)h->Clone();
            }
            else
            {
              H[h->GetName()]->Add(h);
            }
          }
          if (obj->InheritsFrom("TObjString"))
          {
            O[obj->GetName()] = (TObjString*)obj->Clone();
          }
        }
      }
    }
  }
  outputFile = outputFile + ".root";  
  chain.Merge(outputFile);
  TFile final(outputFile, "UPDATE");
  for (auto &o : O)
  {
    o.second->Write();
  }
  for (auto &h : H)
  {
    h.second->Write();
  }
  final.Close();

  // Clean up temporary files
  for (int i = -1; i < THREAD; ++i)
  {
    std::string removeCommand = "/control/shell rm -f " + outputFile.substr(0, ptr_act->GetFileName().length() - 5) + "_" + std::to_string(i) + ".root";
    UI->ApplyCommand(removeCommand);
  }

  t2 = clock();
  float diff((float)t2 - (float)t1);
  G4cout << " " << G4endl;
  G4cout << "<I>: Run time: " << diff / 1.e6 << " s" << G4endl;
  G4cout << " " << G4endl;

  delete visu_manager;

  return (0);
}