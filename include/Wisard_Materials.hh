#ifndef WISARD_MATERIALS_HH
#define WISARD_MATERIALS_HH

class G4Material;
class G4VisAttributes;

// Provides WISArD material definitions and visualization attributes.
// Geant4 owns the G4Material instances in its global stores.
class Wisard_Materials final
{
public:
  Wisard_Materials();

  G4Material* GetVacuum() const noexcept { return fVacuum; }
  G4Material* GetSilicon() const noexcept { return fSilicon; }
  G4Material* GetSiliconDioxide() const noexcept { return fSiliconDioxide; }

  G4Material* GetAluminium() const noexcept { return fAluminium; }
  G4Material* GetMylar() const noexcept { return fMylar; }
  G4Material* GetCopper() const noexcept { return fCopper; }
  G4Material* GetIron() const noexcept { return fIron; }

  G4Material* GetPlasticScintillator() const noexcept
  {
    return fPlasticScintillator;
  }
  G4Material* GetTeflon() const noexcept { return fTeflon; }
  G4Material* GetPEEK() const noexcept { return fPEEK; }
  G4Material* GetPCB() const noexcept { return fPCB; }

  G4VisAttributes* GetVacuumVisAttributes() const noexcept { return fVisVacuum; }
  G4VisAttributes* GetSiliconVisAttributes() const noexcept { return fVisSilicon; }
  G4VisAttributes* GetSiliconDioxideVisAttributes() const noexcept
  {
    return fVisSiliconDioxide;
  }
  G4VisAttributes* GetAluminiumVisAttributes() const noexcept
  {
    return fVisAluminium;
  }
  G4VisAttributes* GetMylarVisAttributes() const noexcept { return fVisMylar; }
  G4VisAttributes* GetCopperVisAttributes() const noexcept { return fVisCopper; }
  G4VisAttributes* GetIronVisAttributes() const noexcept { return fVisIron; }
  G4VisAttributes* GetPlasticScintillatorVisAttributes() const noexcept
  {
    return fVisPlasticScintillator;
  }
  G4VisAttributes* GetTeflonVisAttributes() const noexcept { return fVisTeflon; }
  G4VisAttributes* GetPEEKVisAttributes() const noexcept { return fVisPEEK; }
  G4VisAttributes* GetPCBVisAttributes() const noexcept { return fVisPCB; }
  G4VisAttributes* GetWorldVisAttributes() const noexcept { return fVisWorld; }
  G4VisAttributes* GetKillerVisAttributes() const noexcept { return fVisKiller; }

  G4VisAttributes* CreateMagnetVisAttributes(bool visible) const;
  G4VisAttributes* CreateBoreVisAttributes(bool visible) const;
  G4VisAttributes* CreateBeamPipeVisAttributes(bool visible) const;

private:
  static G4Material* BuildVacuum();
  static G4Material* BuildSilicon();
  static G4Material* BuildSiliconDioxide();
  static G4Material* BuildPlasticScintillator();
  static G4Material* BuildTeflon();
  static G4Material* BuildPEEK();
  static G4Material* BuildPCB();

  G4Material* fVacuum = nullptr;
  G4Material* fSilicon = nullptr;
  G4Material* fSiliconDioxide = nullptr;
  G4Material* fAluminium = nullptr;
  G4Material* fMylar = nullptr;
  G4Material* fCopper = nullptr;
  G4Material* fIron = nullptr;
  G4Material* fPlasticScintillator = nullptr;
  G4Material* fTeflon = nullptr;
  G4Material* fPEEK = nullptr;
  G4Material* fPCB = nullptr;

  G4VisAttributes* fVisVacuum = nullptr;
  G4VisAttributes* fVisSilicon = nullptr;
  G4VisAttributes* fVisSiliconDioxide = nullptr;
  G4VisAttributes* fVisAluminium = nullptr;
  G4VisAttributes* fVisMylar = nullptr;
  G4VisAttributes* fVisCopper = nullptr;
  G4VisAttributes* fVisIron = nullptr;
  G4VisAttributes* fVisPlasticScintillator = nullptr;
  G4VisAttributes* fVisTeflon = nullptr;
  G4VisAttributes* fVisPEEK = nullptr;
  G4VisAttributes* fVisPCB = nullptr;
  G4VisAttributes* fVisWorld = nullptr;
  G4VisAttributes* fVisKiller = nullptr;
};

#endif
