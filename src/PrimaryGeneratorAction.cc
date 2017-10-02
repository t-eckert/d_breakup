#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction():
G4VUserPrimaryGeneratorAction(),
fParticleGun(0)
{
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    // default particle kinematics
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particle_name = "neutron";
    G4ParticleDefinition* particle = particleTable->FindParticle(particle_name);
    fParticleGun->SetParticleDefinition(particle);

    // Randomize initial momentum direction within D Sphere 
    // G4double theta = G4UniformRand()*twopi;
    // G4double phi = G4UniformRand()*pi;

    // G4double x_p = std::cos(theta)*std::sin(phi);
    // G4double y_p = std::sin(theta)*std::sin(phi);
    // G4double z_p = std::cos(phi);

    G4ThreeVector p = G4ThreeVector(0.,0.,1);

    fParticleGun->SetParticleMomentumDirection(p);
    fParticleGun->SetParticleEnergy(14.03);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // This function is called at the beginning of each event
    fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,-1*cm));
    fParticleGun->GeneratePrimaryVertex(anEvent);
}