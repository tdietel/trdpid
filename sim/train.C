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
#include "AliTRDdigitsTask.h"

#include <iostream>
using namespace std;
#endif



void train(TString mode="local", TString jobname="r1")
{

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

  // //------------------------------------YEAR 3 PROJECT EDITS------------------------------------------------------------//
  // TMacro multSelection(gSystem->ExpandPathName("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C"));
  // AliMultSelectionTask* multSelectionTask = reinterpret_cast<AliMultSelectionTask*>(multSelection.Exec());
  // //--------------------------------------------------------------------------------------------------------------------//

 #if !defined (__CINT__) || defined (__CLING__)
   //   //AliAnalysisTaskMyTask *task = reinterpret_cast<AliAnalysisTaskMyTask*>(
   //   //gInterpreter->ExecuteMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");

   //gInterpreter->LoadMacro("$ALICE_PHYSICS/PWGPP/TRD/macros/AddTRDdigitsFilter.C (\"PbPb-2018\")");
   gInterpreter->ExecuteMacro("AddDigitsTask.C");
   //AddDigitsTask();

   //   //gInterpreter->ExecuteMacro("$ALICE_PHYSICS/PWGPP/TRD/macros/AddTRDdigitsFilter.C (1)");
   //   //gInterpreter->ExecuteMacro("/home/sgrimbly/alice/data/2018/LHC18r/000296934/test/AddTRDdigitsFilter.C (1)");
   //
#else
//   //gROOT->LoadMacro("\$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
//   //AddTaskPIDResponse(); // *
//
//   gROOT->LoadMacro("\$ALICE_PHYSICS/PWGPP/TRD/macros/AddTRDdigitsFilter.C");
   gROOT->LoadMacro("AddTRDdigitsTask.C");
//   //cout << "RUNNING CORRECT FILTER - 2nd" << endl;
//   //gROOT->LoadMacro("/home/sgrimbly/alice/data/2018/LHC18r/000296934/test/AddTRDdigitsFilter.C (1)");
//   //----------------------------------------------------------------------------------------------------------//
//
//   AddTRDdigitsFilter("PbPb-2018");
//
  AddTRDdigitsTask();
#endif



  // Enable debug printouts
  mgr->SetDebugLevel(2);

  cout  << "initialize and run analyses..." << endl;

  if (!mgr->InitAnalysis()) return;
  mgr->PrintStatus();

  if ( mode == "local") { // run macro on the local machine
    TChain* chain = new TChain("esdTree");

    //chain->Add("/Users/tom/alice/data/2018/LHC18r/000296934/18000296934019.100/AliESDs.root");
    chain->Add("AliESDs.root");

    // start the analysis locally, reading the events from the tchain
    mgr->StartAnalysis("local", chain, 1000000000);

  } else { // run macro on the GRID
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
    // alienHandler->AddRunNumber(296934); // 760k events
    alienHandler->AddRunNumber(296835); // 62k events
    // ---------------------------------------------------------- //



    // number of files per subjob
    alienHandler->SetSplitMaxInputFileNumber(30);

    alienHandler->SetExecutable("digfilter_"+jobname+".sh");

    alienHandler->SetTTL(10000);
    alienHandler->SetJDLName("digfilter_"+jobname+".jdl");

    alienHandler->SetOutputToRunNo(kTRUE);
    alienHandler->SetKeepLogs(kTRUE);

    alienHandler->SetDefaultOutputs(kFALSE);

    //----------------- ST JOHN EDIT ---------------------------
    alienHandler->SetOutputFiles("DigitsFilter.root,AnalysisResults.root");
    //----------------------------------------------------------

    //TString archive = "log_archive.zip:stdout,stderr,pythonDict.txt root_archive.zip:";
    //alienHandler->SetOutputArchive("log_archive.zip:stdout,stderr,pythonDict.txt root_archive.zip:DigitsExtractQA.root");
    alienHandler->SetOutputArchive("log_archive.zip:stdout,stderr root_archive.zip:DigitsFilter.root,AnalysisResults.root");

    alienHandler->SetMaxMergeStages(1);
    alienHandler->SetMergeViaJDL(kTRUE);

    //alienHandler->SetGridWorkingDir(Form("DigitsFilter-%s",nowstr));
    alienHandler->SetGridWorkingDir("digfilter_"+jobname);
    alienHandler->SetGridOutputDir("output");

    mgr->SetGridHandler(alienHandler);


    if ( mode == "test" ) { // run in test mode
      alienHandler->SetNtestFiles(1);

      alienHandler->SetRunMode("test");
      mgr->StartAnalysis("grid");

    } else if ( mode == "submit" ) { // run in test mode

      alienHandler->SetRunMode("full");
      mgr->StartAnalysis("grid");

    } else if ( mode == "merge" ) { // run in test mode

      alienHandler->SetRunMode("terminate");
      mgr->StartAnalysis("grid");

    } else {

      cout << "usage: aliroot 'filter.C(\"MODE\")'"  << endl
           << endl
           << "  where MODE is one of:" << endl
           << "     local" << endl
           << "     test" << endl
           << "     submit" << endl
           << "     merge" << endl
           << endl;

    }
  }
}
