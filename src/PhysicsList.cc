#include "PhysicsList.hh"
#include "G4HadronElasticPhysics.hh"

#include "G4BosonConstructor.hh"
#include "TE_HadronInelasticPhysics.hh"

PhysicsList::PhysicsList():
G4VModularPhysicsList()
{
    G4Int verbosity = 0;

    SetVerboseLevel(verbosity);

    // Hadron Elastic Scattering at high precision (HP)
    RegisterPhysics(new G4HadronElasticPhysicsHP(verbosity));

    // *** I think I want to write my own HadronInelasticPhysics
    RegisterPhysics(new TE_HadronInelasticPhysics(verbosity));

    // Ion Physics 
   //RegisterPhysics(new G4IonPhysics(verbosity))
}

PhysicsList::~PhysicsList()
{}

void PhysicsList::ConstructParticle()
{
    G4BosonConstructor pBosonConstructor;
    pBosonConstructor.ConstructParticle();

    //G4IonConstructor pIonConstructor;
    //pIonConstructor.ConstructParticle();
}

