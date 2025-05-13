

double function1D_Bz(double *x, double *par)
{
    // gate convoluted with gaussian
    double A = par[0];
    double mu = par[1];
    double sigma = par[2];
    double l = par[3];
    double a = par[4];
    double b = par[5];

    if (x[0] > 350 || x[0] < -350)
    {
        return a / (1 + pow(-abs(x[0]), 5) / b);
    }

    return A * (erf((x[0] - mu + l) / (sqrt(2) * sigma)) - erf((x[0] - mu - l) / (sqrt(2) * sigma)));
}

double function1D_Br(double *x, double *p)
{
    double A = p[0];
    double mu = p[1];
    double sigma = p[2];
    double A1 = p[3];
    double mu1 = p[4];
    double sigma1 = p[5];
    double l = p[6];

    double first = A * exp(-pow((x[0] - mu), 2) / (2 * pow(sigma, 2)));
    double second = 0;

    if (-450 > x[0])
    {
        second = A1 * exp(-pow((x[0] - mu1), 2) / (2 * pow(sigma1, 2)));
    }

    return (first + second);
}

void Fitting_B()
{
    // read txt file of field data
    std::ifstream file("wisard_field_complete.txt");
    std::string line;

    // Create histograms
    TH2D *hBr = new TH2D("hBr", "Br vs z and r", 2529, -1851, 425, 5, 0, 88);

    TH2D *hBz = new TH2D("hBz", "Bz vs z and r", 2529, -1851, 425, 5, 0, 88);

    TGraph2D *gBr = new TGraph2D();
    gBr->SetName("gBr");
    gBr->SetTitle("Br vs z and r");
    gBr->GetXaxis()->SetTitle("z (mm)");
    gBr->GetYaxis()->SetTitle("r (mm)");
    gBr->GetZaxis()->SetTitle("Br (T)");
    TGraph2D *gBz = new TGraph2D();
    gBz->SetName("gBz");
    gBz->SetTitle("Bz vs z and r");
    gBz->GetXaxis()->SetTitle("z (mm)");
    gBz->GetYaxis()->SetTitle("r (mm)");
    gBz->GetZaxis()->SetTitle("Bz (T)");
    TGraph *gBr10 = new TGraph();
    TGraph *gBz10 = new TGraph();

    TFile *fileout = new TFile("Fitting_Br_Bz.root", "RECREATE");

    // Skip header line
    std::getline(file, line);

    int counter = 0;
    int counter_g = 0;
    int counter_g1D = 0;
    // Read data lines
    while (std::getline(file, line))
    {
        if (counter == 0)
        {
            counter++;
            continue;
        }

        std::istringstream iss(line);
        double z, Bz10, Br10, Bz28, Br28, Bz48, Br48, Bz68, Br68, Bz88, Br88;
        iss >> z >> Bz10 >> Br10 >> Bz28 >> Br28 >> Bz48 >> Br48 >> Bz68 >> Br68 >> Bz88 >> Br88;

        hBr->Fill(z, 10, Br10);
        hBr->Fill(z, 28, Br28);
        hBr->Fill(z, 48, Br48);
        hBr->Fill(z, 68, Br68);
        hBr->Fill(z, 88, Br88);

        hBz->Fill(z, 10, Bz10);
        hBz->Fill(z, 28, Bz28);
        hBz->Fill(z, 48, Bz48);
        hBz->Fill(z, 68, Bz68);
        hBz->Fill(z, 88, Bz88);

        gBr->SetPoint(counter_g, z, 10, Br10);
        gBr->SetPoint(counter_g + 1, z, 28, Br28);
        gBr->SetPoint(counter_g + 2, z, 48, Br48);
        gBr->SetPoint(counter_g + 3, z, 68, Br68);
        gBr->SetPoint(counter_g + 4, z, 88, Br88);

        gBz->SetPoint(counter_g, z, 10, Bz10);
        gBz->SetPoint(counter_g + 1, z, 28, Bz28);
        gBz->SetPoint(counter_g + 2, z, 48, Bz48);
        gBz->SetPoint(counter_g + 3, z, 68, Bz68);
        gBz->SetPoint(counter_g + 4, z, 88, Bz88);

        gBz10->SetPoint(counter_g1D, z, Bz10);
        gBr10->SetPoint(counter_g1D, z, abs(Br10));

        counter_g += 5;
        counter_g1D++;
    }

    // Bz
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);
    c1->Divide(2, 1);
    c1->cd(1);
    gBr10->Draw("AP");
    TF1 *f1Dr = new TF1("f1D", function1D_Br, -1000, 0, 6);
    f1Dr->SetParLimits(0, 0, 1);
    f1Dr->SetParameter(0, 0.1);
    f1Dr->SetParLimits(1, -400, -100);
    f1Dr->SetParameter(1, -350);
    f1Dr->SetParLimits(2, 0, 1000);
    f1Dr->SetParameter(2, 100);
    f1Dr->SetParLimits(3, 0, 1);
    f1Dr->SetParameter(3, 0.1);
    f1Dr->SetParLimits(4, -800, -300);
    f1Dr->SetParameter(4, -400);
    f1Dr->SetParLimits(5, 100, 1000);
    f1Dr->SetParameter(5, 150);
    // f1Dr->SetParLimits(6, -1000, 1000);
    // f1Dr->SetParameter(6, -400);
    gBr10->Fit(f1Dr, "R");

    // residus
    TGraph *gResidusr = new TGraph();
    for (int i = 0; i < gBr10->GetN(); i++)
    {
        double x, y;
        gBr10->GetPoint(i, x, y);
        gResidusr->SetPoint(i, x, y - f1Dr->Eval(x));
    }

    c1->cd(2);
    gResidusr->Draw("AP");
    c1->Write();

    // /// Br
    // TCanvas *c2 = new TCanvas("c2", "c2", 800, 800);
    // c2->Divide(2, 1);
    // c2->cd(1);
    // gBz10->Draw("AP");
    // TF1 *f1D = new TF1("f1D", function1D_Bz, -1000, 500, 6);
    // f1D->SetParLimits(0, 0, 9);
    // f1D->SetParameter(0, 2);
    // f1D->SetParLimits(1, -10, 10);
    // f1D->SetParameter(1, 0);
    // f1D->SetParLimits(2, 0, 1000);
    // f1D->SetParameter(2, 500);
    // f1D->SetParLimits(3, 0, 500);
    // f1D->SetParameter(3, 355);
    // f1D->SetParameter(4, -300000);
    // f1D->SetParameter(5, 3.86e7);
    // gBz10->Fit(f1D, "R");

    // // residus
    // TGraph *gResidus = new TGraph();
    // for (int i = 0; i < gBz10->GetN(); i++)
    // {
    //    double x, y;
    //     gBz10->GetPoint(i, x, y);
    //     gResidus->SetPoint(i, x, y - f1D->Eval(x));
    // }

    // c2->cd(2);
    // gResidus->Draw("AP");
    // c2->Draw();

    hBr->Write();
    hBz->Write();
    gBr->Write();
    gBz->Write();

    fileout->Close();
}
