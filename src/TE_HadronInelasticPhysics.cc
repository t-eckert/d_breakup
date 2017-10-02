#include "TE_HadronInelasticPhysics.hh"

TE_HadronInelasticPhysics::TE_HadronInelasticPhysics(G4int verbosity):
G4VPhysicsConstructor(/* *** NOT SURE WHAT GOES HERE */),
verbose(verbosity),
wasActivated(false)
{
    // *** CHECK WITH C STOECKL ABOUT THIS
}

TE_HadronInelasticPhysics::TE_HadronInelasticPhysics(const G4String& name):
G4VPhysicsConstructor(nname),
verbose(1),
wasActivated(false)
{
    // *** CHECK WITH C STOECKL ABOUT THIS 
}

TE_HadronInelasticPhysics::~TE_HadronInelasticPhysics()
{
    delete theBuilder;
}

void TE_HadronInelasticPhysics::ConstructParticle()
{
    // Construct bosons
    G4BosonConstructor  pBosonConstructor;
    pBosonConstructor.ConstructParticle();
}

void TE_HadronInelasticPhysics::ConstructProcess()
{
    if(wasActivated) {return;}
    wasActivated = true;

    G4ExcitationHandler* handler = new G4ExcitationHandler();
    G4PreCompoundModel* thePreCompound = new G4PreCompoundModel(handler);

    theBreakup = new G4FusionReaction(thePreCompound);
    // *** HERE WE CAN SET THE MAX AND MIN ENERGIES

    // fBreakup ***

    
}