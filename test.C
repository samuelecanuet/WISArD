#include <iostream>
#include "TFile.h"
#include "TH1D.h"

int test()
{
    TFile *f = new TFile("test0T.root", "READ");
    for (int det=1; det<=8; det++)
    {
        for (int strip = 1; strip <= 5; strip ++)
        {
            TH1D *H = (TH1D*)f->Get(("D" + to_string(det) + "." + to_string(strip) + "_single").c_str());
            if (H)
            {
                cout << "D" + to_string(det) + "." + to_string(strip) + "_single" << "\t" << H->GetEntries() << endl;
            }
        }
    }
}