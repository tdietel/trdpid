void summary(TString fname = "DigitsFilter.root") {

  TFile *f = new TFile(fname.Data());
  TCanvas *cnv = NULL;
  TList *lst;
  f->GetObject("TRDdigitsFilter", lst);

  // --------------------------------------------------------------
  // statistics of available and selected events

  TH1 *hstat = (TH1 *)lst->FindObject("EventCuts");

  hstat->SetStats(0);

  hstat->GetXaxis()->SetBinLabel(3,"HELLLOOOOO!!!");

  cnv = new TCanvas("evstats", "event statistics");
  cnv->SetLogy();

  hstat->Draw();
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
  hptall->SetStats(0);
  hptall->Draw();

  TLegend *legpt = new TLegend(0.5, 0.6, 0.85, 0.85);
  legpt->SetFillColor(kWhite);
  legpt->SetLineColor(kWhite);
  legpt->AddEntry(hptall, "all particles");

  for (int i = 1; i <= fhAcc->GetAxis(0)->GetNbins(); i++) {
    fhAcc->GetAxis(0)->SetRange(i, i);
    TH1 *hpt = fhAcc->Projection(1);

    if (hpt->GetEntries() == 0)
      continue;

    cout << i << " -> " << hpt->GetEntries() << endl;

    switch (i) {
    case 2:
      hpt->SetLineColor(kRed);
      legpt->AddEntry(hpt, "v0elec");
      break;

    case 3:
      hpt->SetLineColor(kBlue);
      legpt->AddEntry(hpt, "v0pilo");
      break;

    case 5:
      hpt->SetLineColor(kBlue + 1);
      legpt->AddEntry(hpt, "v0pihi");
      break;

    case 9:
      hpt->SetLineColor(kGreen - 1);
      legpt->AddEntry(hpt, "v0prot");
      break;

    default:
      hpt->SetLineColor(kYellow);
      legpt->AddEntry(hpt, "UNDEF");
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
