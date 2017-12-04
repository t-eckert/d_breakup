#ifndef Steppingaction_h
#define Steppingaction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class DetectorConstruction;
class EventAction;

// In UserSteppingAction(), there are collected the energy deposit and track
// lengths of charged particles in Absorber and Gap layers and updated in 
// EventAction

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction(
        const DetectorConstruction* detectorConstruction,
        EventAction* eventAction
    );
    virtual ~SteppingAction();

    // method from user base class
    virtual void UserSteppingAction(const G4Step* step);

private: 
    EventAction* fEventAction;
    G4LogicalVolume* fScoringVolume;

};

#endif