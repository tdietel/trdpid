

void digitsqa( TString infilename="TRD.Digits.root",
               TString outfilename="digitsqa.root")
{

  // ======================================================================
  // Book histograms


  TFile* outfile = new TFile(outfilename, "RECREATE");

  TH1F* hAdc = new TH1F("hADC", "ADC spectrum", 1024, -0.5, 1023.5);
  hAdc->SetXTitle("ADC value");
  hAdc->SetYTitle("number of entries");

  TH1F* hTBsum = new TH1F("hTBsum", "TBsum", 3000, -0.5, 2999.5);



  TFile fd(infilename);

  AliTRDdigitsManager* digman = new AliTRDdigitsManager;
  digman->CreateArrays();


  TIter next(fd.GetListOfKeys());
  while ( TObject *obj = next() ) {

    cout << "Processing " << obj->GetName() << endl;

    TTree* tr = (TTree*)fd.Get(Form("%s/TreeD",obj->GetName()));
    //tr->Print();

    for (int det=0; det<540; det++) {
      digman->ClearArrays(det);
      digman->ClearIndexes(det);
    }

    digman->ReadDigits(tr);


    for (int det=0; det<540; det++) {
      if (!digman->GetDigits(det))
        continue;

     digman->GetDigits(det)->Expand();

     //cout << "Detector " << det << endl;

     // TODO: check actual number of rows, from geometry
     // here: play it safe, assume 12 rows everywhere
     int nrows = 12;

     for (int r=0; r<nrows; r++) {
       for (int c = 0; c < 144; c++) {

         int tbsum = 0;
         for (int t=0; t<digman->GetDigits(det)->GetNtime(); t++) {
           //
           //   //int adc = digman->GetDigits(det)->GetDataBits(row,c,t);
           int adc = digman->GetDigitAmp(r,c,t,det);

           // this value seems to indicate no digit -> skip
           if (adc==-7169) continue;

           hAdc->Fill(adc);

           tbsum += adc;
         }

         if (tbsum>0) {
           hTBsum->Fill(tbsum);
         }


         //
         // if (tbsum>600) {
         //   cout << det << ":" << r << ":" << c << "   "
         //        << tbsum << endl;
         // }

       } // col
     } // row
   } // det

 } // event loop

 TCanvas* cnv_adc = new TCanvas("cnv_adc", "cnv_adc");
 cnv_adc->SetLogy();
 hAdc->Draw();

 TCanvas* cnv_tbsum = new TCanvas("cnv_tbsum", "cnv_tbsum");
 cnv_adc->SetLogy();
 hTBsum->Draw();

 outfile->Write();

}
