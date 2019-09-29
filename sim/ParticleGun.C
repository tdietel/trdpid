AliGenerator *GeneratorCustom(TString opt = "")
{
  AliGenCocktail *gener = new AliGenCocktail();
  /*gener->SetProjectile("A", 208, 82);
  gener->SetTarget    ("A", 208, 82);
  gener->SetEnergyCMS(2760.);
  */
  // 1. Hijing
  // AliGenHijing *gh = (AliGenHijing*) Hijing();
  // gh->SetImpactParameterRange(14.9, 30.0);

  AliGenFixed *gPionGun = new AliGenFixed(2);
  gPionGun->SetMomentum(3);
  gPionGun->SetPhiRange(8./180.*TMath::Pi());
  gPionGun->SetThetaRange(0);
  gPionGun->SetOrigin(0.0,0.0,0.0);   //vertex position
  gPionGun->SetPart(211);

  gener->AddGenerator(gPionGun, "Pion gun", 1.0);

  AliGenBox* pionPlus = new AliGenBox(2); // pions+
  pionPlus->SetPart(211);
  pionPlus->SetPtRange(6., 50.);
  pionPlus->SetYRange(-0.9, 0.9);

  gener->AddGenerator(pionPlus,"pi+", 1);

  return gener;

  //
  //
  // AliGenCocktail *ctl  = GeneratorCocktail("ParticleGun");
  //
  // printf("INFO: Particle with PDG = %d will be injected in %2.1f < pT < %2.1f (GeV/c), %2.1f < y < %2.1f, %2.1f < phi < %2.1f \n",pdgConfig,ptminConfig,ptmaxConfig,yminConfig,ymaxConfig,phiminConfig,phimaxConfig);
  //
  // AliGenerator   *particle = GeneratorInjector(1, pdgConfig,ptminConfig,ptmaxConfig,yminConfig,ymaxConfig,phiminConfig,phimaxConfig);
  // ctl->AddGenerator(particle, Form("Injector (PDG %d)",pdgConfig), 1.);
  //
  // return ctl;
}
