#ifndef BreakupReaction_h
#define BreakupReaction_h 1

#include <vector>

#include "G4BinaryCascade.hh"
#include "G4PreCompoundModel.hh"

class BreakupReaction: public G4HadronicInteraction
{
    typedef std::vector<G4ParticleDefinition* > ReactionParticleVector;

public:
    BreakupReaction(G4VPreCompoundModel* ptr = 0);
    virtual ~BreakupReaction();
    G4HadFinalState* ApplyYourself(
        const G4HadProjectile& aTrack,
        G4Nucleus& theNucleus
    );
    inline void SetPrecompound(G4VPreCompoundModel* ptr);
    inline void SetDeExcitation(G4ExcitationHandler* ptr);

    virtual void ModelDescription(std::ostream&) const;

private:
    void GetBreakupProducts(
        G4double pA, G4double pZ,
        G4double tA, G4double tZ,
        G4ParticleDefinition* products[]
    );

    G4double Probability_Calculator(const G4double angle);

    G4BinaryCascade* theModel;
    G4ExcitationHandler* theHandler;
    G4VPreCompoundModel* theProjectileFragmentation;
    G4HadFinalState theResult;
    G4int pA, pZ, tA, tZ, spectatorA, spectatorZ;
    G4Fancy3DNucleus* projectile3dNucleus, *target3dNucleus;
    G4FermiMomentum theFermi;
    G4LorentzVector pInitialState, pFinalState;
};

inline void BreakupReaction::SetPrecompound(G4VPreCompoundModel* ptr)
{
  if(ptr) { theProjectileFragmentation = ptr; }
  theHandler = theProjectileFragmentation->GetExcitationHandler();
}

inline void BreakupReaction::SetDeExcitation(G4ExcitationHandler* ptr)
{
  theProjectileFragmentation->SetExcitationHandler(ptr);
  theHandler = ptr;
}
#endif