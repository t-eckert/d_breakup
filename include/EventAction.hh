#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class EventAction : public G4UserEventAction
{
public:
    EventAction();
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void AddDetector(G4double de, G4double dl);

private:
    G4double fEnergyDetector;
    G4double fTrackLDetector;
};

inline void EventAction::AddDetector(G4double de, G4double dl) 
{
    fEnergyDetector += de;
    fTrackLDetector += dl;
}

#endif