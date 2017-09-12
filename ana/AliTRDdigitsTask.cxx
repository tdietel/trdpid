#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"

#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"

#include "AliESDEvent.h"
#include "AliESDInputHandler.h"

#include "AliMCEventHandler.h"
#include "AliMCEvent.h"

//#include "AliKalmanTrack.h"

#include "AliTRDpadPlane.h"
#include "AliTRDtrackV1.h"
#include "AliTRDseedV1.h"

#include "AliTRDdigitsTask.h"

// example of an analysis task creating a p_t spectrum
// Authors: Panos Cristakoglou, Jan Fiete Grosse-Oetringhaus, Christian Klein-Boesing
// Reviewed: A.Gheata (19/02/10)

ClassImp(AliTRDdigitsTask)

//________________________________________________________________________
AliTRDdigitsTask::AliTRDdigitsTask(const char *name) 
  : AliAnalysisTaskSE(name), fESD(0), fOutputList(0), fHistPt(0)
{
  // Constructor

  // Define input and output slots here
  // Input slot #0 works with a TChain
  DefineInput(0, TChain::Class());
  // Output slot #0 id reserved by the base class for AOD
  // Output slot #1 writes into a TH1 container
  DefineOutput(1, TList::Class());
}

//________________________________________________________________________
void AliTRDdigitsTask::UserCreateOutputObjects()
{
  // Create histograms
  // Called once

  fOutputList = new TList();

}

//_______________________________________________________________________
Bool_t AliTRDdigitsTask::UserNotify()
{
  delete fDigitsInputFile;
  delete fDigitsOutputFile;

  AliESDInputHandler *esdH = dynamic_cast<AliESDInputHandler*> (AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler());

  if ( fDigitsInputFileName != "" ) {
    TString ifname = esdH->GetInputFileName();
    ifname.ReplaceAll("AliESDs.root", fDigitsInputFileName);
    AliInfo(Form("opening digits file %s for reading", ifname));
    fDigitsInputFile = new TFile(ifname);
    if (!fDigitsInputFile) {
      AliWarning(Form("digits input file '%s' cannot be opened", ofname));
    }
  } else {
    fDigitsInputFile = NULL;
  }

  if ( fDigitsOutputFileName != "" ) {
    TString ofname = esdH->GetInputFileName();
    ofname.ReplaceAll("AliESDs.root", fDigitsOutputFileName);
    AliInfo(Form("opening digits file %s for writing", ofname));
    fDigitsOutputFile = new TFile(ofname);
    if (!fDigitsOutputFile) {
      AliWarning(Form("digits output file '%s' cannot be opened", ofname));
    }
  } else {
    fDigitsOutputFile = NULL;
  }
  
  fEventNoInFile = 0;

  return kTRUE;
}


