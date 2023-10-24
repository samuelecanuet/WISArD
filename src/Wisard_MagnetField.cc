#include "Wisard_MagnetField.hh"

WisardMagnetField::WisardMagnetField(const string filename, G4double value)
{
  ifstream file(filename); // Open the file for reading.

  // for the first line
  std::string header;
  std::getline(file, header);

  // Lire les données à partir du fichier
  double z_val, BZr1, BRr1, BZr2, BRr2, BZr3, BRr3, BZr4, BRr4, BZr5, BRr5;
  while (file >> z_val >> BZr1 >> BRr1 >> BZr2 >> BRr2 >> BZr3 >> BRr3 >> BZr4 >> BRr4 >> BZr5 >> BRr5)
  {
    dataArray[0].push_back(-z_val * mm);
    dataArray[1].push_back((BZr1 * tesla/4*value ));
    dataArray[2].push_back(0.0);
    dataArray[3].push_back(BZr1 * tesla/4*value );
    dataArray[4].push_back((BRr1 * tesla/4*value ));
    dataArray[5].push_back(BZr2 * tesla/4*value );
    dataArray[6].push_back((BRr2 * tesla/4*value ));
    dataArray[7].push_back(BZr3 * tesla/4*value );
    dataArray[8].push_back((BRr3 * tesla/4*value ));
    dataArray[9].push_back(BZr4 * tesla/4*value );
    dataArray[10].push_back((BRr4 * tesla/4*value ));
    dataArray[11].push_back(BZr5 * tesla/4*value );
    dataArray[12].push_back((BRr5 * tesla/4*value ));
  }

  for (int k = 0; k < 13; k++)
  {
    reverse(dataArray[k].begin(), dataArray[k].end());
  }
  // Fermer le fichier
  file.close();
  nz = dataArray[0].size();

  min = dataArray[0][0];
  max = dataArray[0][nz - 1];
}

void WisardMagnetField::GetFieldValue(const double point[4],
                                      double *Bfield) const
{
  double r = sqrt(pow(point[0], 2) * pow(point[1], 2));
  double z = point[2];

  double tab_r[6] = {0 * mm, 10 * mm, 28 * mm, 48 * mm, 68 * mm, 88 * mm};
  int index;
  double scale_r;

  for (index = 0; index < 6; index++)
  {
    if (r > tab_r[index])
    {
      scale_r = (r - tab_r[index]) / (tab_r[index] - tab_r[index + 1]);
      break;
    }
  }

  std::vector<double> zField = dataArray[index * 2 + 1];
  double rField1, rField0;

  // Check that the point is within the defined region
  if (z < min)
  {
    Bfield[0] = 0.0;
    Bfield[1] = 0.0;
    Bfield[2] = 0.0;
  }
  else if (z >= min && z <= max)
  {
    // find two grid point around point searched for

    if (index != 5)
    {
      for (int ix = 0; ix < nz; ix++)
      {
        if (dataArray[0][ix] > z)
        {
          rField1 = (dataArray[index * 2 + 2][ix] - dataArray[(index + 1) * 2 + 2][ix]) * scale_r + dataArray[index * 2 + 2][ix];
          rField0 = (dataArray[index * 2 + 2][ix - 1] - dataArray[(index + 1) * 2 + 2][ix - 1]) * scale_r + dataArray[index * 2 + 2][ix - 1];
          double Br = 1 / r * ((rField1 - rField0) / (dataArray[0][ix] - dataArray[0][ix - 1]) * (z - dataArray[0][ix - 1]) + rField0);
          Bfield[0] = (point[0]) * (Br);
          Bfield[1] = (point[1]) * (Br);
          Bfield[2] = (zField[ix] - zField[ix - 1]) / (dataArray[0][ix] - dataArray[0][ix - 1]) * (z - dataArray[0][ix - 1]) + zField[ix - 1];
          break;
        }
      }
    }
    else
    {
      for (int ix = 0; ix < nz; ix++)
      {
        if (dataArray[0][ix] > z)
        {
          rField1 = dataArray[index * 2 + 2][ix];
          rField0 = dataArray[index * 2 + 2][ix - 1];
          double Br = 1 / r * ((rField1 - rField0) / (dataArray[0][ix] - dataArray[0][ix - 1]) * (z - dataArray[0][ix - 1]) + rField0);
          Bfield[0] = (point[0]) * Br;
          Bfield[1] = (point[1]) * Br;
          Bfield[2] = (zField[ix] - zField[ix - 1]) / (dataArray[0][ix] - dataArray[0][ix - 1]) * (z - dataArray[0][ix - 1]) + zField[ix - 1];
          break;
        }
      }
    }
  }

  else
  {
    Bfield[0] = 0.0;
    Bfield[1] = 0.0;
    Bfield[2] = 0.0;
  }
}
