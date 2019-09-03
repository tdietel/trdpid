//#include "AliAnalysisAlien.h"
//#include "AliAnalysisManager.h"
//#include "AliAODInputHandler.h"
//#include "AliTRDdigitsExtract.h"
//#include "ana.h
#if !defined (__CINT__) || defined (__CLING__)
#include "AliAnalysisAlien.h"
#include "AliAnalysisManager.h"
#include "AliESDInputHandler.h"
#include "AliTRDdigitsFilter.h"

#include <iostream>
using namespace std;
#endif

void filter(int mode)
{

  // Boolean defining how the analysis should be run
  Bool_t local, gridTest;

  switch (mode) {
    case 0:  local = kTRUE;   gridTest = kTRUE;    break;
    case 1:  local = kFALSE;  gridTest = kTRUE;    break;
    case 2:  local = kFALSE;  gridTest = kFALSE;   break;
  }

#if !defined (__CINT__) || defined (__CLING__)
  gInterpreter->ProcessLine(".include $ROOTSYS/include");
  gInterpreter->ProcessLine(".include $ALICE_ROOT/include");
  gInterpreter->ProcessLine(".include $ALICE_PHYSICS/include");
#else
  gROOT->ProcessLine(".include $ROOTSYS/include");
  gROOT->ProcessLine(".include $ALICE_ROOT/include");
  gROOT->ProcessLine(".include $ALICE_PHYSICS/include");
#endif

  gSystem->Load("libTree.so");
  gSystem->Load("libGeom.so");
  gSystem->Load("libVMC.so");
  gSystem->Load("libSTEERBase.so");
  gSystem->Load("libESD.so");
  gSystem->Load("libAOD.so");

  // load analysis framework
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISalice");

  // for includes use either global setting in $HOME/.rootrc
  // ACLiC.IncludePaths: -I$(ALICE_ROOT)/include
  // or in each macro
  gSystem->AddIncludePath("-I$ALICE_ROOT/include");
  gSystem->AddIncludePath("-I$ALICE_PHYSICS/include");

  // get a string with the current time
  time_t now;
  time(&now);
  char nowstr[20];
  strftime(nowstr, sizeof nowstr, "%Y%m%d-%H%M%S", localtime(&now));

  // Create the analysis manager
  AliAnalysisManager *mgr = new AliAnalysisManager("testAnalysis");
  AliESDInputHandler* esdH = new AliESDInputHandler();
  //esdH->SetReadFriends(kTRUE);
  mgr->SetInputEventHandler(esdH);

  // Enable MC event handler
  //  AliMCEventHandler* handler = new AliMCEventHandler;
  //handler->SetReadTR(kFALSE);
  //mgr->SetMCtruthEventHandler(handler);


  cout << "creating analysis tasks..." << endl;

  //------------------------------------YEAR 3 PROJECT EDITS------------------------------------------------------------//
  TMacro multSelection(gSystem->ExpandPathName("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C"));
  AliMultSelectionTask* multSelectionTask = reinterpret_cast<AliMultSelectionTask*>(multSelection.Exec());
  //--------------------------------------------------------------------------------------------------------------------//

#if !defined (__CINT__) || defined (__CLING__)
  //AliAnalysisTaskMyTask *task = reinterpret_cast<AliAnalysisTaskMyTask*>(
  //gInterpreter->ExecuteMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");


//------------------------------------YEAR 3 PROJECT EDITS---------------------------------------------------//
gInterpreter->ExecuteMacro("$ALICE_PHYSICS/PWGPP/TRD/macros/AddTRDdigitsFilter.C (\"PbPb-2018\")");
//gInterpreter->ExecuteMacro("$ALICE_PHYSICS/PWGPP/TRD/macros/AddTRDdigitsFilter.C (1)");
//gInterpreter->ExecuteMacro("/home/sgrimbly/alice/data/2018/LHC18r/000296934/test/AddTRDdigitsFilter.C (1)");

#else
  //gROOT->LoadMacro("\$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
  //AddTaskPIDResponse(); // *

  gROOT->LoadMacro("\$ALICE_PHYSICS/PWGPP/TRD/macros/AddTRDdigitsFilter.C");
  //cout << "RUNNING CORRECT FILTER - 2nd" << endl;
  //gROOT->LoadMacro("/home/sgrimbly/alice/data/2018/LHC18r/000296934/test/AddTRDdigitsFilter.C (1)");
  //----------------------------------------------------------------------------------------------------------//

  AddTRDdigitsFilter("PbPb-2018");

#endif



  // Enable debug printouts
  mgr->SetDebugLevel(2);

  cout  << "initialize and run analyses..." << endl;

  if (!mgr->InitAnalysis()) return;
  mgr->PrintStatus();

  if (local){ // run macro on the local machine
    TChain* chain = new TChain("esdTree");

    //------------------------ ST JOHN EDIT --------------------------------------//
    chain->Add("/Users/tom/alice/data/2018/LHC18r/000296934/18000296934019.100/AliESDs.root");
    //----------------------------------------------------------------------------//

    // start the analysis locally, reading the events from the tchain
    mgr->StartAnalysis("local", chain, 1000000000);

  }else{ // run macro on the GRID
    AliAnalysisAlien *alienHandler = new AliAnalysisAlien();

    alienHandler->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include");

    //alienHandler->SetAdditionalLibs("AliTRDdigitsExtract.cxx AliTRDdigitsExtract.h");
    //alienHandler->SetAnalysisSource("AliTRDdigitsExtract.cxx");

    //alienHandler->SetAliPhysicsVersion("vAN-20180902-1");
    alienHandler->SetAliPhysicsVersion("vAN-20190903_ROOT6-1");
    alienHandler->SetAPIVersion("V1.1x"); // set the alien API version



    // ----------------- ST JOHN EDIT --------------------------- //
    //alienHandler->SetGridDataDir("/alice/data/2016/LHC16q/");
    alienHandler->SetGridDataDir("/alice/data/2018/LHC18r");
    //alienHandler->SetDataPattern("pass1_CENT_wSDD/*/*ESDs.root"); 
    alienHandler->SetDataPattern("pass1/*/*ESDs.root");
    //
    alienHandler->SetRunPrefix("000");

    // specify the run number of the files to runu the analysis on
    alienHandler->AddRunNumber(296934);
    // ---------------------------------------------------------- //



    // number of files per subjob
    alienHandler->SetSplitMaxInputFileNumber(40);
    TString name = "myTask";

    alienHandler->SetExecutable(name+".sh");

    alienHandler->SetTTL(10000);
    alienHandler->SetJDLName(name+".jdl");

    alienHandler->SetOutputToRunNo(kTRUE);
    alienHandler->SetKeepLogs(kTRUE);

    alienHandler->SetDefaultOutputs(kFALSE);

    //----------------- ST JOHN EDIT ---------------------------
    alienHandler->SetOutputFiles("DigitsFilter.root");
    //----------------------------------------------------------

    //TString archive = "log_archive.zip:stdout,stderr,pythonDict.txt root_archive.zip:";
    //alienHandler->SetOutputArchive("log_archive.zip:stdout,stderr,pythonDict.txt root_archive.zip:DigitsExtractQA.root");
    alienHandler->SetOutputArchive("log_archive.zip:stdout,stderr root_archive.zip:DigitsFilter.root");

    alienHandler->SetMaxMergeStages(1);
    alienHandler->SetMergeViaJDL(kTRUE);

    alienHandler->SetGridWorkingDir(Form("DigitsFilter-%s",nowstr));
    alienHandler->SetGridOutputDir("output");

    mgr->SetGridHandler(alienHandler);


    if (gridTest){ // run in test mode
      alienHandler->SetNtestFiles(1);

      alienHandler->SetRunMode("test");
      mgr->StartAnalysis("grid");
    }else{
      alienHandler->SetRunMode("full");
      //alienHandler->SetRunMode("terminate");
      mgr->StartAnalysis("grid");
    }
  }
}
