#include "PhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

// Physics
#include "G4HadronElasticPhysics.hh"
//#include "GammaPhysics.hh"
//#include "G4HadronElasticPhysicsHP.hh"

// Particles
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

PhysicsList::PhysicsList():
G4VModularPhysicsList()
{
    G4int ver = 0;
    SetVerboseLevel(ver);

    new G4UnitDefinition( "mm2/g", "mm2/g","Surface/Mass", mm2/g);
    new G4UnitDefinition( "um2/mg", "um2/mg","Surface/Mass", um*um/mg);

    RegisterPhysics(new G4HadronElasticPhysics(ver));
}

PhysicsList::~PhysicsList()
{}

void PhysicsList::ConstructParticle()
{
    G4BosonConstructor  pBosonConstructor;
    pBosonConstructor.ConstructParticle();

    G4LeptonConstructor pLeptonConstructor;
    pLeptonConstructor.ConstructParticle();

    G4MesonConstructor pMesonConstructor;
    pMesonConstructor.ConstructParticle();

    G4BaryonConstructor pBaryonConstructor;
    pBaryonConstructor.ConstructParticle();

    G4IonConstructor pIonConstructor;
    pIonConstructor.ConstructParticle();

    G4ShortLivedConstructor pShortLivedConstructor;
    pShortLivedConstructor.ConstructParticle();
}

/* 
void PhysicsList::SetCuts()
{
  SetCutValue(0.01*mm, "proton");
  SetCutValue(0.0001*mm, "triton");
  SetCutValue(0.01*mm, "e-");
  SetCutValue(0.01*mm, "e+");
}
*/