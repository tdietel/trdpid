void sim(Int_t nev=10) {

  // libraries required by geant321
  gSystem->Load("liblhapdf");
  gSystem->Load("libEGPythia6");
  gSystem->Load("libpythia6");
  gSystem->Load("libAliPythia6");
  gSystem->Load("libgeant321");

  AliSimulation simulator;

  // ------------------------------------------------------------------
  // anchor simulation to a run and OCDB
  simulator.SetRunNumber(265525); // p-Pb 2016
  
  simulator.SetDefaultStorage
    ( "local:///cvmfs/alice-ocdb.cern.ch/calibration/data/2016/OCDB" );
  //( "local:///cvmfs/alice-ocdb.cern.ch/calibration/MC/Residual/");

  //simulator.SetSpecificStorage( "",
  //				"local:///cvmfs/alice-ocdb.cern.ch/calibration/data/2016/OCDB");
  //				Form("local://%s/miniocdb",gSystem->pwd()));
  
  
  // ------------------------------------------------------------------
  // Set simulation parameters
  //simulator.SetMakeSDigits("TRD TOF HMPID EMCAL MUON FMD ZDC PMD T0 VZERO ACORDE");
  simulator.SetMakeSDigits("TRD");
  simulator.SetMakeDigits("TRD");
  simulator.SetMakeDigitsFromHits("ITS TPC");
  simulator.SetWriteRawData("TRD TOF HMPID EMCAL MUON FMD ZDC PMD T0","raw.root",kTRUE);


  // ------------------------------------------------------------------
  // run the simulation
  TStopwatch timer;
  timer.Start();
  simulator.Run(nev);
  timer.Stop();
  timer.Print();
}
