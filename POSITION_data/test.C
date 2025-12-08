using namespace std;

void test()
{
    ofstream outfile("Silicon_G4Vector.cfg");
    TVector3 p1, p2, p3, p4;
    // read file
    ifstream infile("Silicon_G4Position.cfg");
    if (!infile.is_open())
    {
        cerr << "Error opening file" << endl;
        return;
    }
    string line;
    

    for (int det = 1; det <= 8; ++det)
    {
        cout << "Plane " << det << ":\n";

        for (int i = 0; i < 4; ++i)
        {
            getline(infile, line);
            if (line.empty() || line[0] == '#')
            {
                cerr << "Error: not enough points in file" << endl;
                return;
            }
            istringstream iss(line);
            double x, y, z;
            if (!(iss >> x >> y >> z))
            {
                cerr << "Error reading line: " << line << endl;
                return;
            }

            cout << "Point " << i + 1 << ": (" << x << ", " << y << ", " << z << ")\n";

            if (i == 0)
                p1 = TVector3(x, y, z);
            else if (i == 1)
                p2 = TVector3(x, y, z);
            else if (i == 2)
                p3 = TVector3(x, y, z);
            else if (i == 3)
                p4 = TVector3(x, y, z);
        }

        // --- 1. Compute centroid
        TVector3 c = (p1 + p2 + p3 + p4) * 0.25;
        double R = c.Mag();

        // --- 2. Build scatter matrix
        TMatrixDSym S(3);
        auto accumulate = [&](const TVector3 &p)
        {
            TVector3 q = p - c;
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    S(i, j) += q[i] * q[j];
        };
        accumulate(p1);
        accumulate(p2);
        accumulate(p3);
        accumulate(p4);

        // --- 3. Eigen decomposition
        TMatrixDSymEigen eig(S);
        TVectorD eval = eig.GetEigenValues();
        TMatrixD evec = eig.GetEigenVectors();

        // --- 4. Smallest eigenvalue → plane normal
        int imin = TMath::LocMin(3, eval.GetMatrixArray());
        TVector3 normal(evec(0, imin), evec(1, imin), evec(2, imin));
        normal = normal.Unit();

        cout << "Centroid = (" << c.X() << ", " << c.Y() << ", " << c.Z() << ")\n";

        cout << "Plane normal = (" << normal.X() << ", " << normal.Y() << ", " << normal.Z() << ")\n";

        if (normal.Z() < 0 && det >= 5)
            normal = -normal;
        if (normal.Z() > 0 && det <= 4)
            normal = -normal;


        // --- 5. Write to output file
        if (det == 1)
            outfile << "D3";
        else if (det == 2)
            outfile << "D4";
        else if (det == 3)
            outfile << "D2";
        else if (det == 4)
            outfile << "D1";
        else if (det == 5)
            outfile << "D7";
        else if (det == 6)
            outfile << "D8";
        else if (det == 7)
            outfile << "D5";
        else if (det == 8)
            outfile << "D6";
        outfile << " " << c.X() << "\t\t" << c.Y() << "\t\t" << c.Z()  <<  "\t\t" 
                << remainder(atan2(normal.Y(), normal.Z()) * 180.0 / TMath::Pi(), 90.) << "\t\t"
                << remainder(atan2(normal.Z(), normal.X()) * 180.0 / TMath::Pi(), 90.) << "\t\t"
                << remainder(atan2(normal.X(), normal.Y()) * 180.0 / TMath::Pi(), 90.) << "\n";

        cout << "Angles with axes:\n";
        cout << "  X: " << normal.Angle(TVector3(1, 0, 0)) * 180.0 / TMath::Pi() << " deg\n";
        cout << "  Y: " << normal.Angle(TVector3(0, 1, 0)) * 180.0 / TMath::Pi() << " deg\n";
        cout << "  Z: " << normal.Angle(TVector3(0, 0, 1)) * 180.0 / TMath::Pi() << " deg\n";
        cout << "Angle with planes:\n";
        cout << "  XY: " << remainder(atan2(normal.X(), normal.Y()) * 180.0 / TMath::Pi(), 90.) << " deg\n";
        cout << "  YZ: " << remainder(atan2(normal.Y(), normal.Z()) * 180.0 / TMath::Pi(), 90.) << " deg\n";
        cout << "  ZX: " << remainder(atan2(normal.Z(), normal.X()) * 180.0 / TMath::Pi(), 90.) << " deg\n";
        cout << "  YX: " << remainder(atan2(normal.Y(), normal.X()) * 180.0 / TMath::Pi(), 90.) << " deg\n";
        cout << "  ZY: " << remainder(atan2(normal.Z(), normal.Y()) * 180.0 / TMath::Pi(), 90.) << " deg\n";
        cout << "  XZ: " << remainder(atan2(normal.X(), normal.Z()) * 180.0 / TMath::Pi(), 90.) << " deg\n";
    
        cout << "----------------------------------------\n";

    
    }

    infile.close();
    outfile.close();
}
