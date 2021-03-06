/**************************************************************************
* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
*                                                                        *
* Author:             *
* Contributors are mentioned in the code where appropriate.              *
*                                                                        *
* Permission to use, copy, modify and distribute this software and its   *
* documentation strictly for non-commercial purposes is hereby granted   *
* without fee, provided that the above copyright notice appears in all   *
* copies and that both the copyright notice and this permission notice   *
* appear in the supporting documentation. The authors make no claims     *
* about the suitability of this software for any purpose. It is          *
* provided "as is" without express or implied warranty.                  *
**************************************************************************/
//
// The task:
// 
//
//  Author:
//  
//

#ifndef ALITRDPIDRAWDATA_H
#define ALITRDPIDRAWDATA_H
#include "AliAnalysisTaskSE.h"


class TTreeStream;
class AliInputEventHandler;
class AliTRDdigitsManager;
class TArrayF;

class TFile;
class AliESDEvent;
class AliESDtrackCuts;
class AliESD;
class AliESDtrack;
class AliAnalysisTask;
class AliESDInputHandler;
class AliESDv0KineCuts;
class AliAnalysisManager;
class AliCentrality;
class TSystem;
class TStyle;
class TROOT;
class Riostream;
class TChain;
class TH2;
class TF1;
class TH1;
class TObjArray;


class AliTRDPIDrawData : public AliAnalysisTaskSE {

    public:
  typedef enum{
      kpp = 0,
      kpPb = 1,
      kPbPb = 2
    } ECollisionSystem_t;

  AliTRDPIDrawData(const char *name = "trd_pid_tree");
  virtual ~AliTRDPIDrawData();
  virtual void   UserCreateOutputObjects();
  virtual Bool_t UserNotify();
  virtual void   UserExec(Option_t *);
  virtual void   Process(AliESDEvent *const esdEvent=0);
  virtual void   Terminate(const Option_t*);

  Int_t          GetV0tag(Int_t trackIndex) const;
  Int_t          *fV0tags;  //! Pointer to array with tags for identified particles from V0 decays

  Bool_t Ispp() const { return fCollisionSystem.TestBitNumber(kpp); }
  Bool_t IspPb() const { return fCollisionSystem.TestBitNumber(kpPb); }
  Bool_t IsPbPb() const { return fCollisionSystem.TestBitNumber(kPbPb); }

  void SetCollisionSystem(ECollisionSystem_t system){
      fCollisionSystem.Clear();
      fCollisionSystem.SetBitNumber(system, kTRUE);
  }
  void SetppAnalysis(){
      fCollisionSystem.SetBitNumber(kpPb, kFALSE);
      fCollisionSystem.SetBitNumber(kPbPb, kFALSE);
      fCollisionSystem.SetBitNumber(kpp, kTRUE);
  }
  void SetpPbAnalysis() {
      fCollisionSystem.SetBitNumber(kpp, kFALSE);
      fCollisionSystem.SetBitNumber(kPbPb, kFALSE);
      fCollisionSystem.SetBitNumber(kpPb, kTRUE);
  }
  void SetPbPbAnalysis() {
      fCollisionSystem.SetBitNumber(kpp, kFALSE);
      fCollisionSystem.SetBitNumber(kpPb, kFALSE);
      fCollisionSystem.SetBitNumber(kPbPb, kTRUE);
  };

  protected:

  AliESDv0KineCuts *fV0cuts;           //! ESD V0 cuts
  TObjArray *fV0electrons;             //! array with pointer to identified particles from V0 decays (electrons)
  TObjArray *fV0pions;                 //! array with pointer to identified particles from V0 decays (pions)

  void ReadDigits();
  void WriteDigits();
  
  void WriteRaw();
  void SetupV0qa();
  void FillV0PIDlist();
  void ClearV0PIDlist();
 
  Bool_t PassTrackCuts(AliESDtrack *fESDTrack=0,Int_t thres=0);
  Bool_t PassTrackPIDCuts(AliESDtrack *fESDTrack=0);

  private:
  //
  //
  AliESDEvent *fESDEvent;              //! ESD object
 
 
  TObjArray *fOutputContainer;         //! output data container
  AliESDtrackCuts *fESDtrackCuts;      //! basic cut variables for all non-V0 tracks
  AliESDtrackCuts *fESDtrackCutsV0;    //! basic cut variables for all V0 tracks
  //
  TList                 *fListQATRD;   //! List with QATRD histograms
  TList                 *fListQATRDV0;   //! List with QATRD histograms
 
  Int_t fNumTagsStored;                //! Number of entries of fV0tags

  TBits fCollisionSystem;              //! Collision System;

  TFile* fDigitsInputFile;             //! Digits file for reading
  TFile* fDigitsOutputFile;            //! Digits file for writing

  Int_t fEventNoInFile;

  AliTRDdigitsManager* fDigMan;        //! digits manager
  
  // Histograms
  TH1F *fhtrackCuts;                 //! Track and Event Cuts - QA
  TH2F *fhArmenteros;                 //! 2D V0 QA Hist
  TH2F *fhNumberEle;
  TH1F *fhNumberEleEvent;
  TH2F *fhNumberEleCut[6];
  TH1F *fhNumberEleEventCut[6];
  TH2F *fhNumberEleCutp;
  TH1F *fhNumberEleEventCutp;
  TH2F *fhNumberPion;
  TH2F *fhNumberPionCut[6];
  TH2F *fhNumberPionCutp;
  TH1F *fhNumberPionEvent;
  TH1F *fhNumberPionEventCut[6];
  TH1F *fhNumberPionEventCutp;
  TH2F *fhTPCsignalvsp;
  TH1F *fhCent;
  TH1F* fhEv;

  
  AliTRDPIDrawData(const AliTRDPIDrawData&); // not implemented
  AliTRDPIDrawData& operator=(const AliTRDPIDrawData&); // not implemented
  
  ClassDef(AliTRDPIDrawData, 1);
};
#endif
