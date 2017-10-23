#ifndef TE_HadronInelasticPhysics_h
#define TE_HadronInelasticPhysics_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"
#include "BreakupReaction.hh"

class G4HadronicInteraction;
class G4VCrossSectionDataSet;
class G4FTFBuilder;
class G4BinaryLightIonReaction;

class TE_HadronInelasticPhysics : public G4VPhysicsConstructor
{
public:
    TE_HadronInelasticPhysics(G4int verbosity = 0);
    TE_HadronInelasticPhysics(const G4String& nname);
    virtual ~TE_HadronInelasticPhysics();

    // This method will be invoked in the Construct() method.
    // Each physics process will be instantiated and 
    // registered to the process manager of each particle type
    void ConstructParticle();
    void ConstructProcess();

private:
    void AddProcess(
            const G4String&, 
            G4ParticleDefinition*,
            G4bool isIon
    );

    G4VCrossSectionDataSet*   fTripathi;
    G4VCrossSectionDataSet*   fTripathiLight;
    G4VCrossSectionDataSet*   fShen;
    G4VCrossSectionDataSet*   fIonH;
    G4VCrossSectionDataSet*   fGGNuclNucl;
	G4VCrossSectionDataSet*   fFusion;

    G4BinaryLightIonReaction* theIonBC;
    BreakupReaction* 		theFusion;
    G4HadronicInteraction*    theFTFP;
    G4FTFBuilder*             theBuilder;

    G4int  verbose;
    G4bool wasActivated;
};

#endif