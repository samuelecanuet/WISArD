#include "globals.hh"
#include "Wisard_Global.hh"
#include "G4MagneticField.hh"
#include "G4ios.hh"

#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

class WisardMagnetField
    // #ifndef STANDALONE
    : public G4MagneticField
// #endif
{

  int nz;
 G4double max, min;
 G4double xField[1001], yField[1001], zField[1001], zval[1001];

public:
  WisardMagnetField(const G4String filename, G4double value);

  void GetFieldValue(const G4double Point[3],
                    G4double *Bfield) const;
};
