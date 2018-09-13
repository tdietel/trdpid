void rec() {
  AliReconstruction reco;

  reco.SetWriteESDfriend();
  reco.SetWriteAlignmentData();

  reco.SetDefaultStorage
    ( "local:///cvmfs/alice-ocdb.cern.ch/calibration/data/2016/OCDB" );
  //( "local:///cvmfs/alice-ocdb.cern.ch/calibration/MC/Residual/");
    
  reco.SetRunPlaneEff(kTRUE);

  reco.SetFractionFriends(1.);
  reco.SetOption("TRD", "tw,dc");
  //reco.SetOption("TRD", "tp,tw,dc");

  AliLog::SetClassDebugLevel("AliTRDReconstructor", 1);
  
  TStopwatch timer;
  timer.Start();
  reco.Run();
  timer.Stop();
  timer.Print();
}
