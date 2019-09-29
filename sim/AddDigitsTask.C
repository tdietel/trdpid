//=============================================================================
//
// *** AddTRDdigitsFilter
//
// This macro sets up the TRD digits filter to write the digits for a
// PID reference sample of electrons and pions from V0 decays.
//
//=============================================================================

#if ! defined (__CINT__) || defined (__MAKECINT__)
#include <AliAnalysisManager.h>
#include <AliAnalysisDataContainer.h>
#include <AliAnalysisTask.h>
#include <AliESDv0KineCuts.h>
#include <AliTRDdigitsFilter.h>
#endif

AliAnalysisTask  *AddDigitsTask()
{
  //gSystem->Load("libTRDrec");
  // pointer to the analysis manager
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    Error("AddDigitsTask", "No analysis manager to connect to.");
    return NULL;
  }

  // check the input handler
  if (!mgr->GetInputEventHandler()) {
    ::Error("AddTask", "This task requires an input event handler");
    return NULL;
  }

  /////////////////////////
  // The TRD filter Task
  /////////////////////////
  AliTRDdigitsTask *task = new AliTRDdigitsTask("DigitsTask");
  //task->PrintSettings();

  mgr->AddTask(task);

  AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer();

  if (!cinput) cinput = mgr->CreateContainer("cchain",TChain::Class(),
                                      AliAnalysisManager::kInputContainer);

  AliAnalysisDataContainer *coutput =mgr->CreateContainer("DigitsTask",TList::Class(), AliAnalysisManager::kOutputContainer, "DigitsTask.root");


  mgr->ConnectInput(task,0,cinput);
  mgr->ConnectOutput(task,1,coutput);
  return task;

}
