

void ana()
{
  TFile *fo = new TFile("ana.root","RECREATE");

  TH1F *hadc  = new TH1F("hadc","ADC spectrum;ADC;counts", 1024, -0.5, 1023.5);
  TH1F *hadcw = new TH1F("hadcw","ADC spectrum;ADC;counts", 1000, -100000, 100000);
  TH1F *h2 = new TH1F("h2","TBsum spectrum;TBsum;counts", 600, -0.5, 6000);
  TNtuple *nt = new TNtuple("nt","nt", "det:row:col:tb:sum");
  
  TFile fd("TRD.Digits.root");
  tr = (TTree*)fd.Get("Event1/TreeD");
  tr->Print();


  AliTRDdigitsManager* digman = new AliTRDdigitsManager;
  digman->CreateArrays();
  digman->ReadDigits(tr);

  for (int det=0; det<540; det++) {

    if (!digman->GetDigits(det))
      continue;

    digman->GetDigits(det)->Expand();
    
    for (int row=0; row<digman->GetDigits(det)->GetNrow(); row++) {

      for (int c=0; c<digman->GetDigits(det)->GetNcol(); c++) {

	int tbsum = 0;
	for (int t=0; t<digman->GetDigits(det)->GetNtime(); t++) {


	  //int adc = digman->GetDigits(det)->GetDataBits(row,c,t); 
	  int adc = digman->GetDigitAmp(row,c,t,det); 

	  if (adc==-7169)
	    continue;

	  hadc->Fill(adc);
	  hadcw->Fill(adc);
	  tbsum += adc;


	  //if (adc<0 || adc>=1024) {
	  if (1) {
	    cout << det << "/" << row << "/" << c << "/" << t
		 << " -> " << adc
		 << endl;
	  }

	  
	}

	if (0 && tbsum>350 && tbsum<800) {
	  cout << det << "/" 
	       << row << "/" 
	       << c << " -> tbsum="
	       << tbsum << endl;
	}
	
	h2->Fill(tbsum);
	nt->Fill(det,row,c,t,tbsum);
      }
    }

  }

  fo->cd();
  hadc->Write();
  hadcw->Write();
  h2->Write();
  nt->Write();
  
  //h1->Draw();
  h2->Draw();
}
