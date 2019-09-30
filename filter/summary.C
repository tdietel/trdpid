void summary(TString file1, TString file2="", TString file3="", TString file4="", TString file5="") {

  TFile *f1 = new TFile(file1.Data());
  // TFile *f2 = new TFile(file2.Data());
  // TFile *f3 = new TFile(file3.Data());
  // TFile *f4 = new TFile(file4.Data());
  // TFile *f5 = new TFile(file5.Data());

  TCanvas *cnv = NULL;

  TList *lst;
  // TList *lst2;
  // TList *lst3;
  // TList *lst4;
  // TList *lst5;

  f1->GetObject("TRDdigitsFilter", lst);
  // f2->GetObject("TRDdigitsFilter", lst2);
  // f3->GetObject("TRDdigitsFilter", lst3);
  // f4->GetObject("TRDdigitsFilter", lst4);
  // f5->GetObject("TRDdigitsFilter", lst5);

  // --------------------------------------------------------------
  // statistics of available and selected events

  TH1 *hstat = (TH1 *)lst->FindObject("EventCuts");
  // TH1 *hstat2 = (TH1 *)lst2->FindObject("EventCuts");
  // TH1 *hstat3 = (TH1 *)lst3->FindObject("EventCuts");
  // TH1 *hstat4 = (TH1 *)lst4->FindObject("EventCuts");
  // TH1 *hstat5 = (TH1 *)lst5->FindObject("EventCuts");

  hstat->SetStats(0);

  hstat->SetTitle("Summary of Event Statistics;;Number of Events");
  
  // Lable all the bins in human readable wording
  hstat->GetXaxis()->SetBinLabel(1,"Incoming Events");
  hstat->GetXaxis()->SetBinLabel(2,"Incoming Events in ESDs");
  hstat->GetXaxis()->SetBinLabel(3,"Vertex Selection");
  hstat->GetXaxis()->SetBinLabel(4,"Accepted Events");
  hstat->GetXaxis()->SetBinLabel(5,"Centrality Selection");
  hstat->GetXaxis()->SetBinLabel(6,"High P_T Electrons");
  hstat->GetXaxis()->SetBinLabel(7,"Low P_T Electrons");
  hstat->GetXaxis()->SetBinLabel(8,"High P_T Pions");
  hstat->GetXaxis()->SetBinLabel(9,"Medium P_T Pions");
  hstat->GetXaxis()->SetBinLabel(10,"Low P_T Pions");
  hstat->GetXaxis()->SetBinLabel(11,"High P_T Protons");
  hstat->GetXaxis()->SetBinLabel(12,"Medium P_T Protons");
  hstat->GetXaxis()->SetBinLabel(13,"Low P_T Protons");
  
  //hstat->GetXaxis()->SetTitle("X axis title");
  //hstat->GetYaxis()->SetTitle("Y axis title");

  cnv = new TCanvas("evstats", "Event Statistics");
  cnv->SetLogy();

  hstat->Draw();
  // hstat2->Draw("same");
  // hstat3->Draw("same");
  // hstat4->Draw("same");
  // hstat5->Draw("same");

  hstat->SetLineColor(kRed);
  // hstat2->SetLineColor(kBlue);
  // hstat3->SetLineColor(kBlack);
  // hstat4->SetLineColor(kGreen);
  // hstat5->SetLineColor(kRed-1);

  TLegend *legend = new TLegend(0.5, 0.6, 0.85, 0.85);
  legend->SetFillColor(kWhite);
  legend->SetLineColor(kWhite);

  legend->AddEntry(hstat, "Run 296934");
  // legend->AddEntry(hstat2, "Run 297031");
  // legend->AddEntry(hstat3, "Run 297221");
  // legend->AddEntry(hstat4, "Run 297363");
  // legend->AddEntry(hstat5, "Run ...");

  legend->Draw();


  for (int i=1; i<=hstat->GetXaxis()->GetNbins(); i++) {
    cout << setw(3) << i
         << "   " << setw(50) << hstat->GetXaxis()->GetBinLabel(i)
         << "   " << hstat->GetBinContent(i) << endl;
  }

  // --------------------------------------------------------------
  // pT spectrum of all and selected electrons and pions

  cnv = new TCanvas("pt", "pT spectra");
  cnv->SetLogy();

  THnSparse *fhAcc = (THnSparse *)lst->FindObject("fhAcc");

  TH1 *hptall = fhAcc->Projection(1);
  // Normalise bin content due to variable bin size misrepresenting the data
    for (int j = 1; j <= hptall->GetNbinsX(); j++)
    {
      Double_t content = hptall->GetBinContent(j);
      Double_t width = (Double_t)hptall->GetBinWidth(j);
      cout << "Content: " << content
           << "Width:  " << width 
           << "New: " << content*(0.1/width) <<  endl;
      hptall->SetBinContent(j,content*(0.1/width));
    }
    
  hptall->SetTitle("Summary of Momentum Statistics for Electrons, Pions and Protons;;");
  hptall->GetXaxis()->SetTitle("P_T(GeV/c)");
  hptall->GetYaxis()->SetTitle("Number of Particles");

  hptall->SetStats(0);
  hptall->Draw();

  TLegend *legpt = new TLegend(0.5, 0.6, 0.85, 0.85);
  legpt->SetFillColor(kWhite);
  legpt->SetLineColor(kWhite);
  legpt->AddEntry(hptall, "All Particles");

  for (int i = 1; i <= fhAcc->GetAxis(0)->GetNbins(); i++) {
    fhAcc->GetAxis(0)->SetRange(i, i);
    TH1 *hpt = fhAcc->Projection(1);

    if (hpt->GetEntries() == 0)
      continue;

    cout << i << " -> " << hpt->GetEntries() << endl;

    // Normalise bin content due to variable bin size misrepresenting the data
    for (int j = 1; j <= hpt->GetNbinsX(); j++)
    {
      Double_t content = hpt->GetBinContent(j);
      Double_t width = (Double_t)hpt->GetBinWidth(j);
      cout << "Content: " << content
           << "Width:  " << width 
           << "New: " << content*(0.1/width) <<  endl;
      hpt->SetBinContent(j,content*(0.1/width));
    }

    // Case numbers are calculated as 2^n + 1, where n is the number of the cut type in the AddTRDdigitsFilter macro.
    switch (i) {
      case 2:
        hpt->SetLineColor(kRed);
        legpt->AddEntry(hpt, "High Momentum Electrons");
        break;

      case 3:
        hpt->SetLineColor(kRed + 1);
        legpt->AddEntry(hpt, "Low Momentum Electrons");
        break;

      case 5:
        hpt->SetLineColor(kBlue);
        legpt->AddEntry(hpt, "High Momentum Pions");
        break;

      case 9:
        hpt->SetLineColor(kBlue + 1);
        legpt->AddEntry(hpt, "Medium Momentum Pions");
        break;

      case 17:
        hpt->SetLineColor(kBlue -1);
        legpt->AddEntry(hpt, "Low Momentum Pions");
        break;

      case 33:
        hpt->SetLineColor(kGreen);
        legpt->AddEntry(hpt, "High Momentum Protons");
        break;

      case 65:
        hpt->SetLineColor(kGreen + 1);
        legpt->AddEntry(hpt, "Medium Momentum Protons");
        break;

      case 129:
        hpt->SetLineColor(kGreen - 1);
        legpt->AddEntry(hpt, "Low Momentum Protons");
        break;

      default:
        hpt->SetLineColor(kYellow);
        legpt->AddEntry(hpt, "Undefined Particles");
        break;
    }

   hpt->Draw("same");
  }

  legpt->Draw();












  //
  //
  // TH1* hea = (TH1*) lst->FindObject("fhPtElecAll");
  // TH1* hes = (TH1*) lst->FindObject("fhPtElecAcc");
  // TH1* hpa = (TH1*) lst->FindObject("fhPtPionAll");
  // TH1* hps = (TH1*) lst->FindObject("fhPtPionAcc");
  //
  // hea->SetLineColor(kRed);
  // hea->SetLineStyle(kDashed);
  //
  // hes->SetLineColor(kRed);
  // hes->SetLineStyle(kSolid);
  // hes->SetLineWidth(3.0);
  //
  // hpa->SetLineColor(kBlue);
  // hpa->SetLineStyle(kDashed);
  //
  // hps->SetLineColor(kBlue);
  // hps->SetLineStyle(kSolid);
  // hps->SetLineWidth(3.0);
  //
  // //hpa->SetTitle("p_{T} spectra;p_{T} (GeV); dN/dp_{T}");
  // hpa->SetTitle("p_{T} spectra;p_{T} (GeV); N");
  // hpa->SetStats(0);
  // hpa->GetXaxis()->SetRangeUser(0., 5.);
  //
  // TLegend* leg = new TLegend(0.5, 0.6, 0.85, 0.85);
  // leg->SetFillColor(kWhite);
  // leg->SetLineColor(kWhite);
  // leg->AddEntry(hea, "all electrons");
  // leg->AddEntry(hes, "selected electrons");
  // leg->AddEntry(hpa, "all pions");
  // leg->AddEntry(hps, "selected pions");
  //
  //
  // hpa->Draw();
  // hps->Draw("same");
  // hea->Draw("same");
  // hes->Draw("same");
  //
  // leg->Draw();
}
