/*  Breakup Process class
    The specific particle inelastic processes derive from this class
    This is an abstract base class, since the pure virtual function
    PostStepDoIt has not been defined yet.
*/

#ifndef G4HadronBreakupProcess_h
#define G4HadronBreakupProcess_h 1

#include "G4HadronicProcess.hh"

class G4ParticleDefinition;

class G4HadronBreakupProcess : public G4HadronicProcess
{
public:
    
  G4HadronBreakupProcess(
      const G4String &processName,
	    G4ParticleDefinition *aParticle 
    );
    
  virtual ~G4HadronBreakupProcess();
        
  virtual G4bool IsApplicable(const G4ParticleDefinition& aParticleType);

private:

  G4ParticleDefinition* theParticle;

};
 
#endif