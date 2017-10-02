#ifndef BreakupCrossSection_h
#define BreakupCrossSection_h

#include <vector>
#include "G4BinaryCascade.hh"
#include "G4PreCompoundModel.hh"
#include "G4VCrossSectionDataSet.hh"

class BreakupCrossSection: public G4VCrossSectionDataSet
{
public:
    BreakupCrossSection();

    virtual ~BreakupCrossSection();

    virtual G4bool IsElementApplicable(
        const G4DynamicParticle* aDP,
        G4int Z,
        const G4Material*
    );

    virtual G4double GetElementCrossSection(
        const G4DynamicParticle*,
        G4int Z,
        const G4Material*
    );

    virtual G4double GetIsoCrossSection(
        const G4DynamicParticle*,
        G4int Z,
        const G4Isotope*  iso = 0,
        const G4Element*  elm = 0,
        const G4Material* mat = 0
    );

    virtual void CrossSectionDescription(std::ostream&) const;

private:
    const G4double upperLimit;
    const G4double lowerLimit; 
    const G4double r0;

    G4double GetCMEnergy(
        const G4DynamicParticle* aDParticle,
        G4int tZ,
        G4int tA
    );

    G4int GetCrossSectionIndex(
        const G4double E,
        const G4int pZ, 
        const G4int pA,
        const G4int tZ,
        const G4int tA
    );

    G4double CalcCrossSection(
        const G4double E
    );
};

#endif