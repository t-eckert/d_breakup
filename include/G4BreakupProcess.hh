/*  Breakup Process class
    The specific particle inelastic processes derive from this class
    This is an abstract base class, since the pure virtual function
    PostStepDoIt has not been defined yet.
*/

#ifndef G4BreakupProcess_h
#define G4BreakupProcess_h 1

#include "G4HadronicProcess.hh"

class G4ParticleDefinition;

class G4BreakupProcess : public G4Process
{
public:
    
  G4BreakupProcess(
        const G4String &processName,
	    G4ParticleDefinition *aParticle 
    );
    
  virtual ~G4BreakupProcess();
        
  virtual G4bool IsApplicable(const G4ParticleDefinition& aParticleType);

private:

  G4ParticleDefinition* theParticle;

};
 
#endif