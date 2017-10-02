#include <algorithm>
#include <vector>
#include <cmath>
#include <numeric>

#include "BreakupReaction.hh"

BreakupReaction::BreakupReaction(G4VPreCompoundModel* ptr):
G4HadronicInteraction("Breakup"),
theProjectileFragmentation(ptr),
pA(0), 
pZ(0),
tA(0),
tZ(0),
spectatorA(0),
spectatorZ(0),
projectile3dNucleus(0),
target3dNucleus(0)
{}

BreakupReaction::~BreakupReaction()
{}

void BreakupReaction::ModelDescription(std::ostream& outFile) const
{
    outFile << "Breakup test" << endl;
}

struct ReactionProduct4Momentum
{
    G4LorentzVector operator()(G4LorentzVector a, G4ReactionProduct* b)
    {
{return a + G4LorentzVector(b->GetMomentum(), b->GetTotalEnergy() );}
    }
}