//________________________________________________________________________
void AliTRDdigitsTask::UserExec(Option_t *) 
{
  // Main loop
  // Called for each event


  // -----------------------------------------------------------------
  // prepare event data structures
  fESD = dynamic_cast<AliESDEvent*>(InputEvent());
  if (!fESD) {
    printf("ERROR: fESD not available\n");
    return;
  }

  printf("There are %d tracks in this event\n", fESD->GetNumberOfTracks());

//  AliMCEvent* mcEvent = MCEvent();
//  if (!mcEvent) {
//    Printf("ERROR: Could not retrieve MC event");
//    return;
//  }
//
//  // the geometry could be created in the constructor or similar
//  AliTRDgeometry * geo = new AliTRDgeometry;
//  if (! geo) {
//    cerr << "cannot create geometry " << endl;
//    return;
//  }
//    
//
//  
//  Printf("MC particles: %d", mcEvent->GetNumberOfTracks());
//
//  
//  cout << "ESD friend:" << fESDfriend << endl;
//
//  cout << "# of friend tracks: " << fESDfriend->GetNumberOfTracks() << endl;
//  
//
//  // -----------------------------------------------------------------
//  // Track loop to fill a pT spectrum
//  for (Int_t iTracks = 0; iTracks < fESD->GetNumberOfTracks(); iTracks++) {
//
//    cout << "------------------------------------" << endl;
//    cout << "track " << iTracks << endl;
//
//
//
//    // ---------------------------------------------------------------
//    // gather track information
//    AliESDtrack* track = fESD->GetTrack(iTracks);
//    if (!track) {
//      printf("ERROR: Could not receive track %d\n", iTracks);
//      continue;
//    }
//
//    AliESDfriendTrack* friendtrack = fESDfriend->GetTrack(iTracks);
//    if (!friendtrack) {
//      printf("ERROR: Could not receive frienttrack %d\n", iTracks);
//      continue;
//    }
//    
//    if (!track->GetInnerParam()) {
//      cerr << "ERROR: no inner param" << endl;
//      continue;
//    }
//
//    
//    AliVParticle* mctrack = mcEvent->GetTrack(track->GetLabel());
//    if (!mctrack) {
//      Printf("ERROR: Could not receive MC track %d", track->GetLabel());
//      continue;
//    }
//
//    const AliExternalTrackParam* trdtrack = friendtrack->GetTRDIn();
//    if (!trdtrack) {
//      Printf("WARNING: Could not receive TRD track");
//      continue;
//    }
//
//    // find AliTRDtrackV1
//    TObject* fCalibObject = 0;
//    AliTRDtrackV1* trdTrack = 0;
//    // find TRD track
//    int icalib=0;
//    while ((fCalibObject = (TObject*)(friendtrack->GetCalibObject(icalib++)))){
//      if(strcmp(fCalibObject->IsA()->GetName(), "AliTRDtrackV1") != 0)
//	continue;
//      trdTrack = (AliTRDtrackV1 *)fCalibObject;
//    }
//
//    
//    // ---------------------------------------------------------------
//    // display general track information
//    
//    cout << "  label = " << track->GetLabel() << endl;
//
//    cout << "  eta  [rec] = " << track->GetInnerParam()->Eta() << endl;
//    cout << "  phi  [rec] = " << track->GetInnerParam()->Phi() << endl;
//    cout << "  pt   [rec] = " << track->GetInnerParam()->Pt() << endl;
//
//    cout << "  eta  [MC]  = " << mctrack->Eta() << endl;
//    cout << "  phi  [MC]  = " << mctrack->Phi() << endl;
//    cout << "  pt   [MC]  = " << mctrack->Pt() << endl;
//    cout << "  pdg  [MC]  = ";
//
//    switch (abs(mctrack->PdgCode())) {
//    case   11:   cout << "electron"; break; 
//    case  211:   cout << "pion"; break; 
//    case 2212:   cout << "proton"; break; 
//    default:     cout << "unknown"; break; 
//    }
//
//    cout << " (" << mctrack->PdgCode() << ")" << endl;
//    
//
//    cout << "  pt   [TRD] = " << trdtrack->Pt() << endl;
//    cout << "  x    [TRD] = " << trdtrack->GetX() << endl;
//    cout << "  y    [TRD] = " << trdtrack->GetY() << endl;
//    cout << "  z    [TRD] = " << trdtrack->GetZ() << endl;
//
//    // cout << "  trd clusters: " << friendtrack->GetMaxTRDcluster() << endl;
//    // Int_t trkidx[6];
//    // track->GetTRDtracklets(trkidx);
//    // for (int ic = 0; ic < track->GetTRDntracklets(); ic++) {
//    //   cout << "  trkl: " << ic << ": " << trkidx[ic] << endl;
//    // }
//
//    // const AliTrackPointArray *array= friendtrack->GetTrackPointArray();
//    // if (!array) {
//    //   cout << "no track points found" << endl;
//    //   continue;
//    // }
//
//    // cout << "track " << iTracks << " has " << array->GetNPoints()
//    // 	 << " points" << endl;
//
//
//    // Int_t cls[180];
//    // track->GetTRDclusters(cls);
//    // for (int i=0;i<180;i++) {
//    //   if (cls[i] != -2) {
//    // 	cout << "  cls [" << i << "]  " << cls[i] << endl;
//    //   }
//    // }
//
//    // //cout << "  trk " << int(track->GetTRDntracklets()) << endl;
//    // Int_t trk[AliESDtrack::kTRDnPlanes];
//    // track->GetTRDtracklets(trk);
//
//    // for (int it=0; it<AliESDtrack::kTRDnPlanes; it++) {
//    //   if (trk[it] != -2) {
//    // 	cout << "  trk [" << it << "]  " << trk[it] << endl;
//    //   }
//    // }
//
//
//
//    
//    // loop over tracklets
//    for(Int_t itr = 0; itr < 6; ++itr) {
//
//      AliTRDseedV1* tracklet = 0;
//
//      if(!(tracklet = trdTrack->GetTracklet(itr)))
//	continue;
//      if(!tracklet->IsOK())
//	continue;
//
//
//      AliTRDpadPlane *padplane = geo->GetPadPlane(tracklet->GetDetector());
//      
//      cout << "    tracklet: " << tracklet->GetDetector()
//	   << ":" << padplane->GetPadRowNumber(tracklet->GetZ())
//	   << ":" << padplane->GetPadColNumber(tracklet->GetY())
//	   << "   "
//	   << tracklet->GetX() << " / "
//	   << tracklet->GetY() << " / "
//	   << tracklet->GetZ() 
//	   << endl;
//
//    }
//    
//    
//    // AliTrackPoint tp;
//    // for (int ip=0; ip<array->GetNPoints(); ip++) {
//    //   array->GetPoint(tp,ip);
//    //   cout << "  point " << ip << ": "
//    // 	   << tp.GetVolumeID() << "  -  "
//    // 	   << tp.GetX() << "/" << tp.GetY() << "/" << tp.GetZ()
//    // 	   << "   r = " << TMath::Hypot(tp.GetX(),tp.GetY())
//    // 	   << endl;
//    // }
//    
//
//    
//
//
//    
//    fHistPt->Fill(track->Pt());
//  } //track loop 
//
//  delete geo;
//
  
  PostData(1, fOutputList);
}      
//________________________________________________________________________
void AliTRDdigitsTask::Terminate(Option_t *) 
{
  // Draw result to the screen
  // Called once at the end of the query

  fOutputList = dynamic_cast<TList*> (GetOutputData(1));
  if (!fOutputList) {
    printf("ERROR: Output list not available\n");
    return;
  }
  
  fHistPt = dynamic_cast<TH1F*> (fOutputList->At(0));
  if (!fHistPt) {
    printf("ERROR: fHistPt not available\n");
    return;
  }
   
  //TCanvas *c1 = new TCanvas("AliTRDdigitsTask","Pt",10,10,510,510);
  //c1->cd(1)->SetLogy();
  //fHistPt->DrawCopy("E");
}


//________________________________________________________________________
void AliTRDdigitsTask::ReadDigits()
{
  // reset digit arrays
  for (Int_t det=0; det<540; det++) {
    fDigMan->ClearArrays(det);
    fDigMan->ClearIndexes(det);
  }

  // read digits from file
  TTree* tr = (TTree*)fDigitsInputFile->Get(Form("Event%d/TreeD",
                                                 fEventNoInFile));
  fDigMan->ReadDigits(tr);
  delete tr;

  // expand digits for use in this task
  for (Int_t det=0; det<540; det++) {
    fDigMan->GetDigits(det)->Expand();
  }
}

//________________________________________________________________________
void AliTRDdigitsTask::WriteDigits()
{

  // compress digits for storage
  for (Int_t det=0; det<540; det++) {
    fDigMan->GetDigits(det)->Expand();
  }

  // create directory to store digits tree
  TDirectory* evdir =
    fDigitsOutputFile->mkdir(Form("Event%d", fEventNoInFile),
                             Form("Event%d", fEventNoInFile));

  evdir->Write();
  evdir->cd();

  // save digits tree
  TTree* tr = new TTree("TreeD", "TreeD");
  fDigMan->MakeBranch(tr);
  fDigMan->WriteDigits();
  delete tr;
}

