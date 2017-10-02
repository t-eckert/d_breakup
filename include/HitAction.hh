#ifndef HitAction_h
#define HitAction_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class HitAction : public G4VHit
{
public: 
    HitAction();
    ~HitAction();
    HitAction(const HitAction &right);
    const HitAction& operator=(const HitAction &right);
    int operator==(const HitAction &right) const;

    inline void * operator new(size_t);
    inline void operator delete(void *aHit);

    void Draw() const;
    void Print() const;

private:
    G4double edep;

public:
    inline void SetEdep(G4double de){edep=de;}
    inline G4double GetEdep() const {return edep;}
};

typedef G4THitsCollection<HitAction> HitAction_Collection;

extern G4Allocator<HitAction> HitAction_Allocator;

inline void HitAction::operator delete(void *aHit)
{
  HitAction_Allocator.FreeSingle((HitAction*) aHit);
}

#endif
