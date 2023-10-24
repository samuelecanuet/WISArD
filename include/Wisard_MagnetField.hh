#include "globals.hh"
#include "../Wisard_Global.hh"
#include "G4MagneticField.hh"
#include "G4ios.hh"

#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

class WisardMagnetField
    : public G4MagneticField
{
  
public:
  WisardMagnetField(const string filename, G4double value);
  std::vector<double> dataArray[13];
  int nz;
  double max, min;

  void GetFieldValue(const double Point[3],
                     double *Bfield) const;
};
