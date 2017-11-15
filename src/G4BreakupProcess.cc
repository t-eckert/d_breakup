// Breakup Process Class
 
#include "G4BreakupProcess.hh"
#include "G4GenericIon.hh"
#include "G4ParticleDefinition.hh"
  
G4BreakupProcess::G4BreakupProcess(
    const G4String& processName,
    G4ParticleDefinition* aParticle
): G4HadronicProcess(processName, fHadronInelastic) // was fHadronInelastic from G4HadronicProcessType.hh
{
    // AddDataSet(new G4HadronFusionDataSet());
    theParticle = aParticle;
}

G4BreakupProcess::~G4BreakupProcess() 
{}

G4bool G4BreakupProcess::IsApplicable(const G4ParticleDefinition& aP)
{
  G4cout << "G4BreakupProcess::IsApplicable called! "  << G4endl;
  return  theParticle == &aP || theParticle == G4GenericIon::GenericIon();
}