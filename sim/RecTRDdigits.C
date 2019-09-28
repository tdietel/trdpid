
void ReconstructionCustom(AliReconstruction &rec)
{

  Int_t year = atoi(gSystem->Getenv("CONFIG_YEAR"));

  //
  // set OCDB source
  TString ocdbConfig = "default,snapshot";
  if (gSystem->Getenv("CONFIG_OCDB"))
    ocdbConfig = gSystem->Getenv("CONFIG_OCDB");

  if (ocdbConfig.Contains("alien") || ocdbConfig.Contains("cvmfs")) {
    // set OCDB
    gROOT->LoadMacro("$ALIDPG_ROOT/MC/OCDBConfig.C");
    gROOT->ProcessLine("OCDBDefault(1);");
  }
  else {
    // set OCDB snapshot mode
    rec.SetCDBSnapshotMode("OCDBrec.root");
    AliCDBManager *cdbm = AliCDBManager::Instance();
    cdbm->SetDefaultStorage("local://");
    //    cdbm->SetSnapshotMode("OCDBrec.root");
  }

  //
  rec.SetCleanESD(kFALSE);
  rec.SetStopOnError(kFALSE);
  rec.SetWriteESDfriend();
  rec.SetWriteAlignmentData();
  rec.SetFractionFriends(0.01);
  rec.SetRunPlaneEff(kTRUE);
  rec.SetUseTrackingErrorsForAlignment("ITS");

  // Keep the digits for the TRD
  rec.SetOption("TRD", "cw,dc");

  cout << "Writing TRD digits" << endl;

  // IMPORTANT: For pPb productions anchored to 2013 runs, this setting has to be also added in the rec.C using AliRoot from v5-03-Rev-28 (announced in the Physics Board the 6th of June)
  if (year == 2013)
    rec.SetOption("TPC","PID.OADB=TSPLINE3_MC_%s_LHC13B2_FIXn1_PASS1_PPB_MEAN");

  rec.SetRunQA(":");
}
