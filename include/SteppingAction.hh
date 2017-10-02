#ifndef Steppingaction_h
#define Steppingaction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class EventAction;

class G4LogicalVolume;

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction(EventAction* eventAction);
    virtual ~SteppingAction();

    // method from user base class
    virtual void UserSteppingAction(const G4Step*);

private: 
    EventAction* fEventAction;
    G4LogicalVolume* fScoringVolume;

};

#endif