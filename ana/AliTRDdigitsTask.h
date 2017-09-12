#ifndef ALITRDDIGITSTASK_H
#define ALITRDDIGITSTASK_H

// example of an analysis task to analyse TRD digits 
// Authors: Tom Dietel
// based on the AliAnalysisTaskPt

class TH1F;
class AliESDEvent;

#include "AliAnalysisTaskSE.h"

class AliTRDdigitsTask : public AliAnalysisTaskSE {
public:
  AliTRDdigitsTask() : AliAnalysisTaskSE(), fESD(0), fOutputList(0), fHistPt(0) {}
  AliTRDdigitsTask(const char *name);
  virtual ~AliTRDdigitsTask() {}
  
  virtual void   UserCreateOutputObjects();
  virtual Bool_t UserNotify();
  virtual void   UserExec(Option_t *option);
  virtual void   Terminate(Option_t *);

  void SetDigitsInputFilename(TString x)
  
protected:

  void ReadDigits();
  void WriteDigits();

  AliTRDdigitsManager* fDigMan;        //! digits manager


  
private:
  AliESDEvent *fESD;    //! ESD object
  TList       *fOutputList; //! Output list
  TH1F        *fHistAdcSpectrum; //! TRD ADC spectrum
  TH2F        *fHistPadResponse; //! (pseudo) pad response function

  TFile* fDigitsInputFileName;         //! Name of digits file for reading
  TFile* fDigitsOutputFileName;        //! Name of digits file for writing
  
  TFile* fDigitsInputFile;             //! Digits file for reading
  TFile* fDigitsOutputFile;            //! Digits file for writing

  Int_t fEventNoInFile;

  
  AliTRDdigitsTask(const AliTRDdigitsTask&); // not implemented
  AliTRDdigitsTask& operator=(const AliTRDdigitsTask&); // not implemented
  
  ClassDef(AliTRDdigitsTask, 1); // example of analysis
};

#endif
