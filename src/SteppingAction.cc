#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "Analysis.hh"

#include "G4UIcommand.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4VProcess.hh"
#include "G4Track.hh"

extern volatile G4double tag;
extern volatile G4double x_int;
extern volatile G4double y_int;
extern volatile G4double z_int;

SteppingAction::SteppingAction(EventAction* eventAction):
G4UserSteppingAction(),
fEventAction(eventAction),
fScoringVolume(0)
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    // Get the scoring volume
    if(!fScoringVolume)
    {
        const DetectorConstruction* detectorConstruction
        = static_cast<const DetectorConstruction*> 
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        fScoringVolume = detectorConstruction->GetScoringVolume();
    }

    
    // Point to the track
    G4Track* track = step->GetTrack();

    // Get the name of the particle
    G4String particle_name = track->GetDefinition()->GetParticleName();

    if(particle_name == "neutron"){
        // Get volume of the current step and check if we are in the scoring volume
        G4LogicalVolume* volume
        = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

        G4int trackID = track->GetTrackID();
        G4int n_secondaries = step->GetNumberOfSecondariesInCurrentStep();
        G4String process_name = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

        G4ThreeVector position = track->GetPosition();
        G4double x_pos = position.x();
        G4double y_pos = position.y();
        G4double z_pos = position.z();

        if(tag==0 && trackID!=1){
            tag = -1;
        }

        if(process_name=="neutronInelastic" && n_secondaries==3){
            tag=2;                      // breakup neutron
            x_int=x_pos;
            y_int=y_pos;
            z_int=z_pos;
        } 

        if(process_name=="hadElastic"){
            tag=1;                      // scattered neutron
            x_int=x_pos;
            y_int=y_pos;
            z_int=z_pos;
        }

        if(volume==fScoringVolume && tag == 2){
            G4double kinetic_energy = track->GetKineticEnergy();

            G4cout  << "Out: " << kinetic_energy << ", "
                    << x_pos << ", "
                    << y_pos << ", "
                    << z_pos << ", "
                    << tag   << ", "
                    << x_int << ", "
                    << y_int << ", "
                    << z_int << G4endl;
        };
    }
}
