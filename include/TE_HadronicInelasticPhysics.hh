#ifndef TE_HadronicInelasticPhysics_h
#define TE_HadronicInelasticPhysics_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"
// #include "G4FusionReaction.hh" *** I DONT THINK I NEED THIS

class G4HadronicInteraction;
class G4VCrossSectionDataSet;
class G4FTFBuilder;

class TE_HadronicInelasticPhysics: public G4VPhysicsConstructor
{
public:
    TE_HadronicInelasticPhysics(G4int verbosity = 0);
    TE_HadronicInelasticPhysics(const G4String& nname);
    virtual ~TE_HadronicInelasticPhysics();

  // This method will be invoked in the Construct() method.
  // each physics process will be instantiated and
  // registered to the process manager of each particle type
  void ConstructParticle();
  void ConstructProcess(); 
  
private:
    void AddProcess(const G4String&, G4ParticleDefinition*, G4bool isHadron/* *** THIS MAY NOT BE CORRECT*/);
        // *** NOT SURE HOW TO HANDLE THIS:
        /*
        G4VCrossSectionDataSet*   fTripathi;
        G4VCrossSectionDataSet*   fTripathiLight;
        G4VCrossSectionDataSet*   fShen;
        G4VCrossSectionDataSet*   fIonH;
        G4VCrossSectionDataSet*   fGGNuclNucl;
	    G4VCrossSectionDataSet*   fFusion;

        G4BinaryLightIonReaction* theIonBC;
        G4FusionReaction* 		theFusion;
        G4HadronicInteraction*    theFTFP;
        G4FTFBuilder*             theBuilder;
        */

    G4int verbosity;
    G4bool wasActivated;
};

#endif