#include "EventAction.hh"
#include "RunAction.hh"
#include "Analysis.hh"
#include "SteppingAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

G4double tag = 0.;
G4double x_int = 0.;
G4double y_int = 0.;
G4double z_int = 0.;
    
EventAction::EventAction(RunAction* runAction):
G4UserEventAction(),
fRunAction(runAction)
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
    tag = 0.;
    x_int = 0.;
    y_int = 0.;
    z_int = 0.;
}

void EventAction::EndOfEventAction(const G4Event*)
{
    //G4cout << particle_name << G4endl;
}    